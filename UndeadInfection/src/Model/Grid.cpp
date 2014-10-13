/*
 * Grid.cpp
 *
 *  Created on: 21/08/2014
 *      Authors: Andres Chaves, Gustavo Carrion, Diego Montufar
 */

#include "Grid.h"
#ifdef DEBUG
#include <iostream>
#endif

using namespace std;
Grid::Grid() {}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}
#if defined(_OPENMP)
void lock(int i, bool *locks) {
	for (bool locked = false; locked == false; /*NOP*/) {
#pragma omp critical (LockRegion)
		{
			locked = !locks[i-1] && !locks[i] && !locks[i+1];
			if (locked) {
				locks[i-1] = true; locks[i] = true; locks[i+1] = true;
			}
		}
	}
}
void unlock(int i, bool *locks) {
#pragma omp critical (LockRegion)
	{
		locks[i-1] = false; locks[i] = false; locks[i+1] = false;
	}
}
void lock2(int i, bool *locks) {
	for (bool locked = false; locked == false; /*NOP*/) {
#pragma omp critical (LockRegion)
		{
			int im2 = i>2?i - 2:0;
			int ip2 = i<GRIDROWS ?i + 2:GRIDROWS;

			locked = !locks[im2] && !locks[i-1] && !locks[i] && !locks[i+1] && !locks[ip2];
			if (locked) {
				locks[im2] = true; locks[i-1] = true; locks[i] = true; locks[i+1] = true;locks[ip2] = true;
			}
		}
	}
}
void unlock2(int i, bool *locks) {
#pragma omp critical (LockRegion)
	{
		int im2 = i>2?i - 2:0;
		int ip2 = i<GRIDROWS ?i + 2:GRIDROWS;

		locks[im2] = false;locks[i-1] = false; locks[i] = false; locks[i+1] = false;locks[ip2] = false;
	}
}
#endif

Agent** Grid::createMesh (){
	Agent** grid = new Agent*[GRIDROWS+2];
	for (int i = 0; i <= GRIDROWS+1; i++) {
		grid[i] = new Agent[GRIDCOLUMNS+2];
		for (int j = 0; j <= GRIDCOLUMNS+1; j++) {
			grid[i][j].clean();
		}
	}
	return grid;
}
void Grid::initialize(	Agent** gridA, Agent** gridB ) {
	int numZombies = 0;
	int numHumans = 0;
	RandomClass random;
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridB[i][j].clean();
			//todo:change algorithm so it is random and with pop limits
			if (numHumans < DARWINPOPDENSITY && random.randomBoolFalseBiasedN()) {
				int age = random.random(MINLIFEEXPECTANCY - 1);
				int lifeExpectancy=random.random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY);
				bool gender = random.random() < GENDERRATIO ? true : false;
				bool hasAGun = random.random() < GUNDENSITY ? true : false;

				gridA[i][j].migrateToHuman(gender, age, hasAGun, lifeExpectancy);
				numHumans++;
			} else {
				if (numZombies < NUMBEROFZOMBIES && random.randomBoolFalseBiasedZN()) {
					int lifeTime = random.random(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME);
					gridA[i][j].migrateToZombie(lifeTime);
					numZombies++;
				}// else {
				//	gridA[i][j] = NULL;
				//}
			}
			//cout << "i:  "<<i << "j:  "<<j <<"numInitialized with humans: "<< endl;
		}
	}
	cout << "Initialized with Humans: " << numHumans << " and Zombies: " << numZombies << endl;
}
void Grid::run() {
#if defined(_OPENMP) //Cout whether we are working with opemp or not
	cout << "Starting parallel simulation... " << endl;
	cout << "Human: " << human << endl;
	cout << "Zombie: " << zombie << endl;
	cout << "None: " << none << endl;
#else
	cout << "Starting sequential simulation... " << endl;
#endif
	Agent** gridA = createMesh();
	Agent** gridB = createMesh();
	int N_Threads = 1;

	float freeCells=0,totalHumans = 0,totalZombies = 0, deathRate = DEATHRATENT, acumProbOfDying=DEATHRATENT/DARWINPOPDENSITY;
	int shooted = 0, infected = 0, converted = 0, ghostCase = 0, zDead = 0, hDead = 0, outOfBounds = 0, born = 0;

	bool *locks = new bool[GRIDCOLUMNS + 2];
	initialize(gridA, gridB);
#if defined(_OPENMP) //Record starting time and number of threads
	double		wtime		= omp_get_wtime();			// Record the starting time
	N_Threads	= omp_get_max_threads();
	RandomClass randomObj[N_Threads];
	cout << "Num. threads: " << N_Threads << endl;
#else
		RandomClass randomObj[1];
#endif

	for (int i = 0; i < N_Threads; i++){
		//Linux
		randomObj[i].setSeed(i*0xFFFF*drand48());
		//Windows
		//randomObj[i].setSeed(i*0xFFFF*rand());
	}

	for (int i = 0; i < GRIDCOLUMNS + 2; i++) locks[i] = false;

	printState(0, gridA, infected, converted, shooted, zDead, ghostCase, hDead, born);
	//printMatrix(0);
#ifdef DEBUGGRID
	//std::cout << "Run called" << "\n";
#endif
	int markedAsDeadCounter=0;
	//Time loop
	for (int n = 0; n < NUMTICKS; n++) {
		//Code to calculate the number of humans to be removed
		double humansToDie=deathRate*(n+1);
		//cout<<humansToDie<<endl;
#if defined(_OPENMP)
#pragma omp parallel default(none) shared (locks, gridA, gridB, freeCells, totalHumans, totalZombies, deathRate, cout, n, randomObj) reduction(+:shooted, infected, converted, ghostCase, zDead, hDead, born, markedAsDeadCounter, humansToDie)
		{
			//int         tid = omp_get_thread_num();
			RandomClass random = randomObj[omp_get_thread_num()];
#else
			RandomClass random = randomObj[0];
#endif
#if defined(_OPENMP)
      #pragma omp for
#endif
			for (int i = 1; i <= GRIDROWS; i++) { //Move, delete and convert
#if defined (_OPENMP)
				lock(i, locks);
#endif
				for (int j = 1; j <= GRIDCOLUMNS; j++) {
					if ( gridA[i][j].getType() != none ) {
						Agent agent = gridA[i][j];
						//////CHECK THISSS gridA[i][j] = NULL;

						agent.step();

						//Code to remove decomposed agents
						if ((agent.getType() == zombie) && (agent.isDecomposed() || agent.isShooted())) {
							//Delete?
							//Counters::getInstance().newZombieDead();
							zDead++;
							agent.clean();
						} else {
							if ( agent.getType() == human && agent.isNaturalDead() ) {
								//Delete?
								//Counters::getInstance().newHumanDead();
								hDead++;
								agent.clean();
							} else {
								if ( agent.getType() == human && agent.isInfected() ) {
									//Counters::getInstance().newConversion();
									converted++;
									agent.migrateToZombie(random.random(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME));
								}
							}
						}
						//Agent already dead no comparisons required
						if (agent.getType() != none) {
							double move = random.random();
							int desti = 0;
							int destj = 0;

							if (move < 1.0 * MOVE) {
								desti = i - 1;
								destj = j;
							} else if (move < 2.0 * MOVE) {
								desti = i + 1;
								destj = j;
							} else if (move < 3.0 * MOVE) {
								desti = i;
								destj = j - 1;
							} else if (move < 4.0 * MOVE) {
								desti = i;
								destj = j + 1;
							} else {
								desti = i;
								destj = j;
							}
							if (gridA[desti][destj].getType() == none && gridB[desti][destj].getType() == none) {
								gridB[desti][destj].clone(agent);
								//#ifdef DEBUGGRID
								//std::cerr << "Tick: " << (n+1) << "Human moved from" << i <<","<< j << " to "<< desti <<","<< destj<<"\n";
								//#endif

							} else {
								gridB[i][j].clone(agent);
							}
						}
						agent.clean();
					}
				}
#if defined(_OPENMP)
				unlock(i,locks);
#endif

			}

		//Boundary condition
#if defined(_OPENMP)
#pragma omp single
		{
			//cout << tid << "TS:" << n << "Exec single" << endl;
#endif
		for (int i = 1; i <= GRIDROWS; i++) {
			if ( gridB[i][0].getType() != none && gridB[i][1].getType() == none) {
				Agent a = gridB[i][0];
				gridB[i][1].clone(a);
				gridB[i][0].clean();
			} else if ( gridB[i][0].getType() != none && gridB[i][1].getType()!=none ) {
				gridB[i][0].clean();
				ghostCase++;
			}
			if (gridB[i][GRIDCOLUMNS + 1].getType() != none && gridB[i][GRIDCOLUMNS].getType() == none ) {
				gridB[i][GRIDCOLUMNS].clone(gridB[i][GRIDCOLUMNS + 1]);
				gridB[i][GRIDCOLUMNS + 1].clean();
			} else if (gridB[i][GRIDCOLUMNS + 1].getType()!=none && gridB[i][GRIDCOLUMNS].getType() != none) {
				gridB[i][GRIDCOLUMNS + 1].clean();
				ghostCase++;
			}
		}

		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			if (gridB[0][j].getType() != none && gridB[1][j].getType() == none) {
				gridB[1][j].clone(gridB[0][j]);
				gridB[0][j].clean();
			} else if (gridB[0][j].getType() != none && gridB[1][j].getType() != none) {
				gridB[0][j].clean();
				ghostCase++;
			}
			if (gridB[GRIDROWS + 1][j].getType()!=none && gridB[GRIDROWS][j].getType() == none) {
				gridB[GRIDROWS][j] = gridB[GRIDROWS + 1][j];
				gridB[GRIDROWS + 1][j].clean();
			} else if (gridB[GRIDROWS + 1][j].getType() != none && gridB[GRIDROWS][j].getType() != none) {
				gridB[GRIDROWS + 1][j].clean();
				ghostCase++;
			}
		}
		totalHumans = totalZombies = freeCells = 0;
		calculateStatistics(totalHumans,totalZombies, freeCells, gridB);
		deathRate = DEATHRATENT;
#if defined(_OPENMP)
     }
#endif
		//Resolve conflicts between Agents
#if defined(_OPENMP)
		//cout << tid << "TS:" << n << "humans" << totalHumans << "Death rate" << deathRate << endl;
#pragma omp for
#endif
		for (int i = 1; i <= GRIDROWS; i++) {
#if defined (_OPENMP)
				lock2(i, locks);
#endif

			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				//cout <<"Marked as dead counter: "<<markedAsDeadCounter<< "Humans to die:" << humansToDie << "Chance to die " <<chanceToDie<< endl;
				Agent &agentA = gridB[i][j];
				if (agentA.getType() != none) {
					AgentTypeEnum typeA = agentA.getType();
					if (typeA == human) {
						if (i > 2 && gridB[i - 1][j].getType() != none) { //Someone up
							if (gridB[i - 1][j].getType() == zombie)
								resolveGridHumanZombie(agentA,i - 1,j, gridB, random, shooted, infected);
							else
								resolveGridHumanHuman(agentA,i - 1,j, gridB, random, totalHumans, totalZombies, freeCells, born);
						}
						if (j < GRIDROWS && gridB[i][j + 1].getType() != none) { //Someone on the right
							if (gridB[i][j+1].getType() == zombie)
								resolveGridHumanZombie(agentA,i,j + 1, gridB, random, shooted, infected);
							else
								resolveGridHumanHuman(agentA,i,j+1, gridB, random, totalHumans, totalZombies, freeCells, born);
						}
						if (i < GRIDROWS && gridB[i + 1][j].getType() != none) { //Down
							if (gridB[i+1][j].getType() == zombie)
								resolveGridHumanZombie(agentA,i + 1,j, gridB, random, shooted, infected);
							else
								resolveGridHumanHuman(agentA,i + 1,j, gridB, random, totalHumans, totalZombies, freeCells, born);
						}
						if (j > 2 && gridB[i][j - 1].getType() != none) { //Left
							if (gridB[i][j-1].getType() == zombie)
								resolveGridHumanZombie(agentA,i,j - 1, gridB, random, shooted, infected);
							else
								resolveGridHumanHuman(agentA,i,j - 1, gridB, random, totalHumans, totalZombies, freeCells, born);
						}
						//Death process
						if (!agentA.isInfected()){

							double move = random.random();
		//					cout << "TS:" << n << "Death " <<move << "," << deathRate << endl;
							//cout << "TS:" << n << "Death " <<move << "," << deathRate <<"," << humansToDie<<"," << chanceToDie << endl;
							if (move < 0.001 && markedAsDeadCounter<=humansToDie) {
							    markedAsDeadCounter++;
								agentA.markAsDead();
							}

						}
					}
				}
			}
#if defined(_OPENMP)
				unlock2(i,locks);
#endif
		}
#if defined(_OPENMP)
	} //Parallel region
#endif
		//Calculate acum deaths
		deathRate=acumProbOfDying*totalHumans;
		acumProbOfDying=deathRate/totalHumans;



		swap(gridA, gridB);
		///Clean gridB
		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				gridB[i][j].clean();
			}
		}


		if ( n%NUMTICKSPRINT == 0 ){
			printState(n + 1, gridA, infected, converted, shooted, zDead, ghostCase, hDead, born);
			//printMatrix(n + 1);
			shooted=infected=zDead=outOfBounds=ghostCase=hDead=born=converted=0;
		}
	}
	printState(NUMTICKS + 1, gridA, infected, converted, shooted, zDead, ghostCase, hDead, born);
#if defined(_OPENMP)
	wtime	= omp_get_wtime() - wtime;	// Record the end time and calculate elapsed time
	cout << "Simulation took " << wtime/NUMTICKS << " seconds per time step with " << N_Threads << " threads" << endl;
#endif
}

/* Print Grid*/
void Grid::printMatrix(int tick, Agent** gridA) {
	cout << endl;

	cout << "|";
	//for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
	//	cout << j << (j <= 9 ? " |" : "|");
	//}
	cout << endl;
	for (int i = 0; i <= GRIDROWS + 1; i++) {
		for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
			cout << "--";
		}
		cout << endl;
		for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
			if (j == 1)
				cout << "|";
			Agent agent = gridA[i][j];
			if (agent.getType() != none) {
				AgentTypeEnum currentType = agent.getType();
				if (currentType == human)
					cout << "h|";
				else
					cout << "z|";
			} else {
				cout << " |";
			}
		}
		cout << endl;
	}
}

/*Calculates:
 *
 * humanPopulation = Current number of humans within the grid
 * zombies         = Current number of zombies within the grid
 * freeCells       = Current number of free cells in the grid
 * gridA           = The current grid
 *
 * */
void Grid::calculateStatistics(float &humanPopulation,float &zombies, float &freeCells, Agent** gridA) {
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			Agent agent = gridA[i][j];
			if (agent.getType() == human)
				humanPopulation += 1.0;
			else if (agent.getType() == zombie)
				zombies += 1.0;
			else
				freeCells += 1.0;
		}
	}
}

/*Prints the state of the gridA in any time step*/
void Grid::printState(int tick, Agent** gridA, int infected, int converted, int shooted, int zDead, int ghostCase, int hDead, int born) {
	int female = 0;
	int male = 0;
	int zombies = 0;
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
#ifdef DEBUG
			//			std::cout << "printState inspect Agent" << i << "," << j << "\n";
#endif
			Agent agent = gridA[i][j];
			AgentTypeEnum currentType = agent.getType();
			if (currentType == human){
				if (agent.getGender())
					male++;
				else
					female++;
			}
			else if (currentType == zombie)
					zombies++;
			}
	}
#ifdef DEBUG

	std::cout << "Tick:" << tick << ", Humans: " << male+female << ", Males: "<< male << ", Females: " << female << ", Zombies: " << zombies;
	std::cout << ", Infected:" << infected;
	std::cout << ", Converted: " << converted << ",Shots: " << shooted;
	std::cout << ", Dead Deaths: " << zDead << ",Ghost Cases: " << ghostCase;
	std::cout << ", Natural Deaths: " << hDead << ",Births: " << born << "\n";

#endif
}

/*Resolve conflicts between Human-Zombie*/
void Grid::resolveGridHumanZombie(Agent &agentA,int i, int j, Agent** gridB, RandomClass random, int &shooted, int &infected) {
	Agent agentB = gridB[i][j];
	AgentTypeEnum typeB = agentB.getType();
	if (typeB == zombie) {
		resolveHumanZombie(agentA, agentB, random, shooted, infected);
	}

}

/*Resolve conflicts between Human-Zombie*/
void Grid::resolveHumanZombie(Agent &human, Agent &zombie, RandomClass random, int &shooted, int &infected) {
	//Probability and cases, for now it is a rand
	int dice_roll = random.random(0,100);

	if (dice_roll <= HEADSHOTPERCENTAGE) {
		zombie.shoot();
		//Counters::getInstance().newShooted();
		shooted++;
	} else if (dice_roll <= SUCESSFULBITEPERCENTAGE) {
		human.infect();
		//Counters::getInstance().newInfected();
		infected++;
	}
}

/*Resolve conflicts between Human-Human*/
void Grid::resolveGridHumanHuman(Agent &agentA,int i, int j, Agent** gridB, RandomClass random, float totalHumans, float totalZombies, float freeCells, int &born) {
	Agent agentB = gridB[i][j];
	AgentTypeEnum typeB = agentB.getType();
	if (typeB == human) {
		//If humans have opposite genders, then try the chances to have a baby
		if ((agentA.getGender() && !agentB.getGender()) || (!agentA.getGender() && agentB.getGender())){

			//Only Humans between 25 and 45 years can have a baby
			if ( agentA.getAge()>=MINFERTILITYAGE && agentA.getAge()<=MAXFERTILITYAGE && agentB.getAge()>=MINFERTILITYAGE && agentB.getAge()<=MAXFERTILITYAGE ){

				//1. Calculate statistics
				float delta = 0,probPair=0,probAnyHumanHaveBaby=0;


				//2. Calculate probabilities
				delta = totalHumans / (TOTALGRIDCELLS - totalZombies); //delta is the density of humans
				probPair = 1-pow((1-delta),4); //Prob. two humans being pair 1-(1-delta)^4
				probAnyHumanHaveBaby = probPair * PROBTOHAVEBABYWHENPAIRED; //Prob. any Human will have a baby (Per capita birth rate)

				//Roll a dice
				double birth = random.random();

				//Find the first free space around the agent to place the baby
				//freeI,freeJ ???
				int freeI=-1,freeJ=-1;
				bool foundIt = false; //help us to found the first free space

				if ( birth <= probAnyHumanHaveBaby){
					//cout << birth << "," << probAnyHumanHaveBaby << endl;
					//Its a baby!
					//cout<< "Random: "<< move <<"Chance: "<<prob <<endl;
					bool gender = random.random() < GENDERRATIO ? true : false; //consider demographics and ages
					bool hasAGun = false; //->a baby does not have a gun
					int lifeExpectancy=random.random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY);


					/*Find a free space for the baby*/
					if ( !foundIt && i  > 2 && gridB[i - 1][j].getType() == none ) { //Someone up
						freeI=i-1,freeJ=j;
						foundIt = true;
					}
					if ( !foundIt && j < GRIDROWS && gridB[i][j + 1].getType() == none ) { //Someone on the right
						freeI=i,freeJ=j+1;
						foundIt = true;
					}
					if ( !foundIt && i < GRIDROWS && gridB[i + 1][j].getType() == none ) { //Down
						freeI=i+1,freeJ=j;
						foundIt = true;
					}
					if ( !foundIt && j > 2 && gridB[i][j - 1].getType() == none ) { //Left
						freeI=i,freeJ=j-1;
						foundIt = true;
					}
					if ( !foundIt && i > 4 && gridB[i - 2][j].getType() == none ) { //Someone up up
						freeI=i-2,freeJ=j;
						foundIt = true;
					}
					if ( !foundIt && j < GRIDROWS - 2 && gridB[i][j + 2].getType() == none ) { //Someone on the right right
						freeI=i,freeJ=j+2;
						foundIt = true;
					}
					if ( !foundIt && i < GRIDROWS -2  && gridB[i + 2][j].getType() == none ) { //Down Down
						freeI=i+2,freeJ=j;
						foundIt = true;
					}
					if ( !foundIt && j > 4 && gridB[i][j - 2].getType() == none ) { //Left Left
						freeI=i,freeJ=j-2;
						foundIt = true;
					}

					//If we found a free space to the new born, place it, otherwise kill him! :(
					if (freeI!=-1 && freeJ!=-1 && foundIt ){
						gridB[freeI][freeJ].migrateToHuman(gender, 1, hasAGun, lifeExpectancy);
						born++;
					}
				}
			}
		}
	}
}


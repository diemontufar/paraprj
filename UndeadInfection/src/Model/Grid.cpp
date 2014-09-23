/*
 * Grid.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
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
			locked = !locks[i-2] && !locks[i-1] && !locks[i] && !locks[i+1] && !locks[i+2];
			if (locked) {
				locks[i-2] = true; locks[i-1] = true; locks[i] = true; locks[i+1] = true;locks[i+2] = true;
			}
		}
	}
}
void unlock2(int i, bool *locks) {
#pragma omp critical (LockRegion)
	{
		locks[i-2] = false;locks[i-1] = false; locks[i] = false; locks[i+1] = false;locks[i+2] = false;
	}
}
#endif

Agent*** Grid::createMesh (){
	Agent*** grid = new Agent**[GRIDROWS+2];
	for (int i = 0; i <= GRIDROWS+1; i++) {
		grid[i] = new Agent*[GRIDCOLUMNS+2];
		for (int j = 0; j <= GRIDCOLUMNS+1; j++) {
			grid[i][j] = NULL;
		}
	}
	return grid;
}
void Grid::initialize(	Agent*** gridA, Agent*** gridB ) {
	int numZombies = 0;
	int numHumans = 0;
	RandomClass random(0);
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridB[i][j] = NULL;
			//todo:change algorithm so it is random and with pop limits
			if (numHumans < DARWINPOPDENSITY && random.randomBoolFalseBiasedN()) {
				int age = random.random(MINLIFEEXPECTANCY - 1);
				int lifeExpectancy=random.random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY);
				bool gender = random.random() < GENDERRATIO ? true : false;
				bool hasAGun = random.random() < GUNDENSITY ? true : false;

				gridA[i][j] = new Agent(gender, age, hasAGun, lifeExpectancy, human);
				numHumans++;
			} else {
				if (numZombies < NUMBEROFZOMBIES && random.randomBoolFalseBiasedZN()) {
					int lifeTime = random.random(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME);
					gridA[i][j] = new Agent(lifeTime, zombie);
					numZombies++;
				} else {
					gridA[i][j] = NULL;
				}
			}
			//cout << "i:  "<<i << "j:  "<<j <<"numInitialized with humans: "<< endl;
		}
	}
	cout << "Initialized with humans: " << numHumans << "- Zombies: " << numZombies << endl;
}

/*THIS METHOD IS UNUSED!!!!!*/
void Grid::merge(Agent*** gridA, Agent*** gridB) {
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridA[i][j] = gridB[i][j];
		}
	}
}
void Grid::run() {
#if defined(_OPENMP) //Cout whether we are working with opemp or not
	cout << "Starting parallel simulation... " << endl;
#else
	cout << "Starting sequential simulation... " << endl;
#endif
	Agent*** gridA = createMesh();
	Agent*** gridB = createMesh();
	float freeCells=0,totalHumans = 0,totalZombies = 0, deathRate = 0;
	bool *locks = new bool[GRIDCOLUMNS + 2];
	initialize(gridA, gridB);

#if defined(_OPENMP) //Record starting time and number of threads
	double		wtime		= omp_get_wtime();			// Record the starting time
	int			N_Threads	= omp_get_max_threads();
	cout << "Num. threads: " << N_Threads << endl;
#endif

	for (int i = 0; i < GRIDCOLUMNS + 2; i++) locks[i] = false;

	printState(0, gridA);
	//printMatrix(0);
#ifdef DEBUGGRID
	std::cout << "Run called" << "\n";
#endif

	//Time loop
	for (int n = 0; n < NUMTICKS; n++) {
#if defined(_OPENMP)
#pragma omp parallel default(none) shared (locks, gridA, gridB, freeCells, totalHumans, totalZombies, deathRate, cout, n)
		{
			int         tid = omp_get_thread_num();
			RandomClass random(omp_get_thread_num());
#else
			RandomClass random = new RandomClass(0);
#endif
#if defined(_OPENMP)
      #pragma omp for
#endif
			for (int i = 1; i <= GRIDROWS; i++) { //Move, delete and convert
#if defined (_OPENMP)
				lock(i, locks);
#endif
				for (int j = 1; j <= GRIDCOLUMNS; j++) {
					if (gridA[i][j] != NULL) {
						Agent* agent = gridA[i][j];
						gridA[i][j] = NULL;
						double move = random.random();
						agent->step();

						//Code to remove decomposed agents
						if ((agent->getType() == zombie) && (agent->isDecomposed() || agent->isShooted())) {
							//Delete?
							Counters::getInstance().newZombieDead();
							agent = NULL;
						} else {
							if ( agent->getType() == human && agent->isNaturalDead() ) {
								//Delete?
								Counters::getInstance().newHumanDead();
								agent = NULL;
							} else {
								if ( agent->getType() == human && agent->isInfected() ) {
									Counters::getInstance().newConversion();
									agent = new Agent(random.random(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME), zombie);
								}
							}
						}

						//Agent already dead no comparisons required
						if (agent != NULL) {

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

							if (gridA[desti][destj] == NULL && gridB[desti][destj] == NULL) {
								gridB[desti][destj] = agent;
								//#ifdef DEBUGGRID
								//std::cerr << "Tick: " << (n+1) << "Human moved from" << i <<","<< j << " to "<< desti <<","<< destj<<"\n";
								//#endif

							} else {
								gridB[i][j] = agent;
							}

						}
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
			if (gridB[i][0] != NULL && gridB[i][1] == NULL) {
				Agent* a = gridB[i][0];
				gridB[i][1] = a;
				gridB[i][0] = NULL;
			} else if (gridB[i][0] != NULL && gridB[i][1] != NULL) {
				gridB[i][0] = NULL;
				Counters::getInstance().newGhostCase();
			}
			if (gridB[i][GRIDCOLUMNS + 1] != NULL && gridB[i][GRIDCOLUMNS] == NULL) {
				gridB[i][GRIDCOLUMNS] = gridB[i][GRIDCOLUMNS + 1];
				gridB[i][GRIDCOLUMNS + 1] = NULL;
			} else if (gridB[i][GRIDCOLUMNS + 1] != NULL && gridB[i][GRIDCOLUMNS] != NULL) {
				gridB[i][GRIDCOLUMNS + 1] = NULL;
				Counters::getInstance().newGhostCase();
			}
		}

		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			if (gridB[0][j] != NULL && gridB[1][j] == NULL) {
				gridB[1][j] = gridB[0][j];
				gridB[0][j] = NULL;
			} else if (gridB[0][j] != NULL && gridB[1][j] != NULL) {
				gridB[0][j] = NULL;
				Counters::getInstance().newGhostCase();
			}
			if (gridB[GRIDROWS + 1][j] != NULL && gridB[GRIDROWS][j] == NULL) {
				gridB[GRIDROWS][j] = gridB[GRIDROWS + 1][j];
				gridB[GRIDROWS + 1][j] = NULL;
			} else if (gridB[GRIDROWS + 1][j] != NULL && gridB[GRIDROWS][j] != NULL) {
				gridB[GRIDROWS + 1][j] = NULL;
				Counters::getInstance().newGhostCase();
			}
		}

		totalHumans = totalZombies = freeCells = 0;
		calculateStatistics(totalHumans,totalZombies, freeCells, gridB);
		deathRate = DEATHRATENT/totalHumans;
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
				Agent* agentA = gridB[i][j];
				if (agentA != NULL) {
					AgentTypeEnum typeA = agentA->getType();
					if (typeA == human) {
						if (i > 2 && gridB[i - 1][j] != NULL) { //Someone up
							if (gridB[i - 1][j]->getType() == zombie)
								resolveGridHumanZombie(agentA,i - 1,j, gridB, random);
							else
								resolveGridHumanHuman(agentA,i - 1,j, gridB, random, totalHumans, totalZombies, freeCells);
						}
						if (j < GRIDROWS && gridB[i][j + 1] != NULL) { //Someone on the right
							if (gridB[i][j+1]->getType() == zombie)
								resolveGridHumanZombie(agentA,i,j + 1, gridB, random);
							else
								resolveGridHumanHuman(agentA,i,j+1, gridB, random, totalHumans, totalZombies, freeCells);
						}
						if (i < GRIDROWS && gridB[i + 1][j] != NULL) { //Down
							if (gridB[i+1][j]->getType() == zombie)
								resolveGridHumanZombie(agentA,i + 1,j, gridB, random);
							else
								resolveGridHumanHuman(agentA,i + 1,j, gridB, random, totalHumans, totalZombies, freeCells);
						}
						if (j > 2 && gridB[i][j - 1] != NULL) { //Left
							if (gridB[i][j-1]->getType() == zombie)
								resolveGridHumanZombie(agentA,i,j - 1, gridB, random);
							else
								resolveGridHumanHuman(agentA,i,j - 1, gridB, random, totalHumans, totalZombies, freeCells);
						}
						//Death process
						if (!agentA->isInfected()){

							double move = random.random();
							//cout << tid << "TS:" << n << "Death " <<move << "," << deathRate << endl;
							if (move < deathRate) {
								if (n==366)
								    cout << tid << "TS:" << n << "Death " <<move << "," << deathRate << endl;
								agentA->markAsDead();
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

		swap(gridA, gridB);

		if ( n%NUMTICKSPRINT == 0 ){
			printState(n + 1, gridA);
			//printMatrix(n + 1);
			Counters::getInstance().resetCounters();
		}
	}
	printState(NUMTICKS + 1, gridA);
#if defined(_OPENMP)
	wtime	= omp_get_wtime() - wtime;	// Record the end time and calculate elapsed time
	cout << "Simulation took " << wtime/NUMTICKS << " seconds per time step with " << N_Threads << " threads" << endl;
#endif
}

/* Print Grid*/
void Grid::printMatrix(int tick, Agent*** gridA) {
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
			Agent* agent = gridA[i][j];
			if (agent != NULL) {
				AgentTypeEnum currentType = agent->getType();
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
void Grid::calculateStatistics(float &humanPopulation,float &zombies, float &freeCells, Agent*** gridA) {
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			Agent* agent = gridA[i][j];
			if (agent != NULL && agent->getType() == human)
				humanPopulation += 1.0;
			else if (agent != NULL && agent->getType() == zombie)
				zombies += 1.0;
			else
				freeCells += 1.0;
		}
	}
}

/*Prints the state of the gridA in any time step*/
void Grid::printState(int tick, Agent*** gridA) {
	int humans = 0;
	int zombies = 0;
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
#ifdef DEBUG
			//			std::cout << "printState inspect Agent" << i << "," << j << "\n";
#endif
			Agent* agent = gridA[i][j];
			if (agent != NULL) {
				AgentTypeEnum currentType = agent->getType();
				if (currentType == human)
					humans++;
				else
					zombies++;
			}
		}
	}
#ifdef DEBUG
	std::cout << "Tick" << tick << " Humans: " << humans << " Zombies " << zombies;
	std::cout << "- Infected:" << Counters::getInstance().getInfected();
	std::cout << " Converted: " << Counters::getInstance().getConverted() << " Shot: " << Counters::getInstance().getShooted();
	std::cout << " Zdead: " << Counters::getInstance().getZDead() << " Ghost cases: " << Counters::getInstance().getGhostCase();
	std::cout << " Natural deaths: " << Counters::getInstance().getHumanDead() << " Born: " << Counters::getInstance().getBorn() << "\n";
#endif
}

/*Resolve conflicts between Human-Zombie*/
void Grid::resolveGridHumanZombie(Agent* agentA,int i, int j, Agent*** gridB, RandomClass random) {
	Agent* agentB = gridB[i][j];
	AgentTypeEnum typeB = agentB->getType();
	if (typeB == zombie) {
		resolveHumanZombie(agentA, agentB, random);
	}

}

/*Resolve conflicts between Human-Zombie*/
void Grid::resolveHumanZombie(Agent* human, Agent* zombie, RandomClass random) {
	//Probability and cases, for now it is a rand
	int dice_roll = random.random(0,100);

	if (dice_roll <= HEADSHOTPERCENTAGE) {
		zombie->shoot();
		Counters::getInstance().newShooted();
	} else if (dice_roll <= SUCESSFULBITEPERCENTAGE) {
		human->infect();
		Counters::getInstance().newInfected();
	}
}

/*Resolve conflicts between Human-Human*/
void Grid::resolveGridHumanHuman(Agent* agentA,int i, int j, Agent*** gridB, RandomClass random, float totalHumans, float totalZombies, float freeCells) {
	Agent* agentB = gridB[i][j];
	AgentTypeEnum typeB = agentB->getType();
	if (typeB == human) {
		//If humans have opposite genders, then try the chances to have a baby
		if ((agentA->getGender() && !agentB->getGender()) || (!agentA->getGender() && agentB->getGender())){

			//Only Humans between 25 and 45 years can have a baby
			if (agentA->getAge()>=14 && agentA->getAge()<=50 && agentB->getAge()>=14 && agentB->getAge()<=50){

				//1. Calculate statistics
				float delta = 0,probPaired=0,probPair=0,probAnyHumanHaveBaby=0;


				//2. Calculate probabilities
				delta = totalHumans / (TOTALGRIDCELLS - totalZombies); //delta is the density of humans
				probPaired = 0.005; //IRTHSPERDAYNT/21250.0; //Prob. of birth when paired -consider Ages!!!
				probPair = 1-pow((1-delta),4); //Prob. two humans being pair 1-(1-delta)^4
				probAnyHumanHaveBaby = probPair * probPaired; //Prob. any Human will have a baby (Per capita birth rate)

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
					if (!foundIt && i  > 2 && gridB[i - 1][j] == NULL) { //Someone up
						freeI=i-1,freeJ=j;
						foundIt = true;
					}
					if (!foundIt && j < GRIDROWS && gridB[i][j + 1] == NULL) { //Someone on the right
						freeI=i,freeJ=j+1;
						foundIt = true;
					}
					if (!foundIt && i < GRIDROWS && gridB[i + 1][j] == NULL) { //Down
						freeI=i+1,freeJ=j;
						foundIt = true;
					}
					if (!foundIt && j > 2 && gridB[i][j - 1] == NULL) { //Left
						freeI=i,freeJ=j-1;
						foundIt = true;
					}
					if (!foundIt && i > 4 && gridB[i - 2][j] == NULL) { //Someone up up
						freeI=i-2,freeJ=j;
						foundIt = true;
					}
					if (!foundIt && j < GRIDROWS - 2 && gridB[i][j + 2] == NULL) { //Someone on the right right
						freeI=i,freeJ=j+2;
						foundIt = true;
					}
					if (!foundIt && i < GRIDROWS -2  && gridB[i + 2][j] == NULL) { //Down Down
						freeI=i+2,freeJ=j;
						foundIt = true;
					}
					if (!foundIt && j > 4 && gridB[i][j - 2] == NULL) { //Left Left
						freeI=i,freeJ=j-2;
						foundIt = true;
					}

					//If we found a free space to the new born, place it, otherwise kill him! :(
					if (freeI!=-1 && freeJ!=-1 && foundIt ){
						gridB[freeI][freeJ] = new Agent(gender, 1, hasAGun, lifeExpectancy, human);
						Counters::getInstance().newBorn();
					}
				}
			}
		}
	}
}


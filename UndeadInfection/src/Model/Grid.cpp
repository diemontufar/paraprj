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
#endif

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
void Grid::calculatePopulationAndFreeCells(float &population, float &freecells, Agent*** gridA){
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			Agent* agent = gridA[i][j];
			if (agent != NULL && agent->getType()==human)
					population+=1.0;
			else
				freecells+=1.0;
		}
	}
}
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
	std::cout << "- Deads: " << Counters::getInstance().getDead() << " Infected:" << Counters::getInstance().getInfected();
	std::cout << " Converted: " << Counters::getInstance().getConverted() << " Shooted: " << Counters::getInstance().getShooted();
	std::cout << " Zdead: " << Counters::getInstance().getZDead() << " Ghost cases: " << Counters::getInstance().getGhostCase();
	std::cout << " Natural deaths: " << Counters::getInstance().getHumanDead() << " Born: " << Counters::getInstance().getBorn() << "\n";
#endif
}
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

	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridB[i][j] = NULL;
			//todo:change algorithm so it is random and with pop limits
			if (numHumans < DARWINPOPDENSITY && Random::randomBoolFalseBiasedN()) {

				int age = Random::random(MINLIFEEXPECTANCY - 1);
				bool gender = Random::random() < GENDERRATIO ? true : false;
				bool hasAGun = Random::random() < GUNDENSITY ? true : false;

				gridA[i][j] = new Agent(gender, age, hasAGun, human);
				numHumans++;
			} else {
				if (numZombies < NUMBEROFZOMBIES && Random::randomBoolFalseBiasedZN()) {
					gridA[i][j] = new Agent(zombie);
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

void Grid::merge(Agent*** gridA, Agent*** gridB) {
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridA[i][j] = gridB[i][j];
		}
	}
}
void Grid::run() {
	Agent*** gridA = createMesh();
	Agent*** gridB = createMesh();
	bool *locks = new bool[GRIDCOLUMNS + 2];
	initialize(gridA, gridB);

    #if defined(_OPENMP)
	double		wtime		= omp_get_wtime();			// Record the starting time
	int			N_Threads	= omp_get_max_threads();
    #endif

	for (int i = 0; i < GRIDCOLUMNS + 2; i++) locks[i] = false;

	printState(0, gridA);
	//printMatrix(0);
#ifdef DEBUGGRID
	std::cout << "Run called" << "\n";
#endif
    //Time loop
	for (int n = 0; n < NUMTICKS; n++) {
		Random::resetSeed();
        #if defined(_OPENMP)
        #pragma omp parallel for default(none) shared (locks, gridA, gridB)
        #endif
		for (int i = 1; i <= GRIDROWS; i++) {
			#if defined (_OPENMP)
			lock(i, locks);
			#endif
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				if (gridA[i][j] != NULL) {
					Agent* agent = gridA[i][j];
					gridA[i][j] = NULL;
					double move = Random::random();
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
								agent = new Agent(zombie);
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

		//Resolveb
		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				Agent* agentA = gridB[i][j];
				if (agentA != NULL) {

					AgentTypeEnum typeA = agentA->getType();
					if (typeA == human) {
						if (i > 2 && gridB[i - 1][j] != NULL) { //Someone up
							resolveGridHumanZombie(agentA,i - 1,j, gridB);
						}
						if (j < GRIDROWS && gridB[i][j + 1] != NULL) { //Someone on the right
							resolveGridHumanZombie(agentA,i,j + 1, gridB);
						}
						if (i < GRIDROWS && gridB[i + 1][j] != NULL) { //Down

							resolveGridHumanZombie(agentA,i + 1,j, gridB);
						}
						if (j > 2 && gridB[i][j - 1] != NULL) { //Left
							resolveGridHumanZombie(agentA,i,j - 1, gridB);
						}
					}
				}
			}
		}

		swap(gridA, gridB);

		//Apply new births
		float pop=0;
		float freeCells=0;
		calculatePopulationAndFreeCells(pop, freeCells, gridA);
		//Number of humans that must be given to birth in a given tick
		double prob = BIRTHSPERDAYNT;
		prob = prob/freeCells;

		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				if ( gridA[i][j] == NULL ){ //Empty cell
					//Roll a dice
					double move = Random::random();

					if ( move <= prob ){
						//Its a baby!
						//cout<< "Random: "<< move <<"Chance: "<<prob <<endl;
						bool gender = Random::random() < GENDERRATIO ? true : false;
						bool hasAGun = Random::random() < GUNDENSITY ? true : false;
						gridA[i][j] = new Agent(gender, 1, hasAGun, human);
						Counters::getInstance().newBorn();
					}
				}
			}
		}



		if ( n%50 == 0 ){
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
void Grid::resolveGridHumanZombie(Agent* agentA,int i, int j, Agent*** gridB) {
	Agent* agentB = gridB[i][j];
	AgentTypeEnum typeB = agentB->getType();
	if (typeB == zombie) {
		resolveHumanZombie(agentA, agentB);
	}

}
void Grid::resolveHumanZombie(Agent* human, Agent* zombie) {
	//Probability and cases, for now it is a rand
	int dice_roll = Random::random(0,100);

	if (dice_roll <= HEADSHOTPERCENTAGE) {
		zombie->shoot();
		Counters::getInstance().newShooted();
	} else if (dice_roll <= SUCESSFULBITEPERCENTAGE) {
		human->infect();
		Counters::getInstance().newInfected();
	}
}




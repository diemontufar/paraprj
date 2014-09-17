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

void Grid::printMatrix(int tick) {
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
			if (agent != nullptr) {
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
void Grid::calculatePopulationAndFreeCells(float &population, float &freecells){
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			Agent* agent = gridA[i][j];
			if (agent != nullptr && agent->getType()==human)
					population+=1.0;
			else
				freecells+=1.0;
		}
	}
}
void Grid::printState(int tick) {
	int humans = 0;
	int zombies = 0;
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			Agent* agent = gridA[i][j];
#ifdef DEBUGGRID
			//std::cout << "printState inspect Agent" << i << "," << j << "\n";
#endif
			if (agent != nullptr) {
#ifdef DEBUGGRID
				//std::cout << "printState Agent" << agent << "\n";
#endif

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

void Grid::initialize(int nPeople, int nZombies) {
	int numZombies = 0;
	int numHumans = 0;

	gridA = new Agent**[GRIDROWS+2];
	gridB = new Agent**[GRIDROWS+2];
	for (int i = 1; i <= GRIDROWS; i++) {
		gridA[i] = new Agent*[GRIDCOLUMNS+2];
		gridB[i] = new Agent*[GRIDCOLUMNS+2];

		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridB[i][j] = nullptr;
			//todo:change algorithm so it is random and with pop limits
			if (numHumans < DARWINPOPDENSITY && Random::randomBoolTrueBiased()) {

				int age = Random::random(MINLIFEEXPECTANCY - 1);
				bool gender = Random::random() < GENDERRATIO ? true : false;
				bool hasAGun = Random::random() < GUNDENSITY ? true : false;

				gridA[i][j] = new Human(gender, age, hasAGun);
				numHumans++;
			} else {
				if (numZombies < NUMBEROFZOMBIES && Random::randomBool()) {
					gridA[i][j] = new Zombie();
					numZombies++;
				} else {
					gridA[i][j] = nullptr;
				}
			}
			//cout << "i:  "<<i << "j:  "<<j <<"numInitialized with humans: "<< endl;
		}
	}
	//Creating and cleaning halo
	gridA[0] = new Agent*[GRIDCOLUMNS+2];
	gridB[0] = new Agent*[GRIDCOLUMNS+2];
	gridA[GRIDROWS+1] = new Agent*[GRIDCOLUMNS+2];
	gridB[GRIDROWS+1] = new Agent*[GRIDCOLUMNS+2];

	for (int i = 0; i <= GRIDROWS + 1; i++) {
		gridA[i][0] = nullptr;
		gridB[i][0] = nullptr;
		gridA[i][GRIDCOLUMNS + 1] = nullptr;
		gridB[i][GRIDCOLUMNS + 1] = nullptr;
	}
	for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
		gridA[0][j] = nullptr;
		gridB[0][j] = nullptr;
		gridA[GRIDROWS + 1][j] = nullptr;
		gridB[GRIDROWS + 1][j] = nullptr;
	}
	cout << "Initialized with humans: " << numHumans << "- Zombies: " << numZombies << endl;
}

void Grid::merge() {
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridA[i][j] = gridB[i][j];
		}
	}
}
void Grid::run() {
	bool *locks = new bool[GRIDCOLUMNS + 2];
    #if defined(_OPENMP)
	double		wtime		= omp_get_wtime();			// Record the starting time
	int			N_Threads	= omp_get_max_threads();
    #endif

	for (int i = 0; i < GRIDCOLUMNS + 2; i++) locks[i] = false;
	printState(0);
	//printMatrix(0);
#ifdef DEBUGGRID
	std::cout << "Run called" << "\n";
#endif
    //Time loop
	for (int n = 0; n < NUMTICKS; n++) {
        #if defined(_OPENMP)
        #pragma omp parallel for default(none) shared (locks)
        #endif
		for (int i = 1; i <= GRIDROWS; i++) {
			#if defined (_OPENMP)
			lock(i, locks);
			#endif
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				if (gridA[i][j] != nullptr) {
					Agent* agent = gridA[i][j];
					gridA[i][j] = nullptr;
					double move = Random::random();
					agent->step();

					//Code to remove decomposed agents
					if ((agent->getType() == zombie) && (dynamic_cast<Zombie*>(agent)->isDecomposed() || dynamic_cast<Zombie*>(agent)->isShooted())) {
						//Delete?
						Counters::getInstance().newZombieDead();
						agent = nullptr;
					} else {
						if ((agent->getType() == human) && (dynamic_cast<Human*>(agent)->isNaturalDead())) {
							//Delete?
							Counters::getInstance().newHumanDead();
							agent = nullptr;
						} else {
							if ((agent->getType() == human) && (dynamic_cast<Human*>(agent)->isInfected())) {
								Counters::getInstance().newConversion();
								agent = new Zombie;
							}
						}
					}

					//Agent already dead no comparisons required

					if (agent != nullptr) {

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

						if (gridA[desti][destj] == nullptr && gridB[desti][destj] == nullptr) {
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
			if (gridB[i][0] != nullptr && gridB[i][1] == nullptr) {
				Agent* a = gridB[i][0];
				gridB[i][1] = a;
				gridB[i][0] = nullptr;
			} else if (gridB[i][0] != nullptr && gridB[i][1] != nullptr) {
				gridB[i][0] = nullptr;
				Counters::getInstance().newGhostCase();
			}
			if (gridB[i][GRIDCOLUMNS + 1] != nullptr && gridB[i][GRIDCOLUMNS] == nullptr) {
				gridB[i][GRIDCOLUMNS] = gridB[i][GRIDCOLUMNS + 1];
				gridB[i][GRIDCOLUMNS + 1] = nullptr;
			} else if (gridB[i][GRIDCOLUMNS + 1] != nullptr && gridB[i][GRIDCOLUMNS] != nullptr) {
				gridB[i][GRIDCOLUMNS + 1] = nullptr;
				Counters::getInstance().newGhostCase();
			}
		}

		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			if (gridB[0][j] != nullptr && gridB[1][j] == nullptr) {
				gridB[1][j] = gridB[0][j];
				gridB[0][j] = nullptr;
			} else if (gridB[0][j] != nullptr && gridB[1][j] != nullptr) {
				gridB[0][j] = nullptr;
				Counters::getInstance().newGhostCase();
			}
			if (gridB[GRIDROWS + 1][j] != nullptr && gridB[GRIDROWS][j] == nullptr) {
				gridB[GRIDROWS][j] = gridB[GRIDROWS + 1][j];
				gridB[GRIDROWS + 1][j] = nullptr;
			} else if (gridB[GRIDROWS + 1][j] != nullptr && gridB[GRIDROWS][j] != nullptr) {
				gridB[GRIDROWS + 1][j] = nullptr;
				Counters::getInstance().newGhostCase();
			}
		}

		//Resolve
		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				Agent* agentA = gridB[i][j];
				if (agentA != nullptr) {

					AgentTypeEnum typeA = agentA->getType();
					if (typeA == human) {
						if (i > 2 && gridB[i - 1][j] != nullptr) { //Someone up
							resolveGridHumanZombie(agentA,i - 1,j);
						}
						if (j < GRIDROWS && gridB[i][j + 1] != nullptr) { //Someone on the right
							resolveGridHumanZombie(agentA,i,j + 1);
						}
						if (i < GRIDROWS && gridB[i + 1][j] != nullptr) { //Down

							resolveGridHumanZombie(agentA,i + 1,j);
						}
						if (j > 2 && gridB[i][j - 1] != nullptr) { //Left
							resolveGridHumanZombie(agentA,i,j - 1);
						}
					}
				}
			}
		}

		swap(gridA, gridB);

		//Apply new births
		float pop=0;
		float freeCells=0;
		calculatePopulationAndFreeCells(pop, freeCells);
		double prob = BIRTHSPERDAY*pop/AUSPOP;
		prob = prob/freeCells;

		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				if ( gridA[i][j] == nullptr ){ //Empty cell
					//Roll a dice
					double move = Random::random();
					if ( move < prob ){
						//Its a baby!
						bool gender = Random::random() < GENDERRATIO ? true : false;
						bool hasAGun = Random::random() < GUNDENSITY ? true : false;
						gridA[i][j] = new Human(gender, 1, hasAGun);
						Counters::getInstance().newBorn();
					}
				}
			}
		}



		if ( n%50 == 0 ){
			printState(n + 1);
			//printMatrix(n + 1);
			Counters::getInstance().resetCounters();
		}
	}
	printState(NUMTICKS + 1);
#if defined(_OPENMP)
	wtime	= omp_get_wtime() - wtime;	// Record the end time and calculate elapsed time
	cout << "Simulation took " << wtime/NUMTICKS << " seconds per time step with " << N_Threads << " threads" << endl;
#endif
}
void Grid::resolveGridHumanZombie(Agent* agentA,int i, int j) {
	Agent* agentB = gridB[i][j];
	AgentTypeEnum typeB = agentB->getType();
	if (typeB == zombie) {
		resolveHumanZombie(agentA, agentB);
	}

}
void Grid::resolveHumanZombie(Agent* agentHuman, Agent* agentZombie) {
	Human* h = dynamic_cast<Human*>(agentHuman);
	Zombie* z = dynamic_cast<Zombie*>(agentZombie);
	//Probability and cases, for now it is a rand
	int dice_roll = Random::random(0,100);

	if (dice_roll <= HEADSHOTPERCENTAGE) {
		z->shoot();
		Counters::getInstance().newShooted();
	} else if (dice_roll <= SUCESSFULBITEPERCENTAGE) {
		h->infect();
		Counters::getInstance().newInfected();
	}
}




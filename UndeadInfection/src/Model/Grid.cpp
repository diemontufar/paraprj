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
Grid::Grid() {
	randomObj = new RandomGen();
}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}
void Grid::printMatrix(int tick) {
	cout << endl;

	cout << "|";
	for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
		cout << j << (j <= 9 ? " |" : "|");
	}
	cout << endl;
	for (int i = 0; i <= GRIDROWS + 1; i++) {
		for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
			cout << "---";
		}
		cout << endl;
		for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
			if (j == 1)
				cout << "|";
			Agent* agent = gridA[i][j];
			if (agent != nullptr) {
				AgentTypeEnum currentType = agent->getType();
				if (currentType == human)
					cout << "H |";
				else
					cout << "Z |";
			} else {
				cout << "  |";
			}
		}
		cout << endl;
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
	for (int i = 1; i <= GRIDROWS; i++) {
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
				if (numZombies < NUMBEROFZOMBIES && RandomGen::randomBool()) {
					gridA[i][j] = new Zombie();
					numZombies++;
				} else {
					gridA[i][j] = nullptr;
				}
			}
			//cout << "i:  "<<i << "j:  "<<j <<"numInitialized with humans: "<< endl;
		}
	}
	cout << "Initialized with humans: " << numHumans << "- Zombies: " << numZombies << endl;
}

//Todo: check adjacencies and move
void Grid::addAgent(int x, int y, Agent* agent) {
	/*	if (cells[x][y]->getCurrentAgent() == nullptr) {
	 cells[x][y]->setCurrentAgent(agent);
	 cells[x][y]->setCandidateAgent(nullptr);
	 }
	 //	cout << "added and agent at: "<<"x:  "<<x << "y:  "<<y<< endl;*/
}
void Grid::removeAgent(int x, int y, Agent* agent) {
	//cells[x][y]->setCurrentAgent(nullptr);
}
void Grid::merge() {
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridA[i][j] = gridB[i][j];
		}
	}
}
void Grid::run() {
	printState(0);
	//printMatrix(0);
#ifdef DEBUGGRID
	std::cout << "Run called" << "\n";
#endif

	for (int n = 0; n < NUMTICKS; n++) {
		for (int i = 1; i <= GRIDROWS; i++) {
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
								//Counters::getInstance().newHumanDead();
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
		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				if ( gridA[i][j] == nullptr ){ //Empty cell
					//Roll a dice
					double move = Random::random();
					if ( move < BIRTHPERCENTAGE ){
						//Its a baby!
						bool gender = Random::random() < GENDERRATIO ? true : false;
						bool hasAGun = Random::random() < GUNDENSITY ? true : false;
						gridA[i][j] = new Human(gender, 1, hasAGun);
						Counters::getInstance().newBorn();
					}
				}
			}
		}

		//if ( n%100 == 0 ){
		printState(n + 1);
		//printMatrix(n + 1);
		Counters::getInstance().resetCounters();
		//}
	}
	printState(NUMTICKS + 1);
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
	int dice_roll = randomObj->getIntUniformRandomBetween(0, 100);

	if (dice_roll <= HEADSHOTPERCENTAGE) {
		z->shoot();
		Counters::getInstance().newShooted();
	} else if (dice_roll <= SUCESSFULBITEPERCENTAGE) {
		h->infect();
		Counters::getInstance().newInfected();
	}
}
/*void Cell::step(){

 if ( currentAgent != nullptr ) currentAgent->step();
 if ( candidateAgent != nullptr ) candidateAgent->step();
 //Check if someone is dead or shooted
 if ( currentAgent != nullptr && currentAgent->getType() == human && (dynamic_cast<Human*>(currentAgent))->isDead() ){ //Its a zombie!
 Counters::getInstance().newDead();
 Counters::getInstance().newConversion();
 grid->removeAgent(x ,y, currentAgent);
 grid->addAgent(x,y,new Zombie());
 }
 if ( currentAgent != nullptr && currentAgent->getType() == zombie &&
 (dynamic_cast<Zombie*>(currentAgent)->isShooted() || (dynamic_cast<Zombie*>(currentAgent))->isDecomposed()) ){ //Remove zombie
 Counters::getInstance().newZombieDead();
 grid->removeAgent(x ,y, currentAgent);
 }


 if (currentAgent == nullptr && candidateAgent == nullptr)//Empty cell do nothing
 return;
 if (currentAgent != nullptr && candidateAgent == nullptr)//Nothing to do
 return;

 if ( currentAgent != nullptr && candidateAgent != nullptr ){//There is someone in the cell, lets check

 AgentTypeEnum currentType = currentAgent->getType();
 AgentTypeEnum candidateType = candidateAgent->getType();

 if ( currentType == human && candidateType == human ){
 resolveHumanHuman();
 }
 else if (currentType==zombie && candidateType == zombie){
 resolveZombieZombie();
 }
 else{
 resolveHumanZombie();
 }
 }
 else{ //The cell is empty accept the candidate
 currentAgent = candidateAgent;
 candidateAgent = nullptr;
 }
 }
 void Cell::resolveHumanHuman(){
 Human* h1;
 Human* h2;
 h1 = dynamic_cast<Human*>(candidateAgent);
 h2 = dynamic_cast<Human*>(currentAgent);
 if ((h1->getGender() && !h2->getGender()) || (!h1->getGender() && h2->getGender())) { //If they are male and woman
 int result = randomObj->getIntUniformRandomBetween(1,100);
 if ( result < BIRTHPERCENTAGE ){ //They have a baby
 bool gender = ( randomObj->getIntUniformRandomBetween(0,1) ? true:false );
 Human baby(gender,false);
 //grid->addAgent(0,0,&baby);
 }
 }
 //Move human
 grid->moveAgentCurrentToCurrent(x,y,currentAgent,3,3);
 currentAgent = candidateAgent;
 }
 void Cell::resolveHumanZombie(){
 Human* h;
 Zombie* z;
 if ( currentAgent->getType() == zombie ){
 h = dynamic_cast<Human*>(candidateAgent);
 z = dynamic_cast<Zombie*>(currentAgent);
 }
 else{
 h = dynamic_cast<Human*>(currentAgent);
 z = dynamic_cast<Zombie*>(candidateAgent);
 }
 //Probability and cases, for now it is a rand
 int dice_roll = randomObj->getIntUniformRandomBetween(0,100);
 //#ifdef DEBUGCELL
 //std::cout << "Dice roll: " << dice_roll << "\n";
 //#endif

 if ( dice_roll <= HEADSHOTPERCENTAGE ){
 z->shoot();
 Counters::getInstance().newShooted();
 #ifdef DEBUGCELL
 std::cout << "Zombie shooted " <<"\n";
 #endif
 if ( currentAgent->getType() == human ){
 #ifdef DEBUGCELL
 std::cout << "Moving Current Human to another cell " <<"\n";
 #endif
 grid->moveAgentCurrentToCurrent(x,y,currentAgent,5,5);
 #ifdef DEBUGCELL
 std::cout << "Moved" <<"\n";
 #endif
 }
 else{
 #ifdef DEBUGCELL
 std::cout << "Moving Candidate Human to another cell " <<"\n";
 #endif
 grid->moveAgentCandidateToCurrent(x,y,currentAgent,5,5);
 }
 }
 else if ( dice_roll <= SUCESSFULBITEPERCENTAGE ){
 h->infect();
 Counters::getInstance().newInfected();
 #ifdef DEBUGCELL
 std::cout << "Human infected " <<"\n";
 #endif
 if ( currentAgent->getType() == zombie ){
 #ifdef DEBUGCELL
 std::cout << "Moving current zombie to another cell " <<"\n";
 #endif
 grid->moveAgentCurrentToCurrent(x,y,currentAgent,6,6);
 }
 else{
 #ifdef DEBUGCELL
 std::cout << "Moving candidate zombie to another cell " <<"\n";
 #endif
 grid->moveAgentCandidateToCurrent(x,y,currentAgent,7,7);
 }
 }
 }
 void Cell::resolveZombieZombie(){
 //Move zombie to another location
 }
 */

void Grid::setRandomObj(RandomGen* obj) {
	delete (randomObj);
	randomObj = obj;
}


//============================================================================
// Name        : UndeadInfection.cpp
// Author      : Diego
// Version     :
// Copyright   : copyright 2014
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include "Agents/Human.h"
#include "Agents/Zombie.h"
#include "Agents/Agent.h"
#include "Model/Cell.h"
#include "Model/Model.h"
#include "Model/Grid.h"
#include "Model/RandomGen.h"
using namespace std;
bool testHuman();
bool testZombie();
bool testInheritanceOfAgents();
bool testZombieDecomposition();
bool testHumanDeseaseIncubation();
bool testHumanHumanClash();
bool testHumanZombieClashZombieShooted();
bool testHumanZombieClashHumanInfected();
bool testHumanZombieClashWithGrid();
bool testCell();
void runSimulation();
void evalResult(bool result);
//Inject mock grid
//Inject mock random
class MockRandom: public RandomGen {
public:
	int returnValue;
	MockRandom(int value) {
		returnValue = value;
	}
	void setReturnValue(int value) {
		returnValue = value;
	}
	int getIntUniformRandomBetween(int start, int end) override {
		return returnValue;
	}
};
class MockGrid: public Grid {

public:
	Cell cells[3][3];

	MockGrid() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cells[i][j].setGrid(this);
			}
		}

	}

	void setAgent(int x, int y, Agent* a) {
		cells[x][y].setCandidateAgent(a);
	}

	void moveAgent(int x, int y, Agent* a) {

	}

	Cell* getNeighbors(int x, int y) {
		/*Cell* returnCells = new Cell[8];
		 returnCells[0] = cells[0][0];
		 returnCells[1] = cells[0][1];
		 returnCells[2] = cells[0][2];
		 returnCells[3] = cells[1][0];
		 returnCells[4] = cells[1][2];
		 returnCells[5] = cells[2][0];
		 returnCells[6] = cells[2][1];
		 returnCells[7] = cells[2][2];
		 return returnCells;*/
		return NULL;
	}
	void moveAgentToCell(int x, int y, Agent* agent) {

	}
	void addAgent(Agent* agent) {

	}
};

int main() {
	evalResult(testHuman());
	evalResult(testZombie());
	evalResult(testInheritanceOfAgents());
	evalResult(testZombieDecomposition());
	evalResult(testHumanDeseaseIncubation());
	evalResult(testCell());
	evalResult(testHumanHumanClash());
//	evalResult(testHumanZombieClashZombieShooted());
	//evalResult(testHumanZombieClashHumanInfected());
	//evalResult(testHumanZombieClashWithGrid());
	runSimulation();
	return 0;
}
void runSimulation(){
	Model *m;
	m = new Model();
	m->setup();
	m->run();
}
void evalResult(bool result) {
	if (result == true)
		cout << "Success" << endl;
	else
		cout << "Fail" << endl;
}
bool testHumanHumanClash() {
	cout << "Testing Human-Human" << endl;
	return true;
}

bool testHumanZombieClashWithGrid() {
	cout << "Testing Human-Zombie with grid" << endl;
	MockGrid mg;
	Human h(true, false);
	Zombie z;
	mg.cells[1][1].setCurrentAgent(&h);
	mg.cells[1][1].setCandidateAgent(&z);
	mg.cells[1][1].step();
	return true;
}
bool testHumanZombieClashHumanInfected() {
	cout << "Testing Human-Zombie clash, human infected" << endl;
	MockRandom rm2(40);
	Cell c;
	Zombie z;
	Human h2(true, false);
	c.setCurrentAgent(&h2);
	c.setCandidateAgent(&z);
	c.setRandomObj(&rm2);
	c.step();
	cout << "Clash resolved infected: " << h2.isInfected() << "Shooteed: "<< z.isShooted() << endl;
	c.step();
	if (!h2.isInfected())
		return false;
	if (c.getCurrentAgent() == NULL
			|| !dynamic_cast<Human*>(c.getCurrentAgent())->getGender())
		return false;
	return true;
}
bool testHumanZombieClashZombieShooted() {
	MockRandom rm(1);
	cout << "Testing Human-Zombie clash, zombie shooted" << endl;
	Cell c;
	Human h1(true, false);
	Zombie z;
	c.setRandomObj(&rm);
	c.setCurrentAgent(&h1);
	c.setCandidateAgent(&z);
	c.step();
	cout << "Clash resolved infected: " << h1.isInfected() << "Shooteed: "
			<< z.isShooted() << endl;
	if (!z.isShooted())
		return false;
	return true;
}
bool testCell() {
	cout << "Testing cell" << endl;
	Cell cell;
	Agent* humanAgent = new Human(true, false);
	Agent* zombieAgent = new Zombie();

	cell.setCandidateAgent(humanAgent);
	cell.setCurrentAgent(zombieAgent);

	if ((cell.getCandidateAgent())->getType() != human) {
		return false;
	}
	if (cell.getCurrentAgent()->getType() != zombie) {
		return false;
	}
	Human* h = dynamic_cast<Human*>(cell.getCandidateAgent());
	if (h->getGender() != true) {
		return false;
	}
	if ((*h).getGender() != true) {
		return false;
	}

	Zombie* z = dynamic_cast<Zombie*>(cell.getCurrentAgent());
	if (z->isDecomposed()) {
		return false;
	}
	if ((*z).isDecomposed()) {
		return false;
	}

	return true;
}
bool testHumanDeseaseIncubation() {
	cout << "Testing step of human";
	Human h(false, false);
	h.infect();
	for (int i = 0; i <= INCUBATIONTIME + 1; i++) {
		h.step();
	}
	if (!h.isDead()) {
		return false;
	}
	return true;
}
bool testZombieDecomposition() {
	cout << "Testing step of zombies";
	Zombie z;
	for (int i = 0; i <= MINDECOMPOSITIONTIME + 1; i++) {
		z.step();
	}
	if (!z.isDecomposed()) {
		return false;
	}
	return true;
}
bool testInheritanceOfAgents() {
	cout << "Testing inheritance of agents" << endl;
	Agent* a = new Human(true, true);
	if (a->getType() != human)
		return false;
	a = new Zombie();

	if (a->getType() != zombie)
		return false;
	return true;
}
bool testZombie() {
	cout << "Testing zombies" << endl;
	Zombie zombieAgent;
	if (zombieAgent.getType() != zombie) {
		return false;
	}
	if (zombieAgent.isDecomposed()) {
		return false;
	}
	if (zombieAgent.isShooted()) {
		return false;
	}
	zombieAgent.shoot();
	if (!zombieAgent.isShooted()) {
		return false;
	}

	return true;

}
bool testHuman() {
	cout << "Testing humans" << endl;
	Human male(false, false);

	if (male.getGender() != false)
		return false;
	if (male.isHasAGun() != false)
		return false;
	if (male.isInfected() != false)
		return false;

	Human female(true, true);
	if (female.getGender() == false)
		return false;
	if (female.isHasAGun() == false)
		return false;
	if (female.isInfected() != false)
		return false;
	if (female.getType() != human)
		return false;

	return true;
}

/* Main de Diego
 int main() {


 class Agent{

 public:
 int id;
 int type;

 Agent(int idParam = 100, int typeParam = 200)
 :id(idParam),type(typeParam){

 }

 };



 class Human : public Agent{

 public:
 int age;

 Human(int ageParam = 400)
 :age(ageParam){
 }

 };


 Agent **grid = new Agent *[10] ;

 for ( int i = 0; i < 10; i++ ){
 for (int j = 0; j < 10; j++){
 Human h(5);
 h.id = i*j;
 h.type = i+j;
 grid[i] = new Agent[10];
 grid[i][j] = h;

 }
 }


 for ( int i = 0; i < 10; i++ ){
 for (int j = 0; j < 10; j++){

 Human h = static_cast<Human&>(grid[i][j]);

 cout << "Humano en la posicion: " << i << "," << j << "=" << h.id << endl;
 }
 }

 return 0;
 }
 */

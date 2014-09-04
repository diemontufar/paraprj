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
#include "Model/Grid.h"
#include "Model/RandomGen.h"
#include "Model/Random.h"
#include <time.h>

using namespace std;

bool testHuman();
bool testZombie();
bool testInheritanceOfAgents();
bool testZombieDecomposition();
bool testHumanDeseaseIncubation();
bool testHumanHumanClash();
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
};

int main() {
	evalResult(testHuman());
	evalResult(testZombie());
	evalResult(testInheritanceOfAgents());
	evalResult(testZombieDecomposition());
	evalResult(testHumanDeseaseIncubation());
	evalResult(testHumanHumanClash());
//	evalResult(testHumanZombieClashZombieShooted());
	//evalResult(testHumanZombieClashHumanInfected());
	//evalResult(testHumanZombieClashWithGrid());
	runSimulation();
	return 0;
}
void runSimulation(){

	//Necessary to evaluate random values correctly.
	//TODO: consider process id when using MPI
	srand(int(time(NULL)));

	Grid g;
	g.initialize(5,5);
	g.run();
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

bool testHumanDeseaseIncubation() {
	cout << "Testing step of human";
	Human h(false,Random::random(100)+1, false);
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
	Agent* a = new Human(true,Random::random(100)+1, true);
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
	Human male(false,Random::random(100)+1, false);

	if (male.getGender() != false)
		return false;
	if (male.isHasAGun() != false)
		return false;
	if (male.isInfected() != false)
		return false;

	Human female(true,Random::random(100)+1, true);
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

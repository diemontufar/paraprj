//============================================================================
// Name        : UndeadInfection.cpp
// Authors     : Andres Chaves, Gustavo Carrion, Diego Montufar
// Version     :
// Copyright   : copyright 2014
// Description : Zombie Apocalypse Simulation using openMP
// Compile     : Use compile.sh script
//============================================================================

#include <iostream>
#include <stdio.h>
#include "Agents/Agent.h"
#include "Model/Grid.h"
#include "Model/RandomClass.h"
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

int main() {

	/*Testing methods*/

	//evalResult(testHuman());
	//evalResult(testZombie());
	//evalResult(testInheritanceOfAgents());
	//evalResult(testZombieDecomposition());
	//evalResult(testHumanDeseaseIncubation());
	//evalResult(testHumanHumanClash());
	//evalResult(testHumanZombieClashZombieShooted());
	//evalResult(testHumanZombieClashHumanInfected());
	//evalResult(testHumanZombieClashWithGrid());

	/*Start Simulation*/
	runSimulation();
	return 0;
}

void runSimulation(){
	Grid g;
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
	RandomClass random(0);
	Agent h(false,random.random(100)+1, false, 80, human);
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
	Agent z(MINDECOMPOSITIONTIME,zombie);
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
	Agent* a = new Agent(true,1, true, 70, human);
	if (a->getType() != human)
		return false;
	a = new Agent(5,zombie);

	if (a->getType() != zombie)
		return false;
	return true;
}

bool testZombie() {
	cout << "Testing zombies" << endl;
	Agent zombieAgent(5,zombie);
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
	Agent male(false,60, false, 80, human);

	if (male.getGender() != false)
		return false;
	if (male.isHasAGun() != false)
		return false;
	if (male.isInfected() != false)
		return false;

	Agent female(true,100, true, 80, human);
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


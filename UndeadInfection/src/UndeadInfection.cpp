//============================================================================
// Name        : UndeadInfection.cpp
// Author      : Diego
// Version     :
// Copyright   : copyright 2014
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include "Agents/Agent.h"
#include "Model/Grid.h"
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

int main() {
	evalResult(testHuman());
	evalResult(testZombie());
	evalResult(testInheritanceOfAgents());
	evalResult(testZombieDecomposition());
	evalResult(testHumanDeseaseIncubation());
	evalResult(testHumanHumanClash());
	//evalResult(testHumanZombieClashZombieShooted());
	//evalResult(testHumanZombieClashHumanInfected());
	//evalResult(testHumanZombieClashWithGrid());
	//we reinitialize the seed
	Random::resetSeed();
	runSimulation();
	return 0;
}

void runSimulation(){

	//Necessary to evaluate random values correctly.
	//TODO: consider process id when using MPI
	srand(int(time(NULL)));

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
	Agent h(false,Random::random(100)+1, false, human);
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
	Agent z(zombie);
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
	Agent* a = new Agent(true,Random::random(100)+1, true, human);
	if (a->getType() != human)
		return false;
	a = new Agent(zombie);

	if (a->getType() != zombie)
		return false;
	return true;
}

bool testZombie() {
	cout << "Testing zombies" << endl;
	Agent zombieAgent(zombie);
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
	Agent male(false,Random::random(100)+1, false, human);

	if (male.getGender() != false)
		return false;
	if (male.isHasAGun() != false)
		return false;
	if (male.isInfected() != false)
		return false;

	Agent female(true,Random::random(100)+1, true, human);
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


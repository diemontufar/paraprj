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
//#include <unordered_map>
#include "Model/Grid.h"
#include "Model/GridMPI.h"
#include "Model/GridHybrid.h"
#include "Model/RandomClass.h"
#include <time.h>
#include <omp.h>

using namespace std;

bool testHuman();
bool testZombie();
bool testInheritanceOfAgents();
bool testZombieDecomposition();
bool testHumanDeseaseIncubation();
bool testHumanHumanClash();
void runSimulation(int argc, char** argv);
void evalResult(bool result);

int main(int argc, char** argv) {

	/*Testing methods*/

	evalResult(testHuman());
	evalResult(testZombie());
	evalResult(testInheritanceOfAgents());
	evalResult(testZombieDecomposition());
	evalResult(testHumanDeseaseIncubation());
	//evalResult(testHumanHumanClash());
	//evalResult(testHumanZombieClashZombieShooted());
	//evalResult(testHumanZombieClashHumanInfected());
	//evalResult(testHumanZombieClashWithGrid());

	/*Start Simulation*/
	runSimulation(argc, argv);
	return 0;
}

void runSimulation(int argc, char** argv){
	//Grid g;
	//g.run();
	GridMPI g;
	g.run(argc, argv);
	//GridHybrid g;
	//g.run(argc, argv);
/*	RandomClass random;
	random.setSeed(0xFFFF*drand48());
	double min=1, max=0;
	int size = 10000;
	float sizef = 10000.0;
	int dist[size];
	for (int i = 0; i < size; i++)
		dist[i] = 0;

	for (int i = 0; i < 4000000; i++){
		double rand = random.random();

		if (rand < min)
			min = rand;
		if (rand > max)
			max = rand;

		for ( int j = 0; j < size;j++){
			if (rand >= j/sizef && rand < (j+1)/sizef){
				dist[j]++;
				j = size;
			}
		}

	}
	for ( int j = 0; j < size;j++){
		cout<< ">" << j/sizef << " <" << (j+1)/sizef << ":" << dist[j] << endl;
	}
	cout<< "MIN: " << min <<" - MAX:" << max << endl;*/

	/*std::unordered_map<double, int> map;
	for (int i = 0; i < 40000000; i++){
		double rand = random.random();
		std::unordered_map<double,int>::const_iterator got = map.find (rand);

		  if ( got == map.end() )
			  map[rand]++;
		  else
			  map[rand] = 1;
	}
	cout << "Repetidos: "<<endl;
	for ( auto it = map.begin(); it != map.end(); ++it ){
		if (it->second > 1){
			std::cout << " " << it->first << ":" << it->second << endl;
		}
	}*/
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
	Agent h;
	h.migrateToHuman(false,random.random(100)+1, false, 80,0);
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
	Agent z;
	z.migrateToZombie(MINDECOMPOSITIONTIME);
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
	Agent a;
	a.migrateToHuman(true,1, true, 70,0);
	if (a.getType() != human)
		return false;
	a.migrateToZombie(5);
	if (a.getType() != zombie)
		return false;
	return true;
}

bool testZombie() {
	cout << "Testing zombies" << endl;
	Agent zombieAgent;
	zombieAgent.migrateToZombie(5);
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
	Agent male;
	male.migrateToHuman(false,60, false, 80,0);

	if (male.getGender() != false)
		return false;
	if (male.isHasAGun() != false)
		return false;
	if (male.isInfected() != false)
		return false;

	Agent female;
	female.migrateToHuman(true,100, true, 80,0);
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


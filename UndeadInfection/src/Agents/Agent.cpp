/*
 * Agent.cpp
 *
 *  Created on: 21/08/2014
 *      Authors: Andres Chaves, Gustavo Carrion, Diego Montufar
 */

#include "Agent.h"


Agent::Agent(){
	type = none;
}
void Agent::migrateToZombie(int theLifeTime){
	type = zombie;
	decompositionTime = 0;
	decomposed = false;
	shooted = false;
	lifeTime = theLifeTime;
	numBabies = 0;
	//lifeTime=Random::random(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME);
}
void Agent::migrateToHuman(bool theGender,int theAge, bool isHasAGun, int theLifeExpectancy, int theNumBabies){
	type = human;
	infected = false;
	gender = theGender;
	age = theAge;
	hasAGun = isHasAGun;
	infectionTime = 0;
	yearTime = 0;
	deadByConversion = false;
	naturalDead = false;
	lifeExpectancy = theLifeExpectancy;
	numBabies = theNumBabies;
	//lifeExpectancy=Random::random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY);
}
void Agent::clean(){
	type = none;
	decompositionTime = 0;
	decomposed = false;
	shooted = false;
	lifeTime = 0;
	infected = false;
	gender = false;
	age = 0;
	hasAGun = false;
	infectionTime = 0;
	yearTime = 0;
	deadByConversion = false;
	naturalDead = false;
	lifeExpectancy = 0;
	numBabies = 0;
}
void Agent::clone(Agent b){
	type = b.getType();
	decompositionTime = b.decompositionTime;
	decomposed = b.decomposed;
	shooted = b.shooted;
	lifeTime = b.lifeTime;
	infected = b.infected;
	gender = b.gender;
	age = b.age;
	hasAGun = b.hasAGun;
	infectionTime = b.infectionTime;
	yearTime = b.yearTime;
	deadByConversion = b.deadByConversion;
	naturalDead = b.naturalDead;
	lifeExpectancy = b.lifeExpectancy;
	numBabies = b.numBabies;
}
Agent::~Agent() {
}
AgentTypeEnum Agent::getType(){
//	std::cout << "El tipoooo es :"<<type<<"\n";
	return type;
}

//Human methods
void Agent::step() {
	if (type == human){
		//#ifdef DEBUG
		//std::cout << "Step called"<<"\n";
		//#endif
		if (infected) {
//#ifdef DEBUG
//   std::cout << "+1!"<<"\n";
//#endif

			infectionTime++;
			if (infectionTime >= INCUBATIONTIME) {
//				#ifdef DEBUG
//				   std::cout << "Zombiefication!"<<"\n";
//				#endif
				deadByConversion = true;
			}
		}
		yearTime++;
		if (yearTime == YEARTICKS) {
			age++;
			yearTime = 0;
		}
	}
	else if (type==zombie){
		decompositionTime++;
		if (decompositionTime >= lifeTime){
			 //#ifdef DEBUG
			 //   std::cout << "Death dead" << "\n";
			 //#endif
			decomposed = true;
		}
	}
}

void Agent::markAsDead(){
	assert(type==human);
	naturalDead = true;
}
bool Agent::getGender() {
	assert(type==human);
	return gender;
}

int Agent::getAge() {
	assert(type==human);
	return age;
}

bool Agent::isHasAGun() {
	assert(type==human);
	return hasAGun;

}
bool Agent::isNaturalDead() {
	assert(type==human);
	return naturalDead;

}

bool Agent::isInfected() {
	//std::cout << "El tipuuuu es :"<<type<<"\n";
	assert(type==human);
	return infected;
}
void Agent::infect() {
	//std::cout << "Infected :"<<type<<"\n";
	assert(type==human);
	infected = true;
}

void Agent::incrementNumBabies() {
	//std::cout << "Num babies :"<<numBabies<<"\n";
	assert(type==human);
	numBabies++;
}

int Agent::getNumBabies() {
	assert(type==human);
	return numBabies;
}

bool Agent::isDead() {
	assert(type==human);
	return deadByConversion;
}
bool Agent::isDecomposed(){
	//std::cout << "El tipo es :"<<type<<"\n";
	assert(type==zombie);
	return decomposed;
}
bool Agent::isShooted(){
	assert(type==zombie);
	return shooted;
}
void Agent::shoot(){
	assert(type==zombie);
	shooted = true;
}






/*
 * Agent.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Agent.h"
Agent::Agent(AgentTypeEnum theType){
	assert (theType!=human);
	type = theType;
	decompositionTime = 0;
	decomposed = false;
	shooted = false;
	lifeTime=Random::random(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME);
}
Agent::Agent(bool theGender,int theAge, bool isHasAGun, AgentTypeEnum theType){
	assert (theType!=zombie);
	type = theType;
	infected = false;
	gender = theGender;
	age = theAge;
	hasAGun = isHasAGun;
	type = theType;
	infectionTime = 0;
	yearTime = 0;
	deadByConversion = false;
	naturalDead = false;
	lifeExpectancy=Random::random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY);
}
Agent::~Agent() {
}
AgentTypeEnum Agent::getType(){
	return type;
}

//Human methods
void Agent::step() {
	if (type == human){
		//#ifdef DEBUG
		//std::cout << "Step called"<<"\n";
		//#endif
		if (infected) {
			infectionTime++;
			if (infectionTime >= INCUBATIONTIME) {
				//#ifdef DEBUG
				//   std::cout << "Dead"<<"\n";
				//#endif
				deadByConversion = true;
			}
		}
		yearTime++;
		if (yearTime == YEARTICKS) {
			age++;
			yearTime = 0;
		}
		double move = Random::random();
		if (move < DEATHRATEAU) {
			//if (move < DEATHRATE) {
			//if (age>lifeExpectancy){
			naturalDead = true;
		}
	}
	else if (type==zombie){
		decompositionTime++;
		if (decompositionTime >= lifeTime){
			// #ifdef DEBUG
			//    std::cout << "Dead" << "\n";
			// #endif
			decomposed = true;
		}
	}
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
	assert(type==human);
	return infected;
}
void Agent::infect() {
	assert(type==human);
	infected = true;
}
bool Agent::isDead() {
	assert(type==human);
	return deadByConversion;
}
bool Agent::isDecomposed(){
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






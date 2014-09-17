/*
 * Human.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Human.h"
#ifdef DEBUG
#include "iostream"
#endif

Human::Human(bool theGender, int theAge, bool ifHasAGun) :
		gender(theGender), age(theAge), hasAGun(ifHasAGun), infected(false), infectionTime(0), yearTime(0), deadByConversion(false), naturalDead(false), lifeExpectancy(
				Random::random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY)) {

}

void Human::step() {
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
	if (move < DEATHRATE) {
		//if (age>lifeExpectancy){
		naturalDead = true;
	}
}

bool Human::getGender() {
	return gender;
}

int Human::getAge() {
	return age;
}

bool Human::isHasAGun() {
	return hasAGun;

}
bool Human::isNaturalDead() {
	return naturalDead;

}

bool Human::isInfected() {
	return infected;
}
void Human::infect() {
	infected = true;
}
bool Human::isDead() {
	return deadByConversion;
}
AgentTypeEnum Human::getType() {
	return human;
}

Human::~Human() {
}


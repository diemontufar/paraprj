/*
 * Human.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Human.h"
#include "Agent.h"
#ifdef DEBUG
#include "iostream"
#endif

Human::Human(bool theGender, bool ifHasAGun):gender(theGender),hasAGun(ifHasAGun),infected(false) {

}

void Agent::step(){
   #ifdef DEBUG
   cout << "Step called";
   #endif
}

bool Human::getGender(){
	return gender;
}

bool Human::isHasAGun(){
	return hasAGun;

}
bool Human::isInfected(){
	return infected;
}
AgentTypeEnum Agent::getType(){
	return human;
}

Human::~Human() {
}


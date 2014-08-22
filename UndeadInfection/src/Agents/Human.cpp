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

Human::Human(bool theGender, bool ifHasAGun):gender(theGender),hasAGun(ifHasAGun),infected(false) {

}

void Human::step(){
   #ifdef DEBUG
   std::cout << "Step called"<<"\n";
   #endif
   if( infected ){
	   infectionTime++;
	   if ( infectionTime >= INCUBATIONTIME ){
           #ifdef DEBUG
              std::cout << "Dead"<<"\n";
           #endif
		   dead = true;
	   }
   }
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
void Human::infect(){
	infected = true;
}
bool Human::isDead(){
	return dead;
}
AgentTypeEnum Human::getType(){
	return human;
}

Human::~Human() {
}


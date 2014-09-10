/*
 * Zombie.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Zombie.h"

#ifdef DEBUG
#include "iostream"
#endif

Zombie::Zombie() {
	decompositionTime = 0;
	decomposed = false;
	shooted = false;
	lifeTime=RandomGen::getIntUniformRandomBetween(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME);
}

void Zombie::step(){
//   #ifdef DEBUG
//   std::cout << "Step called" << "\n";
//   #endif
   decompositionTime++;
   if (decompositionTime >= lifeTime){
     // #ifdef DEBUG
     //    std::cout << "Dead" << "\n";
     // #endif
      decomposed = true;
   }
}
bool Zombie::isDecomposed(){
	return decomposed;
}
bool Zombie::isShooted(){
	return shooted;
}
void Zombie::shoot(){
	shooted = true;
}

AgentTypeEnum Zombie::getType(){
	return zombie;
}

Zombie::~Zombie() {
}


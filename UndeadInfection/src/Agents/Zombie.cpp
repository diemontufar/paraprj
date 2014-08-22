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
}

void Zombie::step(){
   #ifdef DEBUG
   std::cout << "Step called" << "\n";
   #endif
   decompositionTime++;
   if (decompositionTime >= DECOMPOSITIONTIME){
      #ifdef DEBUG
         std::cout << "Dead" << "\n";
      #endif
      decomposed = true;
   }
}
bool Zombie::isDecomposed(){
	return decomposed;
}
AgentTypeEnum Zombie::getType(){
	return zombie;
}

Zombie::~Zombie() {
}


/*
 * RandomGen.cpp
 *
 *  Created on: 24/08/2014
 *      Author: achaves
 */

#include "RandomGen.h"
#include <cstdlib>

RandomGen::RandomGen() {
	// TODO Auto-generated constructor stub

}

RandomGen::~RandomGen() {
	// TODO Auto-generated destructor stub
}
int RandomGen::getIntUniformRandomBetween(int start, int end){
	 // Seed with a real random value, if available
    //std::random_device rd;
	//std::default_random_engine generator(rd());
	//std::uniform_int_distribution<int> distribution(start,end);
	//int dice_roll = distribution(generator);
	return start + (rand() % end);
	//return dice_roll;
}
bool RandomGen::randomBool() {
  return rand() % 2 == 1;
}


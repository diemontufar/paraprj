/*
 * RandomGenerator.cpp
 *
 *  Created on: Aug 30, 2014
 *      Author: GUZ
 */

#include "RandomGenerator.h"

RandomGenerator::RandomGenerator() {
	// TODO Auto-generated constructor stub

}

RandomGenerator::~RandomGenerator() {
	// TODO Auto-generated destructor stub
}

bool RandomGenerator::randomBool() {
  return rand() % 2 == 1;
}


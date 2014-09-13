/*
 * Random.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: diogonal
 */

#include "Random.h"
#include <cstdlib>

Random::Random()
{
    // TODO Auto-generated constructor stub

}

Random::~Random()
{
    // TODO Auto-generated destructor stub
}

/* Generates random number < 1, works for probabilities */
double Random::random() {
	return ((double) rand() / (RAND_MAX));
}

/* Generates random number from 0 to range*/
int Random::random(int range) {
	return int(random() * range);
}

/* Generates values between 0 and 1*/
bool Random::randomBool() {
  return rand() % 2 == 1;
}
bool Random::randomBoolTrueBiased() {
  return rand() % 4 < 3;
}

/*
 * Random.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: diogonal
 */

#include "RandomClass.h"
#include<time.h>
#include <cstdlib>

RandomClass::RandomClass(unsigned int threadNumber){
	seed = threadNumber ;
}


RandomClass::~RandomClass(){}

// Generates random number < 1, works for probabilities
double RandomClass::random() {
	double rng=(double) rand_r(&seed);
	double maxrange=(double)RAND_MAX;
	//if (rng==0)
		//rng=0.000000001;
	double rand01=(rng) / (maxrange);
	return (rand01);
}

// Generates random number from 0 to range
int RandomClass::random(int range) {
	return int(random() * range);
}

// Generates values between start and end
int RandomClass::random(int start, int end){
	return start + (rand_r(&seed) % end);
}

// Generates values between 0 and 1
bool RandomClass::randomBool() {
  return rand_r(&seed) % 2 == 1;
}

bool RandomClass::randomBoolTrueBiased() {
  return rand_r(&seed) % 4 < 3;
}
bool RandomClass::randomBoolFalseBiasedN() {
	return rand_r(&seed) % 10000 > 9000;
	//return rand() % 1000 > 900;
}
bool RandomClass::randomBoolFalseBiasedZN() {
  //return rand() % 10000 > 9990;
  return rand_r(&seed) % 10000 > 9998;
}


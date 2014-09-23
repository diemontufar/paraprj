/*
 * Random.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: diogonal
 */

#include "RandomClass.h"



RandomClass::RandomClass(unsigned int threadNumber){
	seed = (threadNumber+1) * ( static_cast<unsigned int>(time(NULL)) );
	seed = (threadNumber+1) *std::random_device()();
	rng.seed(seed);
}


RandomClass::~RandomClass(){}

// Generates random number < 1, works for probabilities
double RandomClass::random() {
	std::uniform_real_distribution<double> zeroToOne(0.0, 1.0);
	return zeroToOne(rng);
}

// Generates random number from 0 to range
int RandomClass::random(int range) {
	return int(random() * range);
}

// Generates values between start and end
int RandomClass::random(int start, int end){
	std::uniform_int_distribution<int> dist10(start,end);
	return dist10(rng);
}

// Generates values between 0 and 1
bool RandomClass::randomBool() {
  return random(1,2) % 2 == 1;
}

bool RandomClass::randomBoolTrueBiased() {
  return random(1,4) % 4 < 3;
}
bool RandomClass::randomBoolFalseBiasedN() {
	return random(1,10000) % 10000 > 9000;
	//return rand_r(&seed) % 10000 > 9000;
	//return rand() % 1000 > 900;
}
bool RandomClass::randomBoolFalseBiasedZN() {
  //return rand() % 10000 > 9990;
  //return rand_r(&seed) % 10000 > 9998;
	return random(1,10000) % 10000 > 9098;
}

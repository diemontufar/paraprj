/*
 * RandomClass.cpp
 *
 *  Created on: 03/09/2014
 *      Author: Andres Chaves, Gustavo Carrion, Diego Montufar
 */

#include "RandomClass.h"


RandomClass::RandomClass(){
}
RandomClass::RandomClass(unsigned int seed){
	rng.seed(seed);
}


RandomClass::~RandomClass(){}

// Generates random number < 1, works for probabilities
double RandomClass::random() {
	return rng.rand53();
}

// Generates random number from 0 to range
int RandomClass::random(int range) {
	return rng.randInt(range);
}

// Generates values between start and end
int RandomClass::random(int start, int end){
	return rng.randInt((end - start) + 1) + start;
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
void RandomClass::setSeed(int seed){
	rng.seed(seed);
}

/*
 * Random.cpp
 *
 *  Created on: Sep 3, 2014
 *      Author: diogonal
 */

#include "Random.h"
#include<time.h>
#include <cstdlib>

Random::Random(){
	srand ( time(NULL) );
}


Random::~Random(){}

void Random::resetSeed(){
	srand ( time(NULL) );
}

/* Generates random number < 1, works for probabilities */
double Random::random() {


	double rng=(double) rand();
	double maxrange=(double)RAND_MAX;
	//if (rng==0)
		//rng=0.000000001;
	double rand01=(rng) / (maxrange);
	return (rand01);
}

/* Generates random number from 0 to range*/
int Random::random(int range) {
	return int(random() * range);
}

/* Generates values between start and end*/
int Random::random(int start, int end){
	return start + (rand() % end);
}

/* Generates values between 0 and 1*/
bool Random::randomBool() {
  return rand() % 2 == 1;
}

bool Random::randomBoolTrueBiased() {
  return rand() % 4 < 3;
}
bool Random::randomBoolFalseBiasedN() {
	return rand() % 10000 > 9000;
	//return rand() % 1000 > 900;
}
bool Random::randomBoolFalseBiasedZN() {
  //return rand() % 10000 > 9990;
  return rand() % 10000 > 9998;
}

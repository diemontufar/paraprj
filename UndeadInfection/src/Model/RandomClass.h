/*
 * Random.h
 *
 *  Created on: 03/09/2014
 *      Author: Andres Chaves, Gustavo Carrion, Diego Montufar
 */

#ifndef RANDOMCLASS_H_
#define RANDOMCLASS_H_
#include<time.h>
#include <ctime>
#include <cstdlib>

#include "MersenneTwister.h"

class RandomClass
{
private:
	typedef MTRand MyRNG;  // the Mersenne Twister with a popular choice of parameters
	unsigned int seed;           // populate somehow

//	std::uniform_int_distribution<int> uint_dist;         // by default range [0, MAX]
	//std::uniform_int_distribution<int> dist10(0,10); // range [0,10]
	//std::uniform_real_distribution<double> zeroToOne(0.0, 1.0);

	MyRNG rng;                   // e.g. keep one global instance (per thread)

public:
	RandomClass();
    RandomClass(unsigned int seed);
    virtual ~RandomClass();

    double random();
    double random(double max);
    int random(int range);
    int random(int start, int end);
    bool randomBool();
    bool randomBoolTrueBiased();
    bool randomBoolFalseBiasedN();
    bool randomBoolFalseBiasedZN();
    void setSeed(int seed);
};


#endif /* RANDOM_H_ */

/*
 * RandomGen.h
 *
 *  Created on: 24/08/2014
 *      Author: achaves
 */

#ifndef RANDOMGEN_H_
#define RANDOMGEN_H_
#include <random>

class RandomGen {
public:
	RandomGen();
	virtual ~RandomGen();
	virtual int getIntUniformRandomBetween(int start, int end);
};

#endif /* RANDOMGEN_H_ */

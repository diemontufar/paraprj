/*
 * RandomGenerator.h
 *
 *  Created on: Aug 30, 2014
 *      Author: GUZ
 */

#ifndef RANDOMGENERATOR_H_
#define RANDOMGENERATOR_H_
#include <time.h>
#include <stdlib.h>

class RandomGenerator {
public:
	RandomGenerator();
	virtual ~RandomGenerator();
	static bool randomBool();
};

#endif /* RANDOMGENERATOR_H_ */

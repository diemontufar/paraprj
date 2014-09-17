/*
 * RandomGen.h
 *
 *  Created on: 24/08/2014
 *      Author: achaves
 */

#ifndef RANDOMGEN_H_
#define RANDOMGEN_H_

class RandomGen {
public:
	RandomGen();
	virtual ~RandomGen();
	//virtual int getIntUniformRandomBetween(int start, int end);
	static int getIntUniformRandomBetween(int start, int end);
	static bool randomBool();
};

#endif /* RANDOMGEN_H_ */

/*
 * Zombie.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Agent.h"

class Zombie: public Agent {
private:
	const int SPEED = 1;
	const int DECOMPOSITIONTIME = 10;

public:
	Zombie();
	virtual ~Zombie();

	void step(){
	}
};


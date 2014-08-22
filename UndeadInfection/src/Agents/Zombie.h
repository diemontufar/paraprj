/*
 * Zombie.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */


#ifndef ZOMBIE_H_
#define ZOMBIE_H_
#include "Agent.h"

class Zombie: public virtual Agent {
private:
    short decompositionTime; //Number of ticks that have passed
    bool decomposed;
public:
	Zombie();
	virtual ~Zombie();
	virtual void step();
	virtual AgentTypeEnum getType();
	bool isDecomposed();
};

#endif

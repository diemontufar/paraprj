/*
 * Zombie.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */


#ifndef ZOMBIE_H_
#define ZOMBIE_H_
#include "Agent.h"
#include "../Model/RandomGen.h"

class Zombie: public virtual Agent {
private:
    short decompositionTime; //Number of ticks that have passed
    bool shooted;
    bool decomposed;
    short lifeTime;
public:
	Zombie();
	virtual ~Zombie();
	virtual void step();
	virtual AgentTypeEnum getType();
	bool isDecomposed();
	bool isShooted();
	void shoot();
};

#endif

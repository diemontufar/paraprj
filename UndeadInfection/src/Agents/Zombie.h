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
    bool shooted;
    bool decomposed;
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

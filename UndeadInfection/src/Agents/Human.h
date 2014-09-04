/*
 * Represents a Human agent
 * Human.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */


#ifndef HUMAN_H_
#define HUMAN_H_
#include "Agent.h"

class Human: public Agent {
private:

    bool gender; //True is male, false is female
    int age;
    bool infected; //Whether the human is infected or not
    bool hasAGun; //Whether the human has a gun or not
    bool dead;
    short int infectionTime; //How much ticks have passed since the human has been exposed

public:
	Human(bool gender,int age, bool hasAGun);
	bool getGender();
	int getAge();
	bool isHasAGun();
	bool isInfected();
	bool isDead();
	void infect();
	virtual ~Human();
	virtual void step();
	virtual AgentTypeEnum getType();

};
#endif

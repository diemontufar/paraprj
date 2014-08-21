/*
 * Human.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Agent.h"

class Human: public Agent {
private:
	const int SPEED = 2;
	const int INFECTIONTIME = 5;

    bool gender;
    bool infected;
    bool hasAGun;
    short int infectionTime;

public:
	Human();
	virtual ~Human();

	void step(){
	}


};

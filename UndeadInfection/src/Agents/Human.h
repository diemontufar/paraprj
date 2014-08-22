/*
 * Human.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Agent.h"

class Human: public Agent {
private:


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

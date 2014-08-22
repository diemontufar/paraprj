/*
 * Represents a Human agent
 * Human.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Agent.h"

class Human: public Agent {
private:


    bool gender; //True male, false female
    bool infected; //Whether the human is infected or not
    bool hasAGun; //Whether the human has a gun or not
    short int infectionTime; //How much ticks have passed since the human has been exposed

public:
	Human(bool gender, bool hasAGun);
	bool getGender();
	bool isHasAGun();
	bool isInfected();
	virtual ~Human();

	void step(){
	}


};

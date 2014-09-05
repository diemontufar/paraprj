/*
 * Counters.h
 *
 *  Created on: 02/09/2014
 *      Author: achaves
 */

#ifndef COUNTERS_H_
#define COUNTERS_H_

class Counters {
public:
	static Counters& getInstance()
	{
		static Counters instance;
		return instance;
	}
	void resetCounters(){dead=shooted=infected=zDead=outOfBounds=0;}
	void newDead(){dead++;}
	void newOutOfBounds(){outOfBounds++;}
	void newZombieDead(){zDead++;}
	void newShooted(){shooted++;}
	void newInfected(){infected++;}
	void newConversion(){converted++;}
	int getDead(){return dead;}
	int getShooted(){return shooted;}
	int getInfected(){return infected;}
	int getConverted(){return converted;}
	int getZDead(){return zDead;}
	int getOutOfBounds(){return outOfBounds;}
private:
	int dead;
	int shooted;
	int infected;
	int converted;
	int zDead;
	int outOfBounds;

	Counters(){dead=shooted=infected=converted=zDead=outOfBounds=0;};
	Counters(Counters const&);
	void operator=(Counters const&);
};

#endif /* COUNTERS_H_ */

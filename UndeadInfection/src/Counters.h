/*
 * Counters.h
 *
 *  Created on: 02/09/2014
 *      Author: Andres Chaves, Gustavo Carrion, Diego Montufar
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
	void resetCounters(){dead=shooted=infected=zDead=outOfBounds=ghostCase=hDead=born=converted=0;}
	void newDead(){dead++;}
	void newOutOfBounds(){outOfBounds++;}
	void newZombieDead(){zDead++;}
	void newHumanDead(){hDead++;}
	void newShooted(){shooted++;}
	void newInfected(){infected++;}
	void newConversion(){converted++;}
	void newGhostCase(){ghostCase++;}
	void newBorn(){born++;}
	int getDead(){return dead;}
	int getShooted(){return shooted;}
	int getInfected(){return infected;}
	int getConverted(){return converted;}
	int getZDead(){return zDead;}
	int getHumanDead(){return hDead;}
	int getOutOfBounds(){return outOfBounds;}
	int getGhostCase(){return ghostCase;}
	int getBorn(){return born;}
private:
	int dead;
	int shooted;
	int infected;
	int converted;
	int ghostCase;
	int zDead;
	int hDead;
	int outOfBounds;
	int born;

	Counters(){dead=shooted=infected=converted=zDead=outOfBounds=ghostCase=born=hDead=0;};
	Counters(Counters const&);
	void operator=(Counters const&);
};

#endif /* COUNTERS_H_ */

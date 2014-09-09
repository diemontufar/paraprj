/*
 * Grid.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */


#ifndef GRID_H_
#define GRID_H_

#include "../Agents/Agent.h"
#include "../Agents/Human.h"
#include "../Agents/Zombie.h"
#include "../Counters.h"
#include "RandomGen.h"
#include "Random.h"
#include <iostream>
#include <stdlib.h>

class Cell;

class Grid {
private:
	Agent* gridA[GRIDROWS+2][GRIDCOLUMNS+2];
	Agent* gridB[GRIDROWS+2][GRIDCOLUMNS+2];
	RandomGen* randomObj;

	void printMatrix(int);
	void printState(int);
	void merge();
	void resolveHumanZombie(Agent*, Agent*);
public:
	Grid();
	void initialize (int, int );
	void addAgent( int, int, Agent* );
	void removeAgent( int, int, Agent* );

	void moveAgent( int, int, Agent* );
	void setRandomObj(RandomGen*);
	void run();
	virtual ~Grid();
};

#endif /* GRID_H_ */

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

class Cell;

class Grid {
private:
	Agent* gridA[CELLROWSPERGRID+2][CELLCOLUMNSPERGRID+2];
	Agent* gridB[CELLROWSPERGRID+2][CELLCOLUMNSPERGRID+2];
	RandomGen* randomObj;

	void printState(int);

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

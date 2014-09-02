/*
 * Grid.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */


#ifndef GRID_H_
#define GRID_H_

#include "Cell.h"
#include "../Agents/Agent.h"
#include "RandomGen.h"

class Cell;

class Grid {
private:
	Cell* cells[CELLROWSPERGRID][CELLCOLUMNSPERGRID];

public:
	Grid();
	void initialize (int, int );
	void addAgent( int, int, Agent* );
	void moveAgentCurrentToCurrent( int, int, Agent*,int ,int);
	void moveAgentCurrentToCandidate( int, int, Agent*,int ,int);
	void moveAgentCandidateToCurrent( int, int, Agent*,int ,int);
	void removeAgent( int, int, Agent* );
	void print();
	void calculateAgents( int &humans, int &zombies );
	void moveAgent( int, int, Agent* );
	void step();
	virtual ~Grid();
};

#endif /* GRID_H_ */

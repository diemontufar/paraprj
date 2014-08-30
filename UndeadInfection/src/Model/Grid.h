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
#include "../Utils/RandomGenerator.h"

class Cell;

class Grid {
private:
	Cell* cells[CELLROWSPERGRID][CELLCOLUMNSPERGRID];

public:
	Grid();
	void initialize (int, int );
	void addAgent( int, int, Agent* );
	void moveAgent( int, int, Agent*,int ,int);
	void removeAgent( int, int, Agent* );
	void print();
	void moveAgent( int, int, Agent* );
	virtual ~Grid();
};

#endif /* GRID_H_ */

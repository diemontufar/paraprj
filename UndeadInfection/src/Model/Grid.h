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

class Cell;

class Grid {
private:
	Cell** cells;

public:
	Grid();
	void addAgent( int, int, Agent* );
	void moveAgent( int, int, Agent* );
	virtual ~Grid();
};

#endif /* GRID_H_ */

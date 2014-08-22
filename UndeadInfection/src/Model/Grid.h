/*
 * Grid.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Cell.h"
#include "../Agents/Agent.h"

#ifndef GRID_H_
#define GRID_H_

class Grid {
private:
	Cell** cells;

public:
	Grid();
	void addAgent(int x, int y, Agent agent);
	virtual ~Grid();
};

#endif /* GRID_H_ */

/*
 * Grid.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */


#ifndef GRID_H_
#define GRID_H_

#include "../Agents/Agent.h"
#include "../Counters.h"
#include "Random.h"
#include <iostream>
#include <stdlib.h>
#include <omp.h>

class Cell;

class Grid {
private:

	void printMatrix(int, Agent***);
	void printState(int, Agent***);
	void merge(Agent***, Agent***);
	void resolveHumanZombie(Agent*, Agent*);
	void resolveGridHumanZombie(Agent* agent,int i, int j, Agent***);
	void calculatePopulationAndFreeCells(float &population, float &freecells, Agent***);
public:
	Grid();
	Agent*** createMesh ();
	void initialize ( Agent***, Agent*** );
	void addAgent( int, int, Agent* );
	void removeAgent( int, int, Agent* );

	void moveAgent( int, int, Agent* );
	void run();
	virtual ~Grid();
};

#endif /* GRID_H_ */

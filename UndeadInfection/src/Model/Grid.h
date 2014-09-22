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
#include "RandomClass.h"
#include <iostream>
#include <omp.h>
#include <math.h>

class Cell;

class Grid {
private:

	int candidateI;
	int candidateJ;

	void printMatrix(int, Agent***);
	void printState(int, Agent***);
	void merge(Agent***, Agent***);
	void resolveHumanZombie(Agent*, Agent*, RandomClass random);
	void resolveGridHumanZombie(Agent* agent,int i, int j, Agent***, RandomClass random);
	void resolveGridHumanHuman(Agent* agent,int i, int j, Agent***, RandomClass random);
	void calculateStatistics(float &humanPopulation,float &zombies, float &freeCells, Agent*** gridA);
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

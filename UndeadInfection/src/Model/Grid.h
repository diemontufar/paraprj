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
	void printState(int, Agent***, int infected, int converted, int shooted, int zDead, int ghostCase, int hDead, int born);
	void merge(Agent***, Agent***);
	void resolveHumanZombie(Agent*, Agent*, RandomClass random, int &shooted, int &infected);
	void resolveGridHumanZombie(Agent* agent,int i, int j, Agent***, RandomClass random, int &shooted, int &infected);
	void resolveGridHumanHuman(Agent* agent,int i, int j, Agent***, RandomClass random, float totalHumans, float totalZombies, float freeCells, int &newborn);
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

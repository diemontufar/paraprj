/*
 * Cell.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#ifndef CELL_H_
#define CELL_H_
#include "../Agents/Agent.h"
#include "../Agents/Human.h"
#include "../Agents/Zombie.h"
#include "Grid.h"
#include "RandomGen.h"

class Grid;

class Cell {
private:
	Agent* currentAgent;
	Agent* candidateAgent;
	int x;
	int y;
	Grid* grid;

	void resolveHumanHuman();
	void resolveHumanZombie();
	void resolveZombieZombie();

public:
	Cell();
	virtual ~Cell();
	void setCurrentAgent(Agent*);
	void setCandidateAgent(Agent*);
	Agent* getCurrentAgent();
	Agent* getCandidateAgent();
	void setGrid(Grid*);
	void resolve();
	void setCoordinates(int, int);
};

#endif /* CELL_H_ */

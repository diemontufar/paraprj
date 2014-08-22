/*
 * Model.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Grid.h"
#include "../Agents/Agent.h"
#include "../Agents/Human.h"
#include "../Agents/Zombie.h"

#ifndef MODEL_H_
#define MODEL_H_

int const GRIDROWS = 20;
int const GRIDCOLUMNS = 20;

int const CELLROWSPERGRID = 10;
int const CELLCOLUMNSPERGRID = 10;

class Model {
private:
	Grid grids[GRIDROWS][GRIDCOLUMNS];

public:
	Model();
	virtual ~Model();

	void setup();
	void step();
};

#endif /* MODEL_H_ */

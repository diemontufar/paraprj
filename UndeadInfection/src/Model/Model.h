/*
 * Model.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Grid.h"
#include "../Constants.h"
#include "../Agents/Agent.h"
#include "../Agents/Human.h"
#include "../Agents/Zombie.h"

#ifndef MODEL_H_
#define MODEL_H_

class Model {
private:
	Grid *grids[GRIDROWS][GRIDCOLUMNS];
	void printState(int);
public:
	Model();
	virtual ~Model();

	void setup();
	void run();

};

#endif /* MODEL_H_ */

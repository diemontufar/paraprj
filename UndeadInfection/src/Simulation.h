/*
 * Simulation.h
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Model/Model.h"

#ifndef SIMULATION_H_
#define SIMULATION_H_

class Simulation {
private:
	Model model;

public:
	Simulation();
	virtual ~Simulation();
	void start();
};

#endif /* SIMULATION_H_ */

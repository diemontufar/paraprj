/*
 * GridMPI.h
 *
 *  Created on: 14/10/2014
 *      Author: achaves
 */

#ifndef GRIDMPI_H_
#define GRIDMPI_H_

#include "../Agents/Agent.h"
#include "RandomClass.h"
#include <mpi.h>
#include <iostream>

#define SOURCE 0
#define DEST 1
#define TAG 2013

class GridMPI {
private:

	Agent** createMesh ();
	void initialize ( Agent**, Agent**, int rank );
	void calculateStatistics(int *stats, Agent** gridA);
	void printMatrixBarrier(int tick, Agent** gridA, int rank, char gridName);
	void printMatrix(int tick, Agent** gridA, int rank,  char gridName);
public:
	GridMPI();
	virtual ~GridMPI();

	int run(int argc, char** argv);
};

#endif /* GRIDMPI_H_ */

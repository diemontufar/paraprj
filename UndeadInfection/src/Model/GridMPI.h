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
#define TOTALCOUNTERS 10
#define MEN 0
#define WOMEN 1
#define SHOT 2
#define INFECTED 3
#define CONVERTED 4
#define GHOSTCASE 5
#define CLASHCASE 6
#define ZDEAD 7
#define HDEAD 8
#define BORN 9


class GridMPI {
private:

	Agent** createMesh ();
	void initialize ( Agent**, Agent**, int rank );
	void calculateStatistics(float &humanPopulation,float &zombies, float &freeCells, Agent** gridA);
	void printMatrixBarrier(int tick, Agent** gridA, int rank, char gridName);
	void printMatrix(int tick, Agent** gridA, int rank,  char gridName);
	void fullExchange(int myID, MPI_Datatype agentDatatype, Agent** gridA,
			MPI_Status& status);
	void ownershipExchange(int myID, MPI_Datatype agentDatatype, Agent** gridB, MPI_Status& status, int* localStats);
	void applyBoundary(Agent** gridB, int* localStats);
	void resolveHumanZombie(Agent &human, Agent &zombie, RandomClass random, int *localStats);

public:
	GridMPI();
	virtual ~GridMPI();

	int run(int argc, char** argv);
};

#endif /* GRIDMPI_H_ */

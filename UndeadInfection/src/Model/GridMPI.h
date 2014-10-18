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
#include <iomanip>

#define SOURCE 0
#define DEST 1
#define TAG 2013
#define TOTALCOUNTERS 11
#define ZOMBIES 0
#define MEN 1
#define WOMEN 2
#define SHOT 3
#define INFECTED 4
#define CONVERTED 5
#define GHOSTCASE 6
#define CLASHCASE 7
#define ZDEAD 8
#define HDEAD 9
#define BORN 10


class GridMPI {
private:

	Agent** createMesh ();
	void initialize ( Agent**, Agent**, int rank );
	void calculateStatistics(float &freeCells, Agent** gridA, int* localStats);
	void printMatrixBarrier(int tick, Agent** gridA, int rank, char gridName);
	void printMatrix(int tick, Agent** gridA, int rank,  char gridName);
	void fullExchange(int myID, MPI_Datatype agentDatatype, Agent** gridA,
			MPI_Status& status);
	void ownershipExchange(int myID, MPI_Datatype agentDatatype, Agent** gridB, MPI_Status& status, int* localStats);
	void applyBoundary(Agent** gridB, int* localStats);
	void resolveHumanZombie(Agent &human, Agent &zombie, RandomClass& random, int *localStats);
	void resolveGridHumanHuman(Agent &agentA,int i, int j, Agent** gridA, RandomClass& random, int* localStats, float probHumanHaveBaby);
	void printState(int tick, int* stats);

public:
	GridMPI();
	virtual ~GridMPI();

	int run(int argc, char** argv);
};

#endif /* GRIDMPI_H_ */

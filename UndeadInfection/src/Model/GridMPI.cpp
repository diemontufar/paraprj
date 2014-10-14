/*
 * GridMPI.cpp
 *
 *  Created on: 14/10/2014
 *      Author: achaves
 */

#include "GridMPI.h"

using namespace std;
GridMPI::GridMPI() {
	// TODO Auto-generated constructor stub

}

GridMPI::~GridMPI() {
	// TODO Auto-generated destructor stub
}
Agent** GridMPI::createMesh (){
	Agent** grid = new Agent*[GRIDROWS+2];
	grid[0] = new Agent [(GRIDROWS+2)*(GRIDCOLUMNS*2)];

	for(int i=1, ii=GRIDCOLUMNS+2; i<GRIDROWS+2; i++, ii+=(GRIDCOLUMNS+2))
	{
		grid[i] = &grid[0][ii];
	}

	for (int i = 0; i <= GRIDROWS+1; i++) {
		for (int j = 0; j <= GRIDCOLUMNS+1; j++) {
			grid[i][j].clean();
		}
	}
	return grid;
}
void GridMPI::initialize(	Agent** gridA, Agent** gridB, int rank ) {
	int numZombies = 0;
	int numHumans = 0;
	RandomClass random;
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridB[i][j].clean();
			//todo:change algorithm so it is random and with pop limits
			if (numHumans < DARWINPOPDENSITY && random.randomBoolFalseBiasedN()) {
				int age = random.random(MINLIFEEXPECTANCY - 1);
				int lifeExpectancy=random.random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY);
				bool gender = random.random() < GENDERRATIO ? true : false;
				bool hasAGun = random.random() < GUNDENSITY ? true : false;

				gridA[i][j].migrateToHuman(gender, age, hasAGun, lifeExpectancy);
				numHumans++;
			} else {
				if (numZombies < NUMBEROFZOMBIES && random.randomBoolFalseBiasedZN()) {
					int lifeTime = random.random(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME);
					gridA[i][j].migrateToZombie(lifeTime);
					numZombies++;
				}
			}
			//cout << "i:  "<<i << "j:  "<<j <<"numInitialized with humans: "<< endl;
		}
	}
	cout << "Process: "<<rank<< " Initialized with Humans: " << numHumans << " and Zombies: " << numZombies << endl;
}

int GridMPI::run(int argc, char** argv){
	cout << "Starting MPI simulation... " << endl;
	cout << "Human: " << human << endl;
	cout << "Zombie: " << zombie << endl;
	cout << "None: " << none << endl;
	double wtime= 0;

	//MPI INIT
	int	myID;
	int	N_Procs;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &N_Procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);

	if(myID==0)	{
		wtime = MPI_Wtime();
	}

	//Allocating arrays
	Agent** gridA = createMesh();
	Agent** gridB = createMesh();
	initialize(gridA, gridB, myID);

	//Calculating the jump size to send row
	MPI_Status		status;
	MPI_Datatype agentDatatype;
	MPI_Type_contiguous(sizeof(Agent), MPI_BYTE, &agentDatatype);
	MPI_Type_commit(&agentDatatype);
	MPI_Datatype agentVectorDatatype;
	MPI_Type_vector(GRIDROWS, 1, GRIDCOLUMNS+2, agentDatatype, &agentVectorDatatype);
	MPI_Type_commit(&agentVectorDatatype);

	/*	if (myID == SOURCE) {
		gridA[GRIDROWS+1][0].migrateToZombie(6);
		printf("Sending agents\n");
	    MPI_Send(&gridA[0][0], 1, agentVectorDatatype, DEST, TAG, MPI_COMM_WORLD);
	    printf("Processor %d sent %d Agents %d\n", myID, (GRIDROWS+2),gridA[GRIDROWS+1][0].getType());
	}
	if (myID == DEST){
		printf("%d GridB state %d\n", myID, gridB[GRIDROWS+1][0].getType());
		MPI_Recv(&gridB[0][0], 1, agentVectorDatatype, SOURCE, TAG, MPI_COMM_WORLD, &status);
		printf("Processor %d got a lot of Agents %d\n", myID, gridB[GRIDROWS+1][0].getType());
	}*/


	//Time loop
	RandomClass random;
	random.setSeed(0xFFFF*drand48());

	for (int n = 0; n < NUMTICKS; n++) {
		if (myID == SOURCE) {
			MPI_Send(&gridA[1][1], 1, agentVectorDatatype, DEST, TAG, MPI_COMM_WORLD);
			MPI_Recv(&gridA[1][0], 1, agentVectorDatatype, DEST, TAG, MPI_COMM_WORLD, &status);
		}
		if (myID == DEST){
			MPI_Recv(&gridA[1][GRIDROWS+1], 1, agentVectorDatatype, SOURCE, TAG, MPI_COMM_WORLD, &status);
			MPI_Send(&gridA[1][GRIDROWS], 1, agentVectorDatatype, SOURCE, TAG, MPI_COMM_WORLD);
		}
		//MOVE
		for (int i = 1; i <= GRIDROWS; i++) { //Move
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				if ( gridA[i][j].getType() != none ) {
					Agent agent = gridA[i][j];
					//Agent already dead no comparisons required
					if (agent.getType() != none) {
						double move = random.random();
						int desti = 0;
						int destj = 0;

						if (move < 1.0 * MOVE) {
							desti = i - 1;
							destj = j;
						} else if (move < 2.0 * MOVE) {
							desti = i + 1;
							destj = j;
						} else if (move < 3.0 * MOVE) {
							desti = i;
							destj = j - 1;
						} else if (move < 4.0 * MOVE) {
							desti = i;
							destj = j + 1;
						} else {
							desti = i;
							destj = j;
						}
						if (gridA[desti][destj].getType() == none && gridB[desti][destj].getType() == none) {
							gridB[desti][destj].clone(agent);
							//#ifdef DEBUGGRID
							//std::cerr << "Tick: " << (n+1) << "Human moved from" << i <<","<< j << " to "<< desti <<","<< destj<<"\n";
							//#endif

						} else {
							gridB[i][j].clone(agent);
						}
					}
					agent.clean();
				}
			}
		}
		//Apply Boundary
	}

		if(myID==0)
		{
			wtime	= MPI_Wtime() - wtime;	// Record the end time and calculate elapsed time
			cout << "Simulation took " << wtime/NUMTICKS << " seconds per time step with " << N_Procs << " processes" << endl;
		}

		MPI_Finalize();
		return 0;

	}
	void exchange(double** phiH, double** phiVx, double** phiVy , int myN_x, int myN_y, int myID,
			int rightNeighbor, int leftNeighbor, int topNeighbor, int bottomNeighbor, MPI_Comm Comm2D, MPI_Datatype strideType){
		MPI_Status		status;
		MPI_Sendrecv(&(phiH[2][2]),			myN_y,	MPI_DOUBLE,	leftNeighbor,		0,	&(phiH[myN_x+2][2]),	myN_y,	MPI_DOUBLE,	rightNeighbor,		0, Comm2D, &status);
	}

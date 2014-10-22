/*
 * GridHybrid.cpp
 *
 *  Created on: 22/10/2014
 *      Author: achaves
 */

#include "GridHybrid.h"
using namespace std;

GridHybrid::GridHybrid() {
	// TODO Auto-generated constructor stub

}

GridHybrid::~GridHybrid() {
	// TODO Auto-generated destructor stub
}

Agent** GridHybrid::createMesh (){
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
void GridHybrid::initialize(	Agent** gridA, Agent** gridB, int rank ) {
	int numZombies = 0;
	int numHumans = 0;
	RandomClass random;
	random.setSeed(0xFFFF*drand48());
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			gridB[i][j].clean();
			//todo:change algorithm so it is random and with pop limits
			if (numHumans < DARWINPOPDENSITY && random.randomBoolFalseBiasedN()) {
				int age = random.random(MINLIFEEXPECTANCY - 1);
				int lifeExpectancy=random.random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY);
				bool gender = random.random() < GENDERRATIO ? true : false;
				bool hasAGun = random.random() < GUNDENSITY ? true : false;
				int numBabies = 0;

				gridA[i][j].migrateToHuman(gender, age, hasAGun, lifeExpectancy, numBabies);
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

void GridHybrid::fullExchange(int myID, MPI_Datatype agentDatatype, Agent** gridA,	MPI_Status& status) {
	//Move all rows
	if (myID == SOURCE) {
		MPI_Send(&gridA[GRIDROWS][0], GRIDCOLUMNS + 2, agentDatatype, DEST, TAG,
				MPI_COMM_WORLD );
		MPI_Recv(&gridA[GRIDROWS + 1][0], GRIDCOLUMNS + 2, agentDatatype, DEST,
				TAG, MPI_COMM_WORLD, &status);
	}
	if (myID == DEST) {
		MPI_Recv(&gridA[0][0], GRIDCOLUMNS + 2, agentDatatype, SOURCE, TAG,
				MPI_COMM_WORLD, &status);
		MPI_Send(&gridA[1][0], GRIDCOLUMNS + 2, agentDatatype, SOURCE, TAG,
				MPI_COMM_WORLD );
	}
}
void GridHybrid::ownershipExchange(int myID, MPI_Datatype agentDatatype, Agent** gridB, MPI_Status& status, int* localStats){
	Agent rowToReceive[GRIDCOLUMNS+2];
	if (myID == SOURCE) {
		MPI_Send(&gridB[GRIDROWS+1][0], GRIDCOLUMNS+2, agentDatatype, DEST, TAG, MPI_COMM_WORLD);
		MPI_Recv(&rowToReceive, GRIDCOLUMNS+2, agentDatatype, DEST, TAG, MPI_COMM_WORLD, &status);

		for (int j = 0; j < GRIDCOLUMNS+2; j++){
			gridB[GRIDROWS+1][j].clean(); //Clean because we sent ownership to process B
			if (rowToReceive[j].getType()!=none){
				if (gridB[GRIDROWS][j].getType()!=none){
					localStats[CLASHCASE]++;
				}
				gridB[GRIDROWS][j] = rowToReceive[j];
			}
		}
	}
	else{
		MPI_Recv(&rowToReceive, GRIDCOLUMNS+2, agentDatatype, SOURCE, TAG, MPI_COMM_WORLD, &status);
		MPI_Send(&gridB[0][0], GRIDCOLUMNS+2, agentDatatype, SOURCE, TAG, MPI_COMM_WORLD);
		for (int j = 0; j < GRIDCOLUMNS+2; j++){
			gridB[0][j].clean(); //Clean because we changed ownership
			if (rowToReceive[j].getType()!=none){
				if (gridB[1][j].getType()!=none){
					localStats[CLASHCASE]++;
				}
				gridB[1][j] = rowToReceive[j];
			}
		}
	}
}
void GridHybrid::applyBoundary(Agent** gridB, int* localStats){
	for (int i = 1; i <= GRIDROWS; i++) {
		if ( gridB[i][0].getType() != none && gridB[i][1].getType() == none) {
			Agent a = gridB[i][0];
			gridB[i][1].clone(a);
			gridB[i][0].clean();
		} else if ( gridB[i][0].getType() != none && gridB[i][1].getType()!=none ) {
			gridB[i][0].clean();
			localStats[GHOSTCASE]++;
		}
		if (gridB[i][GRIDCOLUMNS + 1].getType() != none && gridB[i][GRIDCOLUMNS].getType() == none ) {
			gridB[i][GRIDCOLUMNS].clone(gridB[i][GRIDCOLUMNS + 1]);
			gridB[i][GRIDCOLUMNS + 1].clean();
		} else if (gridB[i][GRIDCOLUMNS + 1].getType()!=none && gridB[i][GRIDCOLUMNS].getType() != none) {
			gridB[i][GRIDCOLUMNS + 1].clean();
			localStats[GHOSTCASE]++;
		}
	}

	for (int j = 1; j <= GRIDCOLUMNS; j++) {
		if (gridB[0][j].getType() != none && gridB[1][j].getType() == none) {
			gridB[1][j].clone(gridB[0][j]);
			gridB[0][j].clean();
		} else if (gridB[0][j].getType() != none && gridB[1][j].getType() != none) {
			gridB[0][j].clean();
			localStats[GHOSTCASE]++;
		}
		if (gridB[GRIDROWS + 1][j].getType()!=none && gridB[GRIDROWS][j].getType() == none) {
			gridB[GRIDROWS][j] = gridB[GRIDROWS + 1][j];
			gridB[GRIDROWS + 1][j].clean();
		} else if (gridB[GRIDROWS + 1][j].getType() != none && gridB[GRIDROWS][j].getType() != none) {
			gridB[GRIDROWS + 1][j].clean();
			localStats[GHOSTCASE]++;
		}
	}
}
int GridHybrid::run(int argc, char** argv){
	cout << "Starting MPI simulation... " << endl;
	cout << "Human: " << human << endl;
	cout << "Zombie: " << zombie << endl;
	cout << "None: " << none << endl;
	double wtime= 0;

	//MPI INIT
	int	myID;
	int	N_Procs;
	int localStats[TOTALCOUNTERS]; //men, women, shot, infected, converted, ghostCase, clashCase, zDead, hDead, born
	int globalStats[TOTALCOUNTERS];
	float freeCells=0;

	for (int i = 0; i < TOTALCOUNTERS; i++){
		localStats[i] = globalStats[i] = 0;
	}
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
	write(gridA, 0, myID);

	//Calculating the jump size to send row
	MPI_Status		status;
	MPI_Datatype agentDatatype;
	MPI_Type_contiguous(sizeof(Agent), MPI_BYTE, &agentDatatype);
	MPI_Type_commit(&agentDatatype);
	MPI_Datatype agentVectorDatatype;
	MPI_Type_vector(GRIDROWS, 1, GRIDCOLUMNS+2, agentDatatype, &agentVectorDatatype);
	MPI_Type_commit(&agentVectorDatatype);

	//Time loop
	RandomClass random;
	random.setSeed(0xFFFF*drand48());


	for (int n = 0; n < NUMTICKS; n++) {
		//Check and Death
		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				if ( gridA[i][j].getType() != none ) {
					Agent& agent = gridA[i][j];
					agent.step();

					//Code to remove decomposed agents
					if ((agent.getType() == zombie) && (agent.isDecomposed() || agent.isShooted())) {
						localStats[ZDEAD]++;
						agent.clean();
					} else {
						if ( agent.getType() == human && agent.isNaturalDead() ) {
							localStats[HDEAD]++;
							agent.clean();
						} else {
							if ( agent.getType() == human && agent.isInfected() ) {
								localStats[CONVERTED]++;
								agent.migrateToZombie(random.random(MINDECOMPOSITIONTIME,MAXDECOMPOSITIONTIME));
							}
						}
					}
				}
			}
		}

		//Move all rows
		fullExchange(myID, agentDatatype, gridA, status);
		//printMatrixBarrier(n, gridA, myID, 'A');
		//MOVE
		for (int i = 1; i <= GRIDROWS; i++) { //Move
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				if ( gridA[i][j].getType() != none ) {
					Agent& agent = gridA[i][j];
					//Agent already dead no comparisons required
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
					agent.clean();
				}
			}
		}
		//Move ownership B to A
		ownershipExchange(myID, agentDatatype, gridB, status, localStats);
		//Apply Boundary
		applyBoundary(gridB, localStats);
		swap(gridA, gridB);

		localStats[MEN] = localStats[WOMEN] = localStats[ZOMBIES] = freeCells = 0;
		calculateStatistics(freeCells, gridA, localStats);
        //Probability of birth
		double delta = 0,probPair=0,probAnyHumanHaveBaby=0;
		//2. Calculate probabilities
		delta = (localStats[MEN]+localStats[WOMEN]) / (TOTALGRIDCELLS - localStats[ZOMBIES]); //delta is the density of humans
		probPair = 1-pow((1-delta),4); //Prob. two humans being pair 1-(1-delta)^4
		probAnyHumanHaveBaby = probPair * PROBTOHAVEBABYWHENPAIRED; //Prob. any Human will have a baby (Per capita birth rate)
        //cout << "TS: " << n << " Prob: "<< probAnyHumanHaveBaby << endl;
		//printMatrixBarrier(n, gridA, myID, 'A');

		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				//cout <<"Marked as dead counter: "<<markedAsDeadCounter<< "Humans to die:" << humansToDie << "Chance to die " <<chanceToDie<< endl;
				bool clashDone = false;
				Agent &agentA = gridA[i][j];
				if (agentA.getType() != none) {
					AgentTypeEnum typeA = agentA.getType();
					if (typeA == human) {
						if (gridA[i - 1][j].getType() != none) { //Someone up
							if (gridA[i - 1][j].getType() == zombie && !gridA[i - 1][j].isShooted()){
								resolveHumanZombie(agentA, gridA[i-1][j], random, localStats);
								clashDone = true;
							}
							else{
								resolveGridHumanHuman(agentA,i - 1,j, gridA, random, localStats, probAnyHumanHaveBaby, n);
								clashDone = true;
							}
						}
						if (!clashDone && gridA[i][j + 1].getType() != none) { //Someone on the right
							if (gridA[i][j+1].getType() == zombie && !gridA[i][j+1].isShooted()){
								resolveHumanZombie(agentA,gridA[i][j+1], random, localStats);
								clashDone = true;
							}
							else{
								resolveGridHumanHuman(agentA,i,j+1, gridA, random, localStats, probAnyHumanHaveBaby,n);
								clashDone = true;
							}
						}
						if (!clashDone && gridA[i + 1][j].getType() != none) { //Down
							if (gridA[i+1][j].getType() == zombie && !gridA[i+1][j].isShooted()){
								resolveHumanZombie(agentA,gridA[i+1][j], random, localStats);
								clashDone = true;
							}
							else{
								resolveGridHumanHuman(agentA,i + 1,j, gridA, random, localStats, probAnyHumanHaveBaby,n);
								clashDone = true;
							}
						}
						if (!clashDone && gridA[i][j - 1].getType() != none) { //Left
							if (gridA[i][j-1].getType() == zombie && !gridA[i][j-1].isShooted()){
								resolveHumanZombie(agentA,gridA[i][j-1], random, localStats);
							}
							else{
								resolveGridHumanHuman(agentA,i,j - 1, gridA, random, localStats, probAnyHumanHaveBaby,n);
							}
						}
					}
				}
			}
		}
		//printMatrixBarrier(n, gridA, myID, 'A');
//BC?
		//Dead marking
		double deathRate = DEATHRATENT/(60000.0+(60000.0-(double)(localStats[MEN]+localStats[WOMEN])));
		for (int i = 1; i <= GRIDROWS; i++) {
			for (int j = 1; j <= GRIDCOLUMNS; j++) {
				//cout <<"Marked as dead counter: "<<markedAsDeadCounter<< "Humans to die:" << humansToDie << "Chance to die " <<chanceToDie<< endl;
				Agent &agent = gridA[i][j];
				if (agent.getType() == human) {
					//Death process
					if (!agent.isInfected()){
						double move = random.random();
						//					cout << "TS:" << n << "Death " <<move << "," << deathRate << endl;
						//cout << "TS:" << n << "Death " <<move << "," << deathRate <<"," << humansToDie<<"," << chanceToDie << endl;
						//if (move < DEATHRATENT ) {
						//if ((agent.getGender() && move <= DEATHRATEMALE) || (!agent.getGender() && move <= DEATHRATEFEMALE) ) {
						if ( move <= deathRate  ) {
							//cout<<markedAsDeadCounter<<"<="<<humansToDie<<endl;
							agent.markAsDead();
						}
					}
				}
			}
		}
		//printf("P %d Ts %d: %d humans and %d zombies, %d ghost cases %d deaths %d brths...\n", myID, n,(localStats[MEN]+localStats[WOMEN]),localStats[ZOMBIES], localStats[GHOSTCASE],localStats[HDEAD], localStats[BORN]);
		///Clean gridB
		for (int i = 0; i <= GRIDROWS+1; i++) {
			for (int j = 0; j <= GRIDCOLUMNS+1; j++) {
				gridB[i][j].clean();
			}
		}

		//printMatrixBarrier(n, gridA, myID,'C');
		localStats[MEN]=localStats[WOMEN]=localStats[ZOMBIES]=freeCells = 0;
		calculateStatistics(freeCells, gridA, localStats);
		MPI_Reduce(&localStats, &globalStats,TOTALCOUNTERS,	MPI_INT, MPI_SUM, SOURCE,	MPI_COMM_WORLD);
		if	( myID == SOURCE && n%NUMTICKSPRINT == 0 ){
			printState((n+1), globalStats);
			write(gridA, (n+1), myID);
			for (int i = 0; i < TOTALCOUNTERS; i++){
				globalStats[i] = 0;
				localStats[i] = 0;
			}
		}
		else if (myID == DEST && n%NUMTICKSPRINT == 0 ){
			for (int i = 0; i < TOTALCOUNTERS; i++){
				globalStats[i] = 0;
				localStats[i] = 0;
			}
		}
		localStats[MEN] = localStats[WOMEN] = localStats[ZOMBIES] = freeCells = 0;
		MPI_Barrier( MPI_COMM_WORLD );
	}

	if(myID==0)
	{
		wtime	= MPI_Wtime() - wtime;	// Record the end time and calculate elapsed time
		cout << "Simulation took " << wtime/NUMTICKS << " seconds per time step with " << N_Procs << " processes" << endl;
	}

	MPI_Finalize();
	return 0;

}
/*Prints the state of the gridA in any time step*/
void GridHybrid::printState(int tick, int* stats) {
	std::cout << "Tick:" << tick << ", Human: " << stats[MEN]+stats[WOMEN] << ", Male: "<< stats[MEN] << ", Female: " << stats[WOMEN] << ", Zombies: " << stats[ZOMBIES];
	std::cout << ", Infected:" << stats[INFECTED];
	std::cout << ", Converted: " << stats[CONVERTED] << ",Shots: " << stats[SHOT];
	std::cout << ", Dead Death: " << stats[ZDEAD] << ",Ghost Case: " << stats[GHOSTCASE];
	std::cout << ",Clash Cases " << stats[CLASHCASE] << ", Natural Death: " << stats[HDEAD];
	std::cout << ",Birth: " << stats[BORN] << "\n";
}

void GridHybrid::calculateStatistics(float &freeCells, Agent** gridA, int* localStats){
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			Agent agent = gridA[i][j];
			if (agent.getType() == human){
				if (agent.getGender())
					localStats[MEN]++;
				else
					localStats[WOMEN]++;;
			}
			else if (agent.getType() == zombie)
				localStats[ZOMBIES]++;
			else
				freeCells += 1.0;
		}
	}
}


/*Resolve conflicts between Human-Zombie*/
void GridHybrid::resolveHumanZombie(Agent &humanAgent, Agent &zombieAgent, RandomClass& random, int* localStats) {
	//Probability and cases, for now it is a rand
	int dice_roll = random.random(0,100);

	if (dice_roll <= HEADSHOTPERCENTAGE) {
		zombieAgent.shoot();
		localStats[SHOT]++;
	} else if (dice_roll <= SUCESSFULBITEPERCENTAGE) {
		humanAgent.infect();
		localStats[INFECTED]++;
	}
}
/*Resolve conflicts between Human-Human*/
void GridHybrid::resolveGridHumanHuman(Agent &agentA,int i, int j, Agent** gridA, RandomClass& random, int* localStats, double probAnyHumanHaveBaby, int ts) {
	Agent agentB = gridA[i][j];
	AgentTypeEnum typeB = agentB.getType();
	if (typeB == human && !agentB.isInfected() && !agentA.isInfected()) {

		//If humans have opposite genders, then try the chances to have a baby
		if ((agentA.getGender() && !agentB.getGender()) || (!agentA.getGender() && agentB.getGender())){

			//Only Humans between 15 and 45 years can have a baby
			if ( agentA.getAge()>=MINFERTILITYAGE && agentA.getAge()<=MAXFERTILITYAGE && agentB.getAge()>=MINFERTILITYAGE && agentB.getAge()<=MAXFERTILITYAGE ){
				//cout << "Fertility Range Checked" << endl;
				//1. Calculate statistics

				//Roll a dice
				double birth = random.random();
				//birth = ceil(birth*pow(10,6))/pow(10,6);

				//Find the first free space around the agent to place the baby
				int freeI=-1,freeJ=-1;
				bool foundIt = false; //help us to found the first free space
				//cout << birth << "," << probAnyHumanHaveBaby << endl;
				//cout << "Maybe a baby: "<< birth << "," << probAnyHumanHaveBaby << endl;

				if ( birth < probAnyHumanHaveBaby && agentA.getNumBabies()<=BIRTHSPERPERSON && agentB.getNumBabies()<=BIRTHSPERPERSON){
					//cout << "TS:"<<ts<<"A baby: "<< birth << "," << probAnyHumanHaveBaby << endl;
					//Its a baby!
					//cout<< "Random: "<< move <<"Chance: "<<prob <<endl;
					agentA.incrementNumBabies();
					agentB.incrementNumBabies();
					//cout << "Agent A: "<< agentA.getNumBabies() << "," << "Agent B: "<< agentB.getNumBabies() << endl;
					int numBabies = 0;
					bool gender = random.random() < GENDERRATIO ? true : false; //consider demographics and ages
					bool hasAGun = false; //->a baby does not have a gun
					int lifeExpectancy=random.random(MINLIFEEXPECTANCY, MAXLIFEEXPECTANCY);


					/*Find a free space for the baby*/
					if ( !foundIt && i  > 2 && gridA[i - 1][j].getType() == none ) { //Someone up
						freeI=i-1,freeJ=j;
						foundIt = true;
					}
					if ( !foundIt && j < GRIDROWS && gridA[i][j + 1].getType() == none ) { //Someone on the right
						freeI=i,freeJ=j+1;
						foundIt = true;
					}
					if ( !foundIt && i < GRIDROWS && gridA[i + 1][j].getType() == none ) { //Down
						freeI=i+1,freeJ=j;
						foundIt = true;
					}
					if ( !foundIt && j > 2 && gridA[i][j - 1].getType() == none ) { //Left
						freeI=i,freeJ=j-1;
						foundIt = true;
					}
					if ( !foundIt && i > 4 && gridA[i - 2][j].getType() == none ) { //Someone up up
						freeI=i-2,freeJ=j;
						foundIt = true;
					}
					if ( !foundIt && j < GRIDROWS - 2 && gridA[i][j + 2].getType() == none ) { //Someone on the right right
						freeI=i,freeJ=j+2;
						foundIt = true;
					}
					if ( !foundIt && i < GRIDROWS -2  && gridA[i + 2][j].getType() == none ) { //Down Down
						freeI=i+2,freeJ=j;
						foundIt = true;
					}
					if ( !foundIt && j > 4 && gridA[i][j - 2].getType() == none ) { //Left Left
						freeI=i,freeJ=j-2;
						foundIt = true;
					}

					//If we found a free space to the new born, place it, otherwise kill him! :(
					if (freeI!=-1 && freeJ!=-1 && foundIt ){
						//cout<<"Baby!"<<endl;
						gridA[freeI][freeJ].migrateToHuman(gender, 1, hasAGun, lifeExpectancy, numBabies);
						localStats[BORN]++;
					}
					else{
						//cout<<"Couldnt find a space!"<<endl;
					}
				}
			}
		}
	}
}

/* Print Grid*/
void GridHybrid::printMatrix(int tick, Agent** gridA, int rank, char gridName) {
	cout << endl;
	cout << "P:"<<rank<<"-GRID "<<gridName<<endl;
	//for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
	//	cout << j << (j <= 9 ? " |" : "|");
	//}
	//cout << endl;
	for (int i = 0; i <= GRIDROWS + 1; i++) {
		for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
			cout << "--";
		}
		cout << endl;
		for (int j = 0; j <= GRIDCOLUMNS + 1; j++) {
			if (j == 1)
				cout << "|";
			Agent agent = gridA[i][j];
			if (agent.getType() != none) {
				AgentTypeEnum currentType = agent.getType();
				if (currentType == human)
					cout << "h|";
				else
					cout << "z|";
			} else {
				cout << " |";
			}
		}
		cout << endl;
	}
}
void GridHybrid::printMatrixBarrier(int tick, Agent** gridA, int rank,  char gridName){
	MPI_Barrier( MPI_COMM_WORLD );
	if (rank == SOURCE)
		printMatrix(tick,gridA,rank, gridName);
	MPI_Barrier( MPI_COMM_WORLD );
	if (rank == DEST)
		printMatrix(tick,gridA,rank, gridName);
	MPI_Barrier( MPI_COMM_WORLD );
}
void GridHybrid::write(Agent** gridA, int ts, int rank){
    fstream         file;
    char            fileName[128];
    sprintf(fileName, "Zombies_%02d_%04d.txt", rank, ts);
    file.open(fileName, ios::out);
    file << "x,y,agent"<<endl;
	for (int i = 1; i <= GRIDROWS; i++) {
		for (int j = 1; j <= GRIDCOLUMNS; j++) {
			file << i<<","<<j<<","<<gridA[i][j].getType()<< endl;
		}
	}
    file.close();
}

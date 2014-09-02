/*
 * Cell.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Cell.h"
#ifdef DEBUGCELL
#include "iostream"
#endif

Cell::Cell(){
	randomObj = new RandomGen();
}
Cell::~Cell() {
}
void Cell::setGrid(Grid* theGrid){
	grid = theGrid;
}
void Cell::setCoordinates(int theX, int theY){
	x = theX;
	y = theY;
}
void Cell::step(){
	if ( currentAgent != nullptr ) currentAgent->step();
	if ( candidateAgent != nullptr ) candidateAgent->step();
	//Check if someone is dead or shooted
	if ( currentAgent != nullptr && currentAgent->getType() == human && (dynamic_cast<Human*>(currentAgent))->isDead() ){ //Its a zombie!
		grid->removeAgent(x ,y, currentAgent);
		grid->addAgent(x,y,new Zombie());
	}
	if ( currentAgent != nullptr && currentAgent->getType() == zombie &&
			(dynamic_cast<Zombie*>(currentAgent)->isShooted() || (dynamic_cast<Zombie*>(currentAgent))->isDecomposed()) ){ //Remove zombie
		grid->removeAgent(x ,y, currentAgent);
	}


	if (currentAgent == nullptr && candidateAgent == nullptr)//Empty cell do nothing
		return;
	if (currentAgent != nullptr && candidateAgent == nullptr)//Nothing to do
		return;

	if ( currentAgent != nullptr && candidateAgent != nullptr ){//There is someone in the cell, lets check
		AgentTypeEnum currentType = currentAgent->getType();
		AgentTypeEnum candidateType = candidateAgent->getType();

		if ( currentType == human && candidateType == human ){
			resolveHumanHuman();
		}
		else if (currentType==zombie && candidateType == zombie){
			resolveZombieZombie();
		}
		else{
			resolveHumanZombie();
		}
	}
	else{ //The cell is empty accept the candidate
		currentAgent = candidateAgent;
		candidateAgent = nullptr;
	}
}
void Cell::move(){
	if (currentAgent!=nullptr){
		int dice = randomObj->getIntUniformRandomBetween(0,8);
#ifdef DEBUGCELL
		std::cout << "Move dice: " << dice <<"\n";
#endif

		switch(dice){
		case 0:break; //Do nothing
		case 1: if ( y-1>=0 )
			grid->moveAgentCurrentToCandidate(x, y, currentAgent, x, y-1);//Superior left cell
		break;
		case 2: if( x-1>=0 )
			grid->moveAgentCurrentToCandidate(x,y, currentAgent, x-1, y);//Superior center
		break;
		case 3: if ( x-1>=0 )
			grid->moveAgentCurrentToCandidate(x,y, currentAgent, x-1, y+1);//superior right
		break;
		case 4: if ( y+1<CELLCOLUMNSPERGRID )
			grid->moveAgentCurrentToCandidate(x,y, currentAgent, x, y+1);//Center center
		break;
		case 5: if ( x+1<CELLROWSPERGRID && y+1<CELLCOLUMNSPERGRID )
			grid->moveAgentCurrentToCandidate(x,y, currentAgent, x+1, y+1);//Inferior right
		break;
		case 6: if ( x+1<CELLROWSPERGRID )
			grid->moveAgentCurrentToCandidate(x,y, currentAgent, x+1, y);//Inferior center
		break;
		case 7: if ( x+1<CELLROWSPERGRID && y-1>0 )
			grid->moveAgentCurrentToCandidate(x,y, currentAgent, x+1, y-1);//Inferior left
		break;
		case 8: if ( y-1>0 )
			grid->moveAgentCurrentToCandidate(x,y, currentAgent, x, y-1);//Center left
		break;
		}
	}
	else{
#ifdef DEBUGCELL
		//   std::cout << "Current agent empty nothing to move \n";
#endif

	}
}

void Cell::resolveHumanHuman(){
	Human* h1;
	Human* h2;
	h1 = dynamic_cast<Human*>(candidateAgent);
	h2 = dynamic_cast<Human*>(currentAgent);
	if ((h1->getGender() && !h2->getGender()) || (!h1->getGender() && h2->getGender())) { //If they are male and woman
		int result = randomObj->getIntUniformRandomBetween(1,100);
		if ( result < BIRTHPERCENTAGE ){ //They have a baby
			bool gender = ( randomObj->getIntUniformRandomBetween(0,1) ? true:false );
			Human baby(gender,false);
			//grid->addAgent(0,0,&baby);
		}
	}
	//Move human
	grid->moveAgentCurrentToCurrent(x,y,currentAgent,3,3);
	currentAgent = candidateAgent;
}
void Cell::resolveHumanZombie(){
	Human* h;
	Zombie* z;
	if ( currentAgent->getType() == zombie ){
		h = dynamic_cast<Human*>(candidateAgent);
		z = dynamic_cast<Zombie*>(currentAgent);
	}
	else{
		h = dynamic_cast<Human*>(currentAgent);
		z = dynamic_cast<Zombie*>(candidateAgent);
	}
	//Probability and cases, for now it is a rand
	int dice_roll = randomObj->getIntUniformRandomBetween(0,100);
	//#ifdef DEBUGCELL
	//std::cout << "Dice roll: " << dice_roll << "\n";
	//#endif

	if ( dice_roll <= HEADSHOTPERCENTAGE ){
		z->shoot();
#ifdef DEBUGCELL
		std::cout << "Zombie shooted " <<"\n";
#endif
		if ( currentAgent->getType() == human ){
#ifdef DEBUGCELL
		std::cout << "Moving Current Human to another cell " <<"\n";
#endif
			grid->moveAgentCurrentToCurrent(x,y,currentAgent,5,5);
#ifdef DEBUGCELL
		std::cout << "Moved" <<"\n";
#endif
		}
		else{
#ifdef DEBUGCELL
		std::cout << "Moving Candidate Human to another cell " <<"\n";
#endif
		    grid->moveAgentCandidateToCurrent(x,y,currentAgent,5,5);
		}
	}
	else if ( dice_roll <= SUCESSFULBITEPERCENTAGE ){
		h->infect();
#ifdef DEBUGCELL
		std::cout << "Human infected " <<"\n";
#endif
		if ( currentAgent->getType() == zombie ){
#ifdef DEBUGCELL
		std::cout << "Moving current zombie to another cell " <<"\n";
#endif
			grid->moveAgentCurrentToCurrent(x,y,currentAgent,6,6);
		}
		else{
#ifdef DEBUGCELL
		std::cout << "Moving candidate zombie to another cell " <<"\n";
#endif
			grid->moveAgentCandidateToCurrent(x,y,currentAgent,7,7);
		}
	}
}
void Cell::resolveZombieZombie(){
	//Move zombie to another location
}

void Cell::setCurrentAgent(Agent* agent){
	currentAgent = agent;
}

void Cell::setCandidateAgent(Agent* agent){
	candidateAgent = agent;
}

Agent* Cell::getCurrentAgent(){
	return currentAgent;
}

Agent* Cell::getCandidateAgent(){
	return candidateAgent;
}
void Cell::setRandomObj(RandomGen* obj){
	delete(randomObj);
	randomObj = obj;
}

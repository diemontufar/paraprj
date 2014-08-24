/*
 * Cell.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Cell.h"
#ifdef DEBUG
#include "iostream"
#endif

Cell::Cell(){
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
void Cell::resolve(){
	if (currentAgent == NULL && candidateAgent == NULL)//Empty cell do nothing
		return;

	if (currentAgent != NULL){//There is someone in the cell, lets check
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
		candidateAgent = NULL;
	}
}

void Cell::resolveHumanHuman(){
	Human* h1;
	Human* h2;
	h1 = dynamic_cast<Human*>(candidateAgent);
	h2 = dynamic_cast<Human*>(currentAgent);
	if ((h1->getGender() && !h2->getGender()) || (!h1->getGender() && h2->getGender())) { //If they are male and woman
		RandomGen random;
		int result = random.getIntUniformRandomBetween(1,100);
		if ( result < BIRTHPERCENTAGE ){ //They have a baby
		   bool gender = ( random.getIntUniformRandomBetween(0,1) ? true:false );
		   Human baby(gender,false);
		   grid->addAgent(0,0,&baby);
		}
	}
	//Move human
	grid->moveAgent(x,y,currentAgent);
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
	RandomGen random;
	int dice_roll = random.getIntUniformRandomBetween(0,100);

	if ( dice_roll <= HEADSHOTPERCENTAGE ){
		z->shoot();
		//Move zombie
	}
	else if ( dice_roll <= SUCESSFULBITEPERCENTAGE ){
		h->infect();
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

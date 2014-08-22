/*
 * Cell.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Cell.h"

Cell::Cell() {
}

Cell::~Cell() {
}

void Cell::resolve(){
	AgentTypeEnum currentType = currentAgent->getType();
	AgentTypeEnum candidateType = currentAgent->getType();
	if ( currentType==human && candidateType == human ){
		resolveHumanHuman();
	}
	else if (currentType==zombie && candidateType == zombie){
		resolveZombieZombie();
	}
	else{
		resolveHumanZombie();
	}
}

void Cell::resolveHumanHuman(){

	//Reglas?
	Human* h1 = dynamic_cast<Human*>(candidateAgent);
}
void Cell::resolveHumanZombie(){

	//Reglas?
	Human* h1 = dynamic_cast<Human*>(candidateAgent);
}
void Cell::resolveZombieZombie(){

	//Reglas?
	Human* h1 = dynamic_cast<Human*>(candidateAgent);
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

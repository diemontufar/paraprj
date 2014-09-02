/*
 * Model.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Model.h"
#ifdef DEBUG
#include <iostream>
#endif

Model::Model() {

}

Model::~Model() {
}

void Model::setup(){
	for ( int i = 0; i < GRIDROWS; i++ ){
		for (int j = 0; j < GRIDCOLUMNS; j++){
			 grids[i][j]=new Grid();
			 grids[i][j]->initialize(0,0);
		}
	}
}
void Model::printState(int tick){
	int humans = 0;
	int zombies = 0;
	for ( int i = 0; i < GRIDROWS; i++ ){
		for (int j = 0; j < GRIDCOLUMNS; j++){
			int hGrid = 0;
			int zGrid = 0;
			grids[i][j]->calculateAgents(hGrid, zGrid);
			humans+=hGrid;
			zombies+=zGrid;
		}
	}
#ifdef DEBUG
	std::cout << "Tick" << tick << " Humans: " << humans << " Zombies " << zombies;
	std::cout << "- Deads: "<< Counters::getInstance().getDead() << " Infected:" << Counters::getInstance().getInfected();
	std::cout << " Converted: "<<Counters::getInstance().getConverted() << " Shooted: " <<Counters::getInstance().getShooted() ;
	std::cout << " Zdead: "<<Counters::getInstance().getZDead();
	std::cout << " OutOfBounds: "<<Counters::getInstance().getOutOfBounds()<<"\n";

#endif
}
void Model::run(){
	printState(0);
    #ifdef DEBUGMODEL
	   std::cout << "Run called"<<"\n";
    #endif

	for ( int i = 0; i < NUMTICKS; i++){
#ifdef DEBUGMODEL
   std::cout << "Iterating ticks "<< i <<"\n";
#endif
		for ( int i = 0; i < GRIDROWS; i++ ){
#ifdef DEBUGMODEL
   std::cout << "Iterating Grd rows: "<< i <<"\n";
#endif
			for (int j = 0; j < GRIDCOLUMNS; j++){
#ifdef DEBUGMODEL
   std::cout << "Iterating Grid columns: "<< j <<"\n";
#endif

				grids[i][j]->step();
			}
		}
		//if ( i%100 == 0 ){
			printState(i);
			Counters::getInstance().resetCounters();
		//}
	}
	printState(NUMTICKS);
}


/*
 * Grid.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Grid.h"
#include <iostream>
using namespace std;
Grid::Grid() {
	cells =  new Cell*[CELLROWSPERGRID];
	for (int i = 0; i < CELLROWSPERGRID; i++){
		cells[i] =  new Cell[CELLCOLUMNSPERGRID];
	}

}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

void Grid::initialize(int nPeople, int nZombies){
	for ( int i = 0; i < CELLROWSPERGRID; i++ ){
		for ( int j = 0; j < CELLCOLUMNSPERGRID; j++ ){
			cells[i][j]=new Cell();
			cells[i][j]->setCurrentAgent(new Human(false,false));
			cout << "i:  "<<i << "j:  "<<j <<"Initialized with humans: "<< endl;
		}
	}
}

void Grid::addAgent(int x, int y, Agent* agent){
	cout << "x:  "<<x << "y:  "<<y<< endl;
}
void Grid::moveAgent(int xcurr, int ycurr, Agent* agent){

}

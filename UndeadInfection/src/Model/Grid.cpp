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
	//cells =  new Cell[CELLROWSPERGRID];
	//for (int i = 0; i < CELLROWSPERGRID; i++){
		//cells[i] =  new Cell[CELLCOLUMNSPERGRID];
	//}
	// TODO Auto-generated constructor stub

}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

void Grid::initialize(int nPeople, int nZombies){
	for ( int i = 0; i < CELLROWSPERGRID; i++ ){
		for ( int j = 0; j < CELLCOLUMNSPERGRID; j++ ){
			cells[i][j]=new Cell();
			cells[i][j]->setGrid(this);

			//todo:change algorithm so it is random and with pop limits
			if (j<CELLCOLUMNSPERGRID/2){
				cells[i][j]->setCurrentAgent(new Human(RandomGenerator::randomBool() ,RandomGenerator::randomBool()));
			}
			cout << "i:  "<<i << "j:  "<<j <<"Initialized with humans: "<< endl;
		}
	}
}


//Todo: check adjacencies and move
void Grid::addAgent(int x, int y, Agent* agent){
	if (cells[x][y]->getCurrentAgent()==nullptr){
		cells[x][y]->setCurrentAgent(agent);
	}
	cout << "addent and agent at: "<<"x:  "<<x << "y:  "<<y<< endl;
}
void Grid::moveAgent(int x, int y, Agent* agent,int xtoMove ,int ytoMove){
	if (cells[xtoMove][ytoMove]->getCurrentAgent()==nullptr){
			cells[xtoMove][ytoMove]->setCurrentAgent(agent);
			cells[x][y]->setCurrentAgent(nullptr);
		}
		cout << "addent and agent at: "<<"x:  "<<x << "y:  "<<y<< endl;
}
void Grid::removeAgent(int x, int y, Agent* agent){
	cells[x][y]->setCurrentAgent(nullptr);
}

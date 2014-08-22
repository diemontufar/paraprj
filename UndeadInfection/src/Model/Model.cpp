/*
 * Model.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Model.h"

Model::Model() {

}

Model::~Model() {
	delete grids;
}

void Model::setup(){
	for ( int i = 0; i < GRIDROWS; i++ ){
		for (int j = 0; j < GRIDCOLUMNS; j++){
			 Grid grid;
			 //Get a rand
			 Human h;
//			 Agent a = new Human();
			// grid.addAgent(5,5,h);
			// grids[i][j] = grid;
		}
	}
}


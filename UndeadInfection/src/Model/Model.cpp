/*
 * Model.cpp
 *
 *  Created on: 21/08/2014
 *      Author: achaves
 */

#include "Model.h"
#include <iostream>

Model::Model() {

}

Model::~Model() {
}

void Model::setup(){
	for ( int i = 0; i < GRIDSPERROW; i++ ){
		for (int j = 0; j < GRIDSPERCOLUMN; j++){
			 grids[i][j]=new Grid();
			 Human *a = new Human(false,false);
			 grids[i][j]->addAgent(i,j,a);
			 Grid grid;
			 //Get a rand
//			 Agent a = new Human();
			// grid.addAgent(5,5,h);
			// grids[i][j] = grid;
		}
	}
}


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
	for ( int i = 0; i < GRIDROWS; i++ ){
		for (int j = 0; j < GRIDCOLUMNS; j++){
			 grids[i][j]=new Grid();
			 grids[i][j]->initialize(0,0);
		}
	}
}


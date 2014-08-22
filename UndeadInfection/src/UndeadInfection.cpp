//============================================================================
// Name        : UndeadInfection.cpp
// Author      : Diego
// Version     :
// Copyright   : copyright 2014
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int main() {


	class Agent{

	public:
		int id;
		int type;

	Agent(int idParam = 100, int typeParam = 200)
		:id(idParam),type(typeParam){

	}

	};



	class Human : public Agent{

	public:
		int age;

	Human(int ageParam = 400)
		:age(ageParam){
	}

	};


	Agent **grid = new Agent *[10] ;

	for ( int i = 0; i < 10; i++ ){
			for (int j = 0; j < 10; j++){
				Human h(5);
				h.id = i*j;
				h.type = i+j;
				grid[i] = new Agent[10];
				grid[i][j] = h;

			}
		}


	for ( int i = 0; i < 10; i++ ){
				for (int j = 0; j < 10; j++){

					Human h = static_cast<Human&>(grid[i][j]);

					cout << "Humano en la posicion: " << i << "," << j << "=" << h.id << endl;
				}
			}

	return 0;
}

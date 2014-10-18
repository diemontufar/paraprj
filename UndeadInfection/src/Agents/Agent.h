/*
 * Agent.h
 * Contains the method the all agents must implement
 *  Created on: 19/08/2014
 *      Authors: Andres Chaves, Gustavo Carrion, Diego Montufar
 */
#ifndef AGENT_H_
#define AGENT_H_

#include "../Parameters.h"
#include <assert.h>
#include <iostream>

enum AgentTypeEnum {human,zombie,none};
class Agent{

private:
	AgentTypeEnum type;
    //Human attr
	bool gender; //True is male, false is female
    int age;
    bool infected; //Whether the human is infected or not
    bool hasAGun; //Whether the human has a gun or not
    bool deadByConversion;
    bool naturalDead;
    short int infectionTime; //How much ticks have passed since the human has been exposed
    short int yearTime; //How much ticks have passed to complete a year
    short int lifeExpectancy; // How much time the human will live
    short int numBabies; // How much time the human will live

    //Zombies attr
    short decompositionTime; //Number of ticks that have passed
    bool shooted;
    bool decomposed;
    short lifeTime;


public:
	Agent();
	void migrateToZombie(int lifeTime); //Constructor for zombies
	void migrateToHuman(bool gender,int age, bool hasAGun, int lifeExpectancy, int numBabies);
	void clean();
	void clone(Agent A);

   ~Agent();

	//Forward the agent one step method
	void step();
	bool getGender();
	int getAge();
	bool isHasAGun();
	bool isInfected();
	bool isNaturalDead();
	void markAsDead();
	void incrementNumBabies();
	int getNumBabies();
	bool isDead();
	void infect();

	bool isDecomposed();
	bool isShooted();
	void shoot();

	AgentTypeEnum getType();

};
#endif /* AGENT_H */

/*
 * Agent.h
 * Contains the method the all agents must implement
 *  Created on: 19/08/2014
 *      Author: achaves
 */
#ifndef AGENT_H_
#define AGENT_H_

#include "../Parameters.h"

enum AgentTypeEnum {human,zombie,typeUndef};
class Agent{

private:
	int id;
	AgentTypeEnum type;

public:

	//Virtual destructor
	Agent();
   ~Agent();

	//Forward the agent one step method
	virtual void step()=0;

	virtual AgentTypeEnum getType()=0;

};
#endif /* AGENT_H */

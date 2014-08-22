/*
 * Agent.h
 * Contains the method the all agents must implement
 *  Created on: 19/08/2014
 *      Author: achaves
 */
#ifndef AGENT_H_
#define AGENT_H_

enum AgentTypeEnum {human,zombie};
class Agent{

private:
	int id;
	AgentTypeEnum type;

public:
	//Virtual constructor
	Agent();

	//Virtual destructor
	~Agent();

	//Forward the agent one step method
	virtual void step();

	virtual AgentTypeEnum getType();

};
#endif /* AGENT_H */

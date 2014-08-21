/*
 * Agent.h
 * Contains the method the all agents must implement
 *  Created on: 19/08/2014
 *      Author: achaves
 */

enum AgentTypeEnum {human,zombie};

class Agent{
private:
	int id;
	AgentTypeEnum type;

public:
	//Virtual constructor
	virtual Agent() {}

	//Virtual destructor
	virtual ~Agent() {}

	//Forward the agent one step method
	virtual void step(){};

	virtual AgentTypeEnum getType(){};

}

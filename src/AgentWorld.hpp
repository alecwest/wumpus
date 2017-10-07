
/* Generated from GenMyModel */

#ifndef DEF_AGENTWORLD
#define DEF_AGENTWORLD

#include "GameWorld.hpp"
#include "World.hpp"

class AgentWorld : public World
{
protected:
	GameWorld gameWorld;
	int worldProbability;
	
public:
	AgentWorld();
	AgentWorld(const GameWorld &gw);
	virtual ~AgentWorld();
	AgentWorld* clone();
	void perceptWorld(int room);

};


#endif

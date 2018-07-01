
/* Generated from GenMyModel */

#ifndef DEF_AGENTWORLD
#define DEF_AGENTWORLD

#include "GameWorld.hpp"
#include "World.hpp"

class AgentWorld : public World {
private:
	GameWorld gameWorld;
	
public:
	AgentWorld();
	AgentWorld(const GameWorld &gw);
	virtual ~AgentWorld();
	std::vector<RoomContent> perceptWorld(int room);
	/*
	 * Returns SUPMUW or WUMPUS if either is shot and BUMP if it hits a wall or BLOCKADE
	 */
	RoomContent agentShot(int shootingRoom, Direction directionShot);
};


#endif

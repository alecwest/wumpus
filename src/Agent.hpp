
/* Generated from GenMyModel */

#ifndef DEF_AGENT
#define DEF_AGENT

#include "AgentWorld.hpp"
#include "WorldEnum.hpp"

class Agent
{
protected:
	AgentWorld world;
	Direction dir;
	ExplorationStats info;
	int room;
	int maxArrows;
	std::vector<int> noWumpusRooms;
	std::vector<int> noSupmuwRooms;
public:
	void turnLeft();
	void turnRight();
	void forward();
	void grab();
	RoomContent shoot();
	Agent();
	Agent(const GameWorld &gw);
	virtual ~Agent();
	int calculateScore();
	void printWorld();
	virtual void makeMove() = 0;
private:
	RoomContent getAgentRoomContent();
};


#endif

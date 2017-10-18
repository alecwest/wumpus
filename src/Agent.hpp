
/* Generated from GenMyModel */

#ifndef DEF_AGENT
#define DEF_AGENT

#include "AgentWorld.hpp"
#include "WorldEnum.hpp"

class Agent
{
private:
	void processPerception(std::vector<RoomContent> rc);
protected:
	AgentWorld world;
	Direction dir;
	ExplorationStats info;
	int room;
	// TODO does turnLeft and turnRight count as a move? Signs point to yes
	void turnLeft();
	void turnRight();
	void forward();
	void grab();
	void exit();
	RoomContent shoot();
public:
	Agent();
	Agent(const GameWorld &gw);
	virtual ~Agent();
	int calculateScore();
	void printWorld();
	void gameOver();
	virtual void makeMove() = 0;
private:
	RoomContent getAgentRoomContent();
};


#endif


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
	int printFrequency;
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
	// 0 = final result only, 1 = final result and text walkthrough, 2 = everything without stopping, 3 = everything with getchar() pause
	void setPrintFrequency(int freq);
	virtual void makeMove() = 0;
private:
	RoomContent getAgentRoomContent();
};


#endif

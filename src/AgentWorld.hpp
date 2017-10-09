
/* Generated from GenMyModel */

#ifndef DEF_AGENTWORLD
#define DEF_AGENTWORLD

#include "AgentRoom.hpp"
#include "GameWorld.hpp"
#include "World.hpp"

class AgentWorld : public World
{
private:
	std::vector<AgentRoom> world;
protected:
	GameWorld gameWorld;
	int worldProbability;
	
public:
	AgentWorld();
	AgentWorld(const GameWorld &gw);
	virtual ~AgentWorld();
	AgentWorld* clone();
	void perceptWorld(int room);
	int adjacentRoom(int room, Direction dir);
	std::vector<int> adjacentRooms(int room);
	std::vector<int> adjacentDiagonalRooms(int room);
	Room getRoom(int room);
	bool roomHasContent(int room, RoomContent rc);
	void addRoomContent(int room, RoomContent rc);
	bool removeRoomContent(int room, RoomContent rc);
	void printWorld();
};


#endif

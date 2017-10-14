
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
//	AgentWorld* clone(); // TODO remove this?
	// RoomContent should include dead agent and regular supmuw and evil supmuw (evil supmuw not necessary if agent is dead?)
	// RoomContent should include a piece of food if supmuw not near wumpus (is normal wumpus) and not in a pit
	// If supmuw is in a pit, then no food will be left out and the pit is harmless
	// If wumpus is in a pit, it is still harmful, and if supmuw smells wumpus while in pit, it becomes harmful as well
	// If supmuw has food, all 8 adjacent rooms are wumpus-free
	std::vector<RoomContent> perceptWorld(int room); // Returns implementation of [none, none, none, none, ...] as described in class powerpoints
	int adjacentRoom(int room, Direction dir);
	std::vector<int> adjacentRooms(int room);
	std::vector<int> adjacentDiagonalRooms(int room);
	std::vector<Inference> getInferences(int room);
	void addInference(int room, Inference i);
	void removeInference(int room, Inference i);
	bool hasInference(int room, Inference i);
	bool safeRoom(int room);
	Room getRoom(int room);
	void setRoomStatus(int room, RoomStatus rs);
	RoomStatus getRoomStatus(int room);
	bool roomHasContent(int room, RoomContent rc);
	bool roomIsEmpty(int room);
	void addRoomContent(int room, RoomContent rc);
	bool removeRoomContent(int room, RoomContent rc);
	void printWorld();
};


#endif

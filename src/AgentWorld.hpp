
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
	
public:
	AgentWorld();
	AgentWorld(const GameWorld &gw);
	virtual ~AgentWorld();
	// Returns collection of physical objects and sensations perceived in this room
	std::vector<RoomContent> perceptWorld(int room);
	// Returns room number for specified direction, or -1 if no room exists
	int adjacentRoom(int room, Direction dir);
	// Returns collection of existing adjacent rooms
	std::vector<int> adjacentRooms(int room);
	// Returns collection of diagonally adjacent rooms
	std::vector<int> adjacentDiagonalRooms(int room);
	// Returns collection of both orthogonal and diagonally adjacent rooms
	std::vector<int> allAdjacentRooms(int room);
	std::vector<Inference> getInferences(int room);
	void addInference(int room, Inference i);
	void removeInference(int room, Inference i);
	bool hasInference(int room, Inference i);
	// Returns true if inferences imply safety in the unvisited room
	bool safeUnvisitedRoom(int room);
	// Returns true if room is safe, but not necessarily visited
	bool safeRoom(int room);
	bool roomVisited(int room);
	Room getRoom(int room);
	int getNumRooms();
	void setRoomStatus(int room, RoomStatus rs);
	RoomStatus getRoomStatus(int room);
	bool roomHasContent(int room, RoomContent rc);
	bool roomBlockaded(int room);
	bool roomIsEmpty(int room);
	// Add content to the agent's perception of the world
	void addRoomContent(int room, RoomContent rc);
	// Remove content from the agent's perception of the world
	bool removeRoomContent(int room, RoomContent rc);
	/*
	 * Returns SUPMUW or WUMPUS if either is shot and BUMP if it hits a wall or BLOCKADE
	 */
	RoomContent agentShot(int shootingRoom, Direction directionShot);
	void printWorld();
};


#endif

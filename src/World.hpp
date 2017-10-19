
/* Generated from GenMyModel */

#ifndef DEF_WORLD
#define DEF_WORLD

#include <iostream>

#include "NullRoom.hpp"
#include "Room.hpp"
#include "WorldEnum.hpp"

class World
{
private:
	std::vector<Room> world; // Will declare vector of GameRooms and AgentRooms in each respective world
protected:
	int gridSize;
	void printDividingLine();

public:
	World();
	virtual ~World();
	int getGridSize();
	int getNumRooms();
	virtual int adjacentRoom(int room, Direction dir) = 0;
	virtual std::vector<int> adjacentRooms(int room) = 0;
	virtual std::vector<int> adjacentDiagonalRooms(int room) = 0; // Primary used for Supmuw's Moo
	virtual std::vector<int> allAdjacentRooms(int room) = 0;
	virtual Room getRoom(int room) = 0;
	virtual bool roomHasContent(int room, RoomContent rc) = 0;
	virtual bool roomBlockaded(int room) = 0;
	virtual void addRoomContent(int room, RoomContent rc) = 0;
	virtual bool removeRoomContent(int room, RoomContent rc) = 0;
	virtual void printWorld() = 0;
};


#endif

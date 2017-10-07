
/* Generated from GenMyModel */

#ifndef DEF_WORLD
#define DEF_WORLD

#include <iostream>

#include "NullRoom.hpp"
#include "Room.hpp"
#include "WorldEnum.hpp"

class World
{
protected:
	int gridSize;
	std::vector<Room> world;
	void printDividingLine();

public:
	World();
	virtual ~World();
	int adjacentRoom(int room, Direction dir);
	std::vector<int> adjacentRooms(int room);
	std::vector<int> adjacentDiagonalRooms(int room); // Primary used for Supmuw's Moo
	Room getRoom(int room);
	bool roomHasContent(int room, RoomContent rc);
	void addRoomContent(int room, RoomContent rc);
	bool removeRoomContent(int room, RoomContent rc);
	void printWorld();
};


#endif

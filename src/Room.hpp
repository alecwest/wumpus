
/* Generated from GenMyModel */

#ifndef DEF_ROOM
#define DEF_ROOM

#include <algorithm>
#include <iostream>
#include <vector>

#include "WorldEnum.hpp"

class Room
{
protected:
	int room;
	int worldSize;
	int numRooms;
	std::vector<RoomContent> content;
		
public:
	Room();
	Room(int room, int worldSize);
	virtual ~Room();
	int getRoom();
	int getRow();
	int getCol();
	int adjacentRoom(Direction dir);
	int adjacentDiagonalRoom(Direction dir); // Primarily used for Supmuw's Moo
	bool hasAdjacent(Direction dir);
	std::vector<RoomContent> getRoomContent();
	void addRoomContent(RoomContent rc);
	bool removeRoomContent(RoomContent rc); // return true if successful
	bool hasContent(RoomContent rc);
	bool roomEmpty();
};


#endif

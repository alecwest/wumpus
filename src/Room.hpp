
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
	RoomStatus status;
	std::vector<Inference> inferences;
		
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
	RoomStatus getRoomStatus();
	void setRoomStatus(RoomStatus rs);
	bool roomBlockaded();
	bool roomEmpty();
	Room* clone();
	std::vector<Inference> getInferences();
	/*
	 * If a free inference is added, remove the corresponding contains inferences if it exists
	 * If a contains inference is added, but it's corresponding free inference is already here, ignore it
	 */
	void addInference(Inference i);
	void removeInference(Inference i);
	bool hasInference(Inference i);
	/*
	 * Returns true if room is pit free, wumpus free, and evil supmuw free and NOT yet visited
	 */
	bool safeUnvisitedRoom();
	/*
	 * Returns true if room is pit free, wumpus free, and evil supmuw free. Visited doesn't matter
	 */
	bool safeRoom();
	/*
	 * Returns true if room is visited
	 */
	bool roomVisited();
	/*
	 * Returns true if room is not blocked
	 */
	bool canEnterRoom();
};


#endif

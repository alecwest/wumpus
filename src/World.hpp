
/* Generated from GenMyModel */

#ifndef DEF_WORLD
#define DEF_WORLD

#include <iostream>

#include "Room.hpp"
#include "WorldEnum.hpp"

class World {
protected:
	std::vector<Room> world; // Will declare vector of GameRooms and AgentRooms in each respective world
	static const int ROOM_WIDTH = 8;
	int gridSize;
	void printDividingLine();
	std::string stretchRoomRow(std::string roomRow);
	void printTopRoomRow(int firstRoomInRow);
	void printMiddleRoomRow(int firstRoomInRow);
	void printBottomRoomRow(int firstRoomInRow);

public:
	World();
	virtual ~World();
	int getGridSize();
	int getNumRooms();
	RoomStatus getRoomStatus(int room);
	void setRoomStatus(int room, RoomStatus rs);
	void printWorld();
	Room getRoom(int room);
	int getAdjacentRoomNumber(int room, Direction dir);
	std::vector<int> getAdjacentRoomNumbers(int room);
	std::vector<int> getAdjacentDiagonalRoomNumbers(int room); // Primary used for Supmuw's Moo
	std::vector<int> getAllAdjacentRoomNumbers(int room);
	bool roomHasContent(int room, RoomContent rc);
	bool roomBlockaded(int room);
	bool roomIsEmpty(int room);
	void addRoomContent(int room, RoomContent rc);
	bool removeRoomContent(int room, RoomContent rc);
	std::vector<Inference> getInferences(int room);
	void addInference(int room, Inference i);
	void removeInference(int room, Inference i);
	bool hasInference(int room, Inference i);
	bool safeUnvisitedRoom(int room);
	bool safeRoom(int room);
	bool roomVisited(int room);

};


#endif

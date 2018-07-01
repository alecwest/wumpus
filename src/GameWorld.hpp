
/* Generated from GenMyModel */

#ifndef DEF_GAMEWORLD
#define DEF_GAMEWORLD

#include <fstream>
#include <string.h>

#include "World.hpp"
#include "WorldEnum.hpp"

class GameWorld : public World
{
private:
	// Return a random location that does not interfere with the safe space
	int getRandomLocationForObstacle();
	void removeRoomContentAndDependents(int room, RoomContent rc, RoomContent dep);
public:
	GameWorld();
	GameWorld(std::string fileName);
	virtual ~GameWorld();
	void addToRoom(int room, RoomContent rc);
	int getAdjacentRoomNumber(int room, Direction dir);
	std::vector<int> adjacentRooms(int room);
	std::vector<int> adjacentDiagonalRooms(int room);
	std::vector<int> allAdjacentRooms(int room);
	bool roomHasContent(int room, RoomContent rc);
	bool roomBlockaded(int room);
	void addRoomContent(int room, RoomContent rc);
	bool removeRoomContent(int room, RoomContent rc);
private:
	void addToAdjacentRooms(int room, RoomContent rc);
	void addToAdjacentDiagonalRooms(int room, RoomContent rc);
};


#endif


/* Generated from GenMyModel */

#ifndef DEF_GAMEWORLD
#define DEF_GAMEWORLD

#include <fstream>
#include <string.h>

#include "World.hpp"
#include "WorldEnum.hpp"

class GameWorld : public World {
private:
	// Return a random location that does not interfere with the safe space
	int getRandomLocationForObstacle();
	void removeRoomContentAndDependents(int room, RoomContent rc, RoomContent dep);

public:
	GameWorld();
	GameWorld(std::string fileName);
	virtual ~GameWorld();
	void addToRoom(int room, RoomContent rc);

private:
	void addToAdjacentRooms(int room, RoomContent rc);
	void addToAdjacentDiagonalRooms(int room, RoomContent rc);
};


#endif

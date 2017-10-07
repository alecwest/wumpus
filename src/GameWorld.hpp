
/* Generated from GenMyModel */

#ifndef DEF_GAMEWORLD
#define DEF_GAMEWORLD

#include <fstream>
#include <string.h>

#include "GameRoom.hpp"
#include "World.hpp"
#include "WorldEnum.hpp"

class GameWorld : public World
{
protected:
	int numGold;
	int numSupmuw;
	int numWumpus;
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

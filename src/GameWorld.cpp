
/* Generated from GenMyModel */

#include "GameWorld.hpp"

GameWorld::GameWorld() {
// TODO World should generate a blank world
// TODO GameWorld should place random content into that blank world
}

GameWorld::GameWorld(std::string fileName) {
	int locX;
	int locY;
	std::ifstream file;
	std::string line;
	file.open(fileName.c_str());
	if (!file.is_open()) {
		std::cout << "Could not open file";
	}

	// Init values
	numGold = 0;
	numSupmuw = 0;
	numWumpus = 0;

	// Get worldSize
	getline(file, line);
	worldSize = atoi(line.c_str());
	numRooms = worldSize * worldSize;
	world = new GameRoom[numRooms];
	for (int i = 0; i < numRooms; i++) {
		world[i] = GameRoom(i, worldSize);
	}

	// TODO needs to check for proper format (Content\nX-coord\nY-coord)
	while (getline(file, line)) {
		std::string content = line;
		getline(file, line);
		locX = atoi(line.c_str());
		getline(file, line);
		locY = atoi(line.c_str());

		if (locX == 0 && locY == 0) {
			continue; // Skip any that try to place something in the first square
		}
		else if (content.find("Pit") != std::string::npos) {
			addToRoom(locX * worldSize + locY, RoomContent::PIT);
		}
		else if (content.find("Wumpus") != std::string::npos) {
			addToRoom(locX * worldSize + locY, RoomContent::WUMPUS);
			numWumpus++;
		}
		else if (content.find("Gold") != std::string::npos) {
			addToRoom(locX * worldSize + locY, RoomContent::GOLD);
			numGold++;
		}
		else if (content.find("Supmuw") != std::string::npos) {
			addToRoom(locX * worldSize + locY, RoomContent::SUPMUW);
			numSupmuw++;
		}
	}

	file.close();
}

GameWorld::~GameWorld() {
	// TODO make sure ~World does this
	delete [] world;
}

void GameWorld::addToRoom(int room, RoomContent rc) {

	switch(rc) {
	case RoomContent::GOLD:
		world[room].addRoomContent(RoomContent::GLITTER);
		break;
	case RoomContent::PIT:
		addToAdjacentRooms(room, RoomContent::BREEZE);
		break;
	case RoomContent::SUPMUW:
		addToAdjacentRooms(room, RoomContent::MOO);
		addToAdjacentDiagonalRooms(room, RoomContent::MOO);
		break;
	case RoomContent::WUMPUS:
		addToAdjacentRooms(room, RoomContent::STENCH);
		break;
	default:
		break;
	}
	world[room].addRoomContent(rc);
}

void GameWorld::addToAdjacentRooms(int room, RoomContent rc) {
	std::vector<int> rooms = adjacentRooms(room);
	for (int r : rooms) {
		addToRoom(r, rc);
	}
}

void GameWorld::addToAdjacentDiagonalRooms(int room, RoomContent rc) {
	std::vector<int> rooms = adjacentDiagonalRooms(room);
	for (int r : rooms) {
		addToRoom(r, rc);
	}
}

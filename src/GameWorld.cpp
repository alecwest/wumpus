
/* Generated from GenMyModel */

#include "GameWorld.hpp"

GameWorld::GameWorld() : World() {
	// Init values
	numGold = 0;
	numSupmuw = 0;
	numWumpus = 0;
}

GameWorld::GameWorld(std::string fileName) : World() {
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

	// Get gridSize
	getline(file, line);
	gridSize = atoi(line.c_str());
	for (unsigned int i = 0; i < gridSize * gridSize; i++) {
		world.push_back(GameRoom(i, gridSize));
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
			addToRoom(locX * gridSize + locY, RoomContent::PIT);
		}
		else if (content.find("Wumpus") != std::string::npos) {
			addToRoom(locX * gridSize + locY, RoomContent::WUMPUS);
			numWumpus++;
		}
		else if (content.find("Gold") != std::string::npos) {
			addToRoom(locX * gridSize + locY, RoomContent::GOLD);
			numGold++;
		}
		else if (content.find("Supmuw") != std::string::npos) {
			addToRoom(locX * gridSize + locY, RoomContent::SUPMUW);
			numSupmuw++;
		}
	}

	file.close();
}

GameWorld::~GameWorld() {}

void GameWorld::addToRoom(int room, RoomContent rc) {

	switch(rc) {
	case RoomContent::GOLD:
		world.at(room).addRoomContent(RoomContent::GLITTER);
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
	world.at(room).addRoomContent(rc);
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

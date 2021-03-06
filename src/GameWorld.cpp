
/* Generated from GenMyModel */

#include "GameWorld.hpp"

GameWorld::GameWorld() : World() {
	// Default grid size is 10
	gridSize = 10;
	for (int i = 0; i < gridSize * gridSize; i++) {
		world.push_back(Room(i, gridSize));
	}

	// Generate random pit and blockade rooms
	int chance;
	for (auto w : world) {
		// Don't generate any pits in safe room or it's adjacent rooms
		if (w.getRoom() == 0 || w.getRoom() == 1 || w.getRoom() == gridSize || w.getRoom() == gridSize + 1) continue;
		chance = std::rand() % 100;
		if (chance < 15) {
			addToRoom(w.getRoom(), RoomContent::PIT);
		}
		if (chance > 85) {
			addToRoom(w.getRoom(), RoomContent::BLOCKADE);
		}
	}

	// Generate random Wumpus, Supmuw, and Gold location
	int wumpusLoc = getRandomLocationForObstacle();
	int supmuwLoc = getRandomLocationForObstacle();
	int goldLoc = getRandomLocationForObstacle();
	addToRoom(wumpusLoc, RoomContent::WUMPUS);
	addToRoom(supmuwLoc, RoomContent::SUPMUW);
	addToRoom(goldLoc, RoomContent::GOLD);
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

	// Get gridSize
	getline(file, line);
	gridSize = atoi(line.c_str());
	for (int i = 0; i < gridSize * gridSize; i++) {
		world.push_back(Room(i, gridSize));
	}

	while (getline(file, line)) {
		std::string content = line;
		getline(file, line);
		locX = atoi(line.c_str());
		getline(file, line);
		locY = atoi(line.c_str());

		if ((locX == 0 && locY == 0)
				|| (locX == 0 && locY == 1)
				|| (locX == 1 && locY == 0)
				|| (locX == 1 && locY == 1)) {
			continue; // Skip any that try to place something in or adjacent to the safe square
		}
		else if (content.find("blockade") != std::string::npos) {
			addToRoom(locX * gridSize + locY, RoomContent::BLOCKADE);
		}
		else if (content.find("pit") != std::string::npos) {
			addToRoom(locX * gridSize + locY, RoomContent::PIT);
		}
		else if (content.find("wumpus") != std::string::npos) {
			addToRoom(locX * gridSize + locY, RoomContent::WUMPUS);
		}
		else if (content.find("gold") != std::string::npos) {
			addToRoom(locX * gridSize + locY, RoomContent::GOLD);
		}
		else if (content.find("supmuw") != std::string::npos) {
			addToRoom(locX * gridSize + locY, RoomContent::SUPMUW);
		}
	}

	file.close();
}

GameWorld::~GameWorld() {}

int GameWorld::getRandomLocationForObstacle() {
	int randLoc = std::rand() % getNumRooms();
	while (randLoc == 0 || randLoc == 1 || randLoc == gridSize || randLoc == gridSize + 1) {
		randLoc = std::rand() % getNumRooms();
	}
	return randLoc;
}

void GameWorld::addToRoom(int room, RoomContent rc) {
	if (room < 0 || room > (int)world.size() || roomHasContent(room, RoomContent::BLOCKADE)) {
		return; // Ignore attempts to place content outside of boundaries or in blockades
	}
	std::vector<int> adjRooms = getAdjacentRoomNumbers(room);
	std::vector<int> allRooms = getAllAdjacentRoomNumbers(room);

	switch(rc) {
	case RoomContent::GOLD:
		addRoomContent(room, RoomContent::GLITTER);
		break;
	case RoomContent::PIT:
		if (roomHasContent(room, RoomContent::SUPMUW)) {
			removeRoomContent(room, RoomContent::FOOD);
		}
		if (roomHasContent(room, RoomContent::WUMPUS)) {
			return; // pit cannot be placed in the same location as a Wumpus
		}
		addToAdjacentRooms(room, RoomContent::BREEZE);
		break;
	case RoomContent::SUPMUW:
		// If WUMPUS is nearby, change rc to SUPMUW_EVIL
		for (auto r : allRooms) {
			if (roomHasContent(r, RoomContent::WUMPUS)) {
				rc = RoomContent::SUPMUW_EVIL;
				break;
			}
		}
		// If SUPMUW is not evil and there is no pit in this space, it will share FOOD
		if (rc != RoomContent::SUPMUW_EVIL && !roomHasContent(room, RoomContent::PIT)) {
			addRoomContent(room, RoomContent::FOOD);
		}
		addToAdjacentRooms(room, RoomContent::MOO);
		addToAdjacentDiagonalRooms(room, RoomContent::MOO);
		break;
	case RoomContent::WUMPUS:
		// If SUPMUW is nearby, turn it evil and remove any food it may have been offering
		for (auto r : allRooms) {
			if (roomHasContent(r, RoomContent::SUPMUW)) {
				removeRoomContent(r, RoomContent::SUPMUW);
				removeRoomContent(r, RoomContent::FOOD);
				addRoomContent(r, RoomContent::SUPMUW_EVIL);
			}
		}
		// Wumpus addition trumps pit addition
		removeRoomContentAndDependents(room, RoomContent::PIT, RoomContent::BREEZE);
		addToAdjacentRooms(room, RoomContent::STENCH);
		break;
	case RoomContent::BLOCKADE:
		removeRoomContentAndDependents(room, RoomContent::PIT, RoomContent::BREEZE);
		removeRoomContentAndDependents(room, RoomContent::WUMPUS, RoomContent::STENCH);
		removeRoomContentAndDependents(room, RoomContent::SUPMUW, RoomContent::MOO);
		removeRoomContentAndDependents(room, RoomContent::SUPMUW_EVIL, RoomContent::MOO);
		removeRoomContent(room, RoomContent::FOOD);
		removeRoomContent(room, RoomContent::GLITTER);
		removeRoomContent(room, RoomContent::GOLD);
		addRoomContent(room, RoomContent::BLOCKADE);
		break;
	default:
		break;
	}
	world.at(room).addRoomContent(rc);
}

void GameWorld::removeRoomContentAndDependents(int room, RoomContent rc, RoomContent dep) {
	std::vector<int> adjRooms;
	if (dep == RoomContent::MOO) adjRooms = getAllAdjacentRoomNumbers(room);
	else adjRooms = getAdjacentRoomNumbers(room);
	// If rc exists in this room, remove it and any adjacency only associated with it.
	if (roomHasContent(room, rc)) {
		removeRoomContent(room, rc);
		for (auto r : adjRooms) {
			bool keepDep = false;
			for (auto ar : getAdjacentRoomNumbers(r)) {
				if (roomHasContent(ar, dep)) {
					keepDep = true;
					break;
				}
			}
			if (not keepDep) removeRoomContent(r, dep);
		}
	}
}

void GameWorld::addToAdjacentRooms(int room, RoomContent rc) {
	std::vector<int> rooms = getAdjacentRoomNumbers(room);
	for (int r : rooms) {
		addToRoom(r, rc);
	}
}

void GameWorld::addToAdjacentDiagonalRooms(int room, RoomContent rc) {
	std::vector<int> rooms = getAdjacentDiagonalRoomNumbers(room);
	for (int r : rooms) {
		addToRoom(r, rc);
	}
}

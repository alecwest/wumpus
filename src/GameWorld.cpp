
/* Generated from GenMyModel */

#include "GameWorld.hpp"

GameWorld::GameWorld() : World() {
	// Default grid size is 10
	gridSize = 10;
	for (int i = 0; i < gridSize * gridSize; i++) {
		world.push_back(GameRoom(i, gridSize));
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
		world.push_back(GameRoom(i, gridSize));
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
	std::vector<int> adjRooms = adjacentRooms(room);
	std::vector<int> allRooms = allAdjacentRooms(room);

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
	if (dep == RoomContent::MOO) adjRooms = allAdjacentRooms(room);
	else adjRooms = adjacentRooms(room);
	// If rc exists in this room, remove it and any adjacency only associated with it.
	if (roomHasContent(room, rc)) {
		removeRoomContent(room, rc);
		for (auto r : adjRooms) {
			bool keepDep = false;
			for (auto ar : adjacentRooms(r)) {
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

int GameWorld::adjacentRoom(int room, Direction dir) {
	return world.at(room).adjacentRoom(dir);
}

std::vector<int> GameWorld::adjacentRooms(int room) {
	std::vector<int> rooms = std::vector<int>();
	int northRoom = world.at(room).adjacentRoom(Direction::NORTH);
	int eastRoom = 	world.at(room).adjacentRoom(Direction::EAST);
	int southRoom = world.at(room).adjacentRoom(Direction::SOUTH);
	int westRoom = world.at(room).adjacentRoom(Direction::WEST);
	if (northRoom > -1){
		rooms.push_back(northRoom);
	}
	if (eastRoom > -1){
		rooms.push_back(eastRoom);
	}
	if (southRoom > -1){
		rooms.push_back(southRoom);
	}
	if (westRoom > -1){
		rooms.push_back(westRoom);
	}
	return rooms;
}

std::vector<int> GameWorld::adjacentDiagonalRooms(int room) {
	std::vector<int> rooms = std::vector<int>();
	int northRoom = world.at(room).adjacentRoom(Direction::NORTH);
	int southRoom = world.at(room).adjacentRoom(Direction::SOUTH);
	if (northRoom > -1) {
		int northEastRoom = world.at(northRoom).adjacentRoom(Direction::EAST);
		int northWestRoom = world.at(northRoom).adjacentRoom(Direction::WEST);
		if (northEastRoom > -1) {
			rooms.push_back(northEastRoom);
		}
		if (northWestRoom > -1) {
			rooms.push_back(northWestRoom);
		}
	}
	if (southRoom > -1) {
		int southEastRoom = world.at(southRoom).adjacentRoom(Direction::EAST);
		int southWestRoom = world.at(southRoom).adjacentRoom(Direction::WEST);
		if (southEastRoom > -1) {
			rooms.push_back(southEastRoom);
		}
		if (southWestRoom > -1) {
			rooms.push_back(southWestRoom);
		}
	}
	return rooms;
}

std::vector<int> GameWorld::allAdjacentRooms(int room) {
	std::vector<int> adjRooms = adjacentRooms(room);
	std::vector<int> diagRooms = adjacentDiagonalRooms(room);
	std::vector<int> allRooms;
	allRooms.reserve( adjRooms.size() + diagRooms.size() ); // preallocate memory
	allRooms.insert( allRooms.end(), adjRooms.begin(), adjRooms.end() );
	allRooms.insert( allRooms.end(), diagRooms.begin(), diagRooms.end() );
	return allRooms;
}

Room GameWorld::getRoom(int room) {
	if (room >= 0 && room < (int) world.size()) {
		return world.at(room);
	}
	else {
		return Room();
	}
}

int GameWorld::getNumRooms() {
	return world.size();
}

bool GameWorld::roomHasContent(int room, RoomContent rc) {
	return getRoom(room).hasContent(rc);
}

bool GameWorld::roomBlockaded(int room) {
	return getRoom(room).roomBlockaded();
}

void GameWorld::addRoomContent(int room, RoomContent rc) {
	world.at(room).addRoomContent(rc);
}

bool GameWorld::removeRoomContent(int room, RoomContent rc) {
	return world.at(room).removeRoomContent(rc);
}

void GameWorld::printWorld() {
	if (world.size() < 0 || gridSize < 0){
		std::cout << "Could not print this world!";
		return;
	}

	printDividingLine();
	for (int i = world.size() - gridSize; i >= 0; i -= gridSize) {
		// First line prints physical objects
		for (int j = 0; j < gridSize; j++) {
			if (roomBlockaded(i + j)) {
				std::cout << "|XXXXXXXX";
			}
			else {

				std::cout << "|" <<
							 (roomHasContent(i + j, RoomContent::GOLD) ? "G " : "  ") <<
							 (roomHasContent(i + j, RoomContent::PIT) ? "P " : "  ") <<
							 (roomHasContent(i + j, RoomContent::WUMPUS) ? "W " : "  ");
				if(roomHasContent(i + j, RoomContent::SUPMUW)) {
					std::cout << "S" <<
								 (roomHasContent(i + j, RoomContent::FOOD) ? "F" : " ");
					// Food should only exist in room if SUPMUW does
				}
				else if (roomHasContent(i + j, RoomContent::SUPMUW_EVIL)) {
					std::cout << "E ";
				}
				else {
					std::cout << "  ";
				}
			}
		}
		std::cout << "|" << std::endl;
		// Second line prints sensations
		for (int j = 0; j < gridSize; j++) {
			if (roomBlockaded(i + j)) {
				std::cout << "|XXXXXXXX";
			}
			else {
				std::cout << "|" <<
							(world.at(i + j).hasContent(RoomContent::BREEZE) ? " B" : "  ") <<
							(world.at(i + j).hasContent(RoomContent::GLITTER) ? " G" : "  ") <<
							(world.at(i + j).hasContent(RoomContent::MOO) ? " M" : "  ") <<
							(world.at(i + j).hasContent(RoomContent::STENCH) ? " S" : "  ");
			}
		}
		std::cout << "|" << std::endl;
		// Third line prints the agent
		for (int j = 0; j < gridSize; j++) {
			std::cout << "|";
			if (roomBlockaded(i + j)) {
				std::cout << "XXXXXXXX";
			}
			else {
				if (world.at(i + j).hasContent(RoomContent::AGENT_NORTH)) {
					std::cout << "   ^^   ";
				}
				else if (world.at(i + j).hasContent(RoomContent::AGENT_EAST)) {
					std::cout << "   >>   ";
				}
				else if (world.at(i + j).hasContent(RoomContent::AGENT_SOUTH)) {
					std::cout << "   vv   ";
				}
				else if (world.at(i + j).hasContent(RoomContent::AGENT_WEST)) {
					std::cout << "   <<   ";
				}
				else {
					std::cout << "        ";
				}
			}
		}
		std::cout << "|" << std::endl;

		printDividingLine();
	}
}

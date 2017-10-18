
/* Generated from GenMyModel */

#include "GameWorld.hpp"

GameWorld::GameWorld() : World() {
	// Init values
	numGold = 0;
	numSupmuw = 0;
	numWumpus = 0;

	// Default grid size is 10
	gridSize = 10;
	for (unsigned int i = 0; i < gridSize * gridSize; i++) {
		world.push_back(GameRoom(i, gridSize));
	}

	// Generate random pit rooms
	int pitChance;
	for (auto w : world) {
		// Don't generate any pits in safe room or it's adjacent rooms
		if (w.getRoom() == 0 || w.getRoom() == 1 || w.getRoom() == gridSize || w.getRoom() == gridSize + 1) continue;
		pitChance = std::rand() % 100;
		if (pitChance < 15) {
			addToRoom(w.getRoom(), RoomContent::PIT);
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

	while (getline(file, line)) {
		std::string content = line;
		getline(file, line);
		locX = atoi(line.c_str());
		getline(file, line);
		locY = atoi(line.c_str());

		// TODO assume that pit, wumpus, gold, and supmuw cannot be placed in lower right 2x2, to prevent impossible game or too-easy game
		if ((locX == 0 && locY == 0)
				|| (locX == 0 && locY == 1)
				|| (locX == 1 && locY == 0)
				|| (locX == 1 && locY == 1)) {
			continue; // Skip any that try to place something in or adjacent to the safe square
		}
		else if (content.find("Pit") != std::string::npos) {
//			std::cout << "Testing:: adding pit to (" << locX << ", " << locY << ")\n";
			addToRoom(locX * gridSize + locY, RoomContent::PIT);
		}
		else if (content.find("Wumpus") != std::string::npos) {
//			std::cout << "Testing:: adding wumpus to (" << locX << ", " << locY << ")\n";
			addToRoom(locX * gridSize + locY, RoomContent::WUMPUS);
		}
		else if (content.find("Gold") != std::string::npos) {
//			std::cout << "Testing:: adding gold to (" << locX << ", " << locY << ")\n";
			addToRoom(locX * gridSize + locY, RoomContent::GOLD);
		}
		else if (content.find("Supmuw") != std::string::npos) {
//			std::cout << "Testing:: adding supmuw to (" << locX << ", " << locY << ")\n";
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
	if (room < 0 || room > (int)world.size()) {
		return; // Ignore attempts to place content outside of boundaries
	}
	std::vector<int> adjRooms = adjacentRooms(room);
	std::vector<int> diagRooms = adjacentDiagonalRooms(room);
	std::vector<int> allRooms;
	allRooms.reserve( adjRooms.size() + diagRooms.size() ); // preallocate memory
	allRooms.insert( allRooms.end(), adjRooms.begin(), adjRooms.end() );
	allRooms.insert( allRooms.end(), diagRooms.begin(), diagRooms.end() );
	switch(rc) {
	case RoomContent::GOLD:
		addRoomContent(room, RoomContent::GLITTER);
		numGold++;
		break;
	case RoomContent::PIT:
		if (roomHasContent(room, RoomContent::SUPMUW)) {
			removeRoomContent(room, RoomContent::FOOD);
		}
		if (roomHasContent(room, RoomContent::WUMPUS)) {
			return; // TODO Assuming pit cannot be placed in the same location as a Wumpus
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
		numSupmuw++;
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
		// If a PIT exists in this room, remove it and any BREEZE only associated with it.
		if (roomHasContent(room, RoomContent::PIT)) {
			// TODO assuming Wumpus addition trumps pit addition
			removeRoomContent(room, RoomContent::PIT);
			for (auto r : adjRooms) {
				bool keepBreeze = false;
				for (auto ar : adjacentRooms(r)) {
					if (roomHasContent(ar, RoomContent::PIT)) {
						keepBreeze = true;
						break;
					}
				}
				if (not keepBreeze) removeRoomContent(r, RoomContent::BREEZE);
			}
		}
		addToAdjacentRooms(room, RoomContent::STENCH);
		numWumpus++;
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
			std::cout << "|" <<
						 (world.at(i + j).hasContent(RoomContent::GOLD) ? "G " : "  ") <<
						 (world.at(i + j).hasContent(RoomContent::PIT) ? "P " : "  ") <<
					     (world.at(i + j).hasContent(RoomContent::WUMPUS) ? "W " : "  ") <<
					     (world.at(i + j).hasContent(RoomContent::SUPMUW) ? "S " : "  ");
		}
		std::cout << "|" << std::endl;
		// Second line prints sensations
		for (int j = 0; j < gridSize; j++) {
			std::cout << "|" <<
						 (world.at(i + j).hasContent(RoomContent::BREEZE) ? " B" : "  ") <<
						 (world.at(i + j).hasContent(RoomContent::GLITTER) ? " G" : "  ") <<
						 (world.at(i + j).hasContent(RoomContent::MOO) ? " M" : "  ") <<
						 (world.at(i + j).hasContent(RoomContent::STENCH) ? " S" : "  ");
		}
		std::cout << "|" << std::endl;
		// Third line prints the agent
		for (int j = 0; j < gridSize; j++) {
			std::cout << "|";
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
		std::cout << "|" << std::endl;

		printDividingLine();
	}
}

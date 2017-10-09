
/* Generated from GenMyModel */

#include "GameWorld.hpp"

GameWorld::GameWorld() : World() {
	// Init values
	numGold = 0;
	numSupmuw = 0;
	numWumpus = 0;
	std::cout << "Testing:: Why am I in default GameWorld constructor???\n";
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
	std::cout << "Testing:: just loaded rooms into world\n";
	std::cout << "Testing:: world has " << world.size() << " rooms\n";

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
			std::cout << "Testing:: adding pit to (" << locX << ", " << locY << ")\n";
			addToRoom(locX * gridSize + locY, RoomContent::PIT);
		}
		else if (content.find("Wumpus") != std::string::npos) {
			std::cout << "Testing:: adding wumpus to (" << locX << ", " << locY << ")\n";
			addToRoom(locX * gridSize + locY, RoomContent::WUMPUS);
			numWumpus++;
		}
		else if (content.find("Gold") != std::string::npos) {
			std::cout << "Testing:: adding gold to (" << locX << ", " << locY << ")\n";
			addToRoom(locX * gridSize + locY, RoomContent::GOLD);
			numGold++;
		}
		else if (content.find("Supmuw") != std::string::npos) {
			std::cout << "Testing:: adding supmuw to (" << locX << ", " << locY << ")\n";
			addToRoom(locX * gridSize + locY, RoomContent::SUPMUW);
			numSupmuw++;
		}
	}

	file.close();
}

GameWorld::~GameWorld() {}

void GameWorld::addToRoom(int room, RoomContent rc) {
	// TODO add a check to make sure we aren't adding to rooms outside of the worldSize
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
	std::cout << "Adding to room " << room << " in a world with " << world.size() << " rooms.\n";
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
	std::cout << "Trying to get adjacent rooms to " << room << " in a world with " << world.size() << " rooms.\n";
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

Room GameWorld::getRoom(int room) {
	if (room >= 0 && room < (int) world.size()) {
		return world.at(room);
	}
	else {
		return Room();
	}
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
	// TODO is this the best way to prevent printing?
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

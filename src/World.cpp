
/* Generated from GenMyModel */

#include "World.hpp"

World::World() {
	gridSize = -1;
}

World::~World() {}

void World::printDividingLine() {
	for (int i = 0; i < gridSize; i++) {
		std::cout << " --------";
	}
	std::cout << std::endl;
}

int World::adjacentRoom(int room, Direction dir) {
	return world.at(room).adjacentRoom(dir);
}

std::vector<int> World::adjacentRooms(int room) {
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

std::vector<int> World::adjacentDiagonalRooms(int room) {
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

Room World::getRoom(int room) {
	if (room >= 0 && room < world.size()) {
		return world.at(room);
	}
	else {
		return Room();
	}
}

bool World::roomHasContent(int room, RoomContent rc) {
	return getRoom(room).hasContent(rc);
}

void World::addRoomContent(int room, RoomContent rc) {
	world.at(room).addRoomContent(rc);
}

bool World::removeRoomContent(int room, RoomContent rc) {
	return world.at(room).removeRoomContent(rc);
}

void World::printWorld() {
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


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

int World::getGridSize() {
	return gridSize;
}

std::string World::stretchRoomRow(std::string roomRow) {
	while(roomRow.length() < ROOM_WIDTH) {
		if(roomRow.length() % 2 == 0) {
			roomRow.insert(0, " ");
		} else {
			roomRow.insert(roomRow.length() - 1, " ");
		}
	}
	return roomRow;
}

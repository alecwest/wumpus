
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

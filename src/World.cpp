
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

int World::getNumRooms() {
	return world.size();
}

void World::printWorld() {
	std::string row;
	std::string roomRow;
	if (world.size() < 0 || gridSize < 0){
		std::cout << "Could not print this world!";
		return;
	}
	printDividingLine();
	for (int i = world.size() - gridSize; i >= 0; i -= gridSize) {
		printTopRoomRow(i);
		printMiddleRoomRow(i);
		printBottomRoomRow(i);
		printDividingLine();
	}
}

void World::printTopRoomRow(int firstRoomInRow) {
	std::string row = "";
	std::string roomRow = "";
	// First line prints physical objects
	for (int j = firstRoomInRow; j < firstRoomInRow + gridSize; j++) {
		roomRow = "";
		if (roomBlockaded(j)) {
			roomRow += GetRoomContentStringMap(RoomContent::BLOCKADE);
		}
		else {
			roomRow += (roomHasContent(j, RoomContent::GOLD) ? GetRoomContentStringMap(RoomContent::GOLD) : "") +
					(roomHasContent(j, RoomContent::PIT) ? GetRoomContentStringMap(RoomContent::PIT) : "") +
					(roomHasContent(j, RoomContent::WUMPUS) ? GetRoomContentStringMap(RoomContent::WUMPUS) : "");
			if(roomHasContent(j, RoomContent::SUPMUW)) {
				roomRow += GetRoomContentStringMap(RoomContent::SUPMUW) +
						(roomHasContent(j, RoomContent::FOOD) ? GetRoomContentStringMap(RoomContent::FOOD) : "");
				// Food should only exist in room if SUPMUW does
			}
			else if (roomHasContent(j, RoomContent::SUPMUW_EVIL)) {
				roomRow += GetRoomContentStringMap(RoomContent::SUPMUW_EVIL);
			}
		}
		roomRow = "|" + stretchRoomRow(roomRow);
		row += roomRow;
	}
	std::cout << row << "|" << std::endl;
}

void World::printMiddleRoomRow(int firstRoomInRow) {
	std::string row = "";
	std::string roomRow = "";
	// Second line prints sensations
	for (int j = firstRoomInRow; j < firstRoomInRow + gridSize; j++) {
		roomRow = "";
		if (roomBlockaded(j)) {
			roomRow += GetRoomContentStringMap(RoomContent::BLOCKADE);
		}
		else {
			roomRow += (roomHasContent(j, RoomContent::BREEZE) ? GetRoomContentStringMap(RoomContent::BREEZE) : "") +
					(roomHasContent(j, RoomContent::GLITTER) ? GetRoomContentStringMap(RoomContent::GLITTER) : "") +
					(roomHasContent(j, RoomContent::MOO) ? GetRoomContentStringMap(RoomContent::MOO) : "") +
					(roomHasContent(j, RoomContent::STENCH) ? GetRoomContentStringMap(RoomContent::STENCH) : "");
		}
		roomRow = "|" + stretchRoomRow(roomRow);
		row += roomRow;
	}
	std::cout << row << "|" << std::endl;
}

void World::printBottomRoomRow(int firstRoomInRow) {
	RoomStatus rs;
	std::string row = "";
	std::string roomRow = "";
	// Third line prints the agent and the room status
	for (int j = firstRoomInRow; j < firstRoomInRow + gridSize; j++) {
		roomRow = "";
		if (roomBlockaded(j)) {
			roomRow += GetRoomContentStringMap(RoomContent::BLOCKADE);
		}
		else {
			rs = getRoomStatus(j);
			if (roomHasContent(j, RoomContent::AGENT_NORTH)) {
				roomRow += GetRoomContentStringMap(RoomContent::AGENT_NORTH);
			}
			else if (roomHasContent(j, RoomContent::AGENT_EAST)) {
				roomRow += GetRoomContentStringMap(RoomContent::AGENT_EAST);
			}
			else if (roomHasContent(j, RoomContent::AGENT_SOUTH)) {
				roomRow += GetRoomContentStringMap(RoomContent::AGENT_SOUTH);
			}
			else if (roomHasContent(j, RoomContent::AGENT_WEST)) {
				roomRow += GetRoomContentStringMap(RoomContent::AGENT_WEST);
			}
			else if (roomHasContent(j, RoomContent::AGENT_DEAD)) {
				roomRow += GetRoomContentStringMap(RoomContent::AGENT_DEAD);
			}
			if (rs == RoomStatus::VISITED) {
				roomRow += "*"; // TODO GetRoomStatusStringMap
			}
			else if (rs == RoomStatus::FRINGE) {
				roomRow += "?";
			}
			else {
				roomRow += "";
			}
		}
		roomRow = "|" + stretchRoomRow(roomRow);
		row += roomRow;
	}
	std::cout << row << "|" << std::endl;
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

RoomStatus World::getRoomStatus(int room) {
	if (room >= 0 && room < (int) world.size())
		return world.at(room).getRoomStatus();
	return static_cast<RoomStatus>(NULL);
}

void World::setRoomStatus(int room, RoomStatus rs) {
	world.at(room).setRoomStatus(rs);
}

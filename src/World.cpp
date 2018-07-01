
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

Room World::getRoom(int room) {
	if (room >= 0 && room < (int) world.size()) {
		 return world.at(room);
	} else {
		return Room();
	}
}

int World::getAdjacentRoomNumber(int room, Direction dir) {
	return world.at(room).adjacentRoom(dir);
}

std::vector<int> World::getAdjacentRoomNumbers(int room) {
	std::vector<int> rooms = std::vector<int>();
	for (Direction d : directionVector()) {
		int r = getAdjacentRoomNumber(room, d);
		if (r > -1) rooms.push_back(r);
	}
	return rooms;
}

std::vector<int> World::getAdjacentDiagonalRoomNumbers(int room) {
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

std::vector<int> World::getAllAdjacentRoomNumbers(int room) {
	std::vector<int> adjRooms = getAdjacentRoomNumbers(room);
	std::vector<int> diagRooms = getAdjacentDiagonalRoomNumbers(room);
	std::vector<int> allRooms;
	allRooms.reserve( adjRooms.size() + diagRooms.size() ); // preallocate memory
	allRooms.insert( allRooms.end(), adjRooms.begin(), adjRooms.end() );
	allRooms.insert( allRooms.end(), diagRooms.begin(), diagRooms.end() );
	return allRooms;
}

bool World::roomHasContent(int room, RoomContent rc) {
	if (room < 0 || room > getNumRooms()) return false;
	return getRoom(room).hasContent(rc);
}

bool World::roomBlockaded(int room) {
	if (room < 0 || room > getNumRooms()) return false;
	return getRoom(room).roomBlockaded();
}

bool World::roomIsEmpty(int room) {
	if (room < 0 || room > getNumRooms()) return false;
	return getRoom(room).roomEmpty();
}

void World::addRoomContent(int room, RoomContent rc) {
	if (room < 0 || room > getNumRooms()) return;
	world.at(room).addRoomContent(rc);
}

bool World::removeRoomContent(int room, RoomContent rc) {
	if (room < 0 || room > getNumRooms()) return false;
	return world.at(room).removeRoomContent(rc);
}

std::vector<Inference> World::getInferences(int room) {
	if (room < 0 || room > getNumRooms()) return std::vector<Inference>();
	return world.at(room).getInferences();
}

void World::addInference(int room, Inference i) {
	if (room < 0 || room > getNumRooms()) return;
		world.at(room).addInference(i);
}

void World::removeInference(int room, Inference i) {
	if (room < 0 || room > getNumRooms()) return;
	world.at(room).removeInference(i);
}

bool World::hasInference(int room, Inference i) {
	if (room < 0 || room > getNumRooms()) return false;
	return world.at(room).hasInference(i);
}

bool World::safeUnvisitedRoom(int room) {
	if (room < 0 || room > getNumRooms()) return false;
	return world.at(room).safeUnvisitedRoom();
}

bool World::safeRoom(int room) {
	if (room < 0 || room > getNumRooms()) return false;
	return world.at(room).safeRoom();
}

bool World::roomVisited(int room) {
	return world.at(room).roomVisited();
}

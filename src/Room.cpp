
/* Generated from GenMyModel */

#include "Room.hpp"

Room::Room() {
	room = -1;
	worldSize = -1;
	numRooms = -1;
	content = std::vector<RoomContent>();
}

Room::Room(int room, int worldSize) {
	this->room = room;
	this->worldSize = worldSize;
	numRooms = worldSize * worldSize;
	content = std::vector<RoomContent>();
}

Room::~Room() {}

int Room::adjacentRoom(Direction dir) {
	if (hasAdjacent(dir)) {
		switch (dir) {
		case Direction::NORTH:
			return room + worldSize;
		case Direction::EAST:
			return room + 1;
		case Direction::SOUTH:
			return room - worldSize;
		case Direction::WEST:
			return room - 1;
		}
	}
	return -1;
}

bool Room::hasAdjacent(Direction dir) {
	switch (dir) {
	case Direction::NORTH:
		return getRow() < worldSize - 1;
	case Direction::EAST:
		return getCol() < worldSize - 1;
	case Direction::SOUTH:
		return getRow() > 0;
	case Direction::WEST:
		return getCol() > 0;
	default:
		return false;
	}
}

void Room::addRoomContent(RoomContent rc) {
	if (!hasContent(rc))
		content.push_back(rc);
}

bool Room::removeRoomContent(RoomContent rc) {
	std::ptrdiff_t pos = distance(content.begin(), std::find(content.begin(), content.end(), rc));
	if (pos >= (int) content.size()) {
		return false;
	}
	content.erase(content.begin() + pos);
	return true;
}

int Room::getRoom() {
	return room;
}

int Room::getRow() {
	return room / worldSize;
}

int Room::getCol() {
	return room % worldSize;
}

std::vector<RoomContent> Room::getRoomContent() {
	return content;
}

bool Room::hasContent(RoomContent rc) {
	return std::find(content.begin(), content.end(), rc) != content.end();
}

bool Room::roomEmpty() {
	bool agentOnly = content.size() == 1 &&
			(hasContent(RoomContent::AGENT_DEAD) ||
			 hasContent(RoomContent::AGENT_EAST) ||
			 hasContent(RoomContent::AGENT_NORTH) ||
			 hasContent(RoomContent::AGENT_SOUTH) ||
			 hasContent(RoomContent::AGENT_WEST));
	return content.size() == 0 || agentOnly;
}


/* Generated from GenMyModel */

#include "Room.hpp"

Room::Room() {
	room = -1;
	worldSize = -1;
	numRooms = -1;
	content = std::vector<RoomContent>();
	status = RoomStatus::UNKNOWN;
}

Room::Room(int room, int worldSize) {
	this->room = room;
	this->worldSize = worldSize;
	numRooms = worldSize * worldSize;
	content = std::vector<RoomContent>();
	status = RoomStatus::UNKNOWN;
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
	if (rc == RoomContent::BLOCKADE) {
		content.clear();
	}
	if (!hasContent(rc) && !hasContent(RoomContent::BLOCKADE))
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

RoomStatus Room::getRoomStatus() {
	return status;
}

void Room::setRoomStatus(RoomStatus rs) {
	status = rs;
}

bool Room::roomBlockaded() {
	return std::find(content.begin(), content.end(), RoomContent::BLOCKADE) != content.end();
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

Room* Room::clone() {
	Room* ar = this;
	return ar;
}

std::vector<Inference> Room::getInferences() {
	return inferences;
}

void Room::addInference(Inference i) {
	if (!hasInference(i)){
		switch(i) {
		case Inference::PIT_FREE: removeInference(Inference::CONTAINS_PIT); break;
		case Inference::SUPMUW_EVIL_FREE: removeInference(Inference::CONTAINS_SUPMUW_EVIL); break;
		case Inference::SUPMUW_FREE: removeInference(Inference::CONTAINS_SUPMUW); break;
		case Inference::WUMPUS_FREE: removeInference(Inference::CONTAINS_WUMPUS); break;
		case Inference::CONTAINS_PIT: if (hasInference(Inference::PIT_FREE)) return; break;
		case Inference::CONTAINS_SUPMUW_EVIL: if (hasInference(Inference::SUPMUW_EVIL_FREE)) return; break;
		case Inference::CONTAINS_SUPMUW: if (hasInference(Inference::SUPMUW_FREE)) return; break;
		case Inference::CONTAINS_WUMPUS: if (hasInference(Inference::WUMPUS_FREE)) return; break;
		default: break;
		}
		inferences.push_back(i);
	}
}

void Room::removeInference(Inference i) {
	inferences.erase(std::remove(inferences.begin(), inferences.end(), i), inferences.end());
}

bool Room::hasInference(Inference i) {
	return std::find(inferences.begin(), inferences.end(), i) != inferences.end();
}

bool Room::safeUnvisitedRoom() {
	return (status != RoomStatus::VISITED
		 && !hasInference(Inference::CONTAINS_PIT)
		 && !hasInference(Inference::CONTAINS_SUPMUW_EVIL)
		 && !hasInference(Inference::CONTAINS_WUMPUS));
}

bool Room::safeRoom() {
	return (roomVisited() && canEnterRoom())
			|| (safeUnvisitedRoom());
}

bool Room::roomVisited() {
	return status == RoomStatus::VISITED;
}

bool Room::canEnterRoom() {
	return !roomBlockaded();
}

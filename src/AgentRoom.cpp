
/* Generated from GenMyModel */

#include "AgentRoom.hpp"

AgentRoom::AgentRoom(int room, int worldSize) : Room(room, worldSize) {
	status = RoomStatus::UNKNOWN;
}

AgentRoom::~AgentRoom() {}

AgentRoom* AgentRoom::clone() {
	AgentRoom* ar = this;
	return ar;
}

RoomStatus AgentRoom::getRoomStatus() {
	return status;
}

void AgentRoom::setRoomStatus(RoomStatus rs) {
	status = rs;
}

std::vector<Inference> AgentRoom::getInferences() {
	return inferences;
}

void AgentRoom::addInference(Inference i) {
	if (!hasInference(i)){
		inferences.push_back(i);
	}
}

bool AgentRoom::hasInference(Inference i) {
	return std::find(inferences.begin(), inferences.end(), i) != inferences.end();
}

bool AgentRoom::safeRoom() {
	// TODO should a room that contains the supmuw be considered safe?
	return (!hasInference(Inference::CONTAINS_PIT) && !hasInference(Inference::CONTAINS_SUPMUW_EVIL) && !hasInference(Inference::CONTAINS_WUMPUS));
}

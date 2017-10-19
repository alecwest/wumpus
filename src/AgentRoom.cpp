
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

void AgentRoom::removeInference(Inference i) {
	inferences.erase(std::remove(inferences.begin(), inferences.end(), i), inferences.end());
}

bool AgentRoom::hasInference(Inference i) {
	return std::find(inferences.begin(), inferences.end(), i) != inferences.end();
}

bool AgentRoom::safeUnvisitedRoom() {
	return (status != RoomStatus::VISITED
		 && !hasInference(Inference::CONTAINS_PIT)
		 && !hasInference(Inference::CONTAINS_SUPMUW_EVIL)
		 && !hasInference(Inference::CONTAINS_WUMPUS));
}

bool AgentRoom::safeRoom() {
	return (getRoomStatus() == RoomStatus::VISITED
			&& !roomBlockaded())
			|| (!hasInference(Inference::CONTAINS_PIT)
					&& !hasInference(Inference::CONTAINS_SUPMUW_EVIL)
					&& !hasInference(Inference::CONTAINS_WUMPUS));
}

bool AgentRoom::roomVisited() {
	return status == RoomStatus::VISITED;
}

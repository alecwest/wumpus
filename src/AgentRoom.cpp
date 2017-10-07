
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

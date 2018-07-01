
/* Generated from GenMyModel */

#include "AgentWorld.hpp"

AgentWorld::AgentWorld() : World() {
	gameWorld = GameWorld();
}

AgentWorld::AgentWorld(const GameWorld &gw) : World() {
	gameWorld = gw;
	gridSize = gameWorld.getGridSize();
	for (int i = 0; i < gridSize * gridSize; i++) {
		world.push_back(Room(i, gridSize));
	}
}

AgentWorld::~AgentWorld() {
}

std::vector<RoomContent> AgentWorld::perceptWorld(int room) {
	Room currentRoom = gameWorld.getRoom(room);
	std::vector<RoomContent> content = currentRoom.getRoomContent();
	for (RoomContent c : content){
		world.at(room).addRoomContent(c);
	}
	// Add a BUMP to the list if it exists in the agentWorld
	if (roomHasContent(room, RoomContent::BUMP)) {
		content.push_back(RoomContent::BUMP);
	}
	// If FOOD is found, remove it
	if (roomHasContent(room, RoomContent::FOOD)) {
		removeRoomContent(room, RoomContent::FOOD);
		// Without following line, the AgentWorld will continue perceiving food from GameWorld
		gameWorld.removeRoomContent(room, RoomContent::FOOD);
	}
	world.at(room).setRoomStatus(RoomStatus::VISITED);
	return content;
}

RoomContent AgentWorld::agentShot(int shootingRoom, Direction directionShot) {
	int room = getAdjacentRoomNumber(shootingRoom, directionShot);
	while (room > -1) {
		if (gameWorld.roomHasContent(room, RoomContent::SUPMUW)) {
			gameWorld.removeRoomContent(room, RoomContent::SUPMUW);
			return RoomContent::SUPMUW;
		}
		else if (gameWorld.roomHasContent(room, RoomContent::SUPMUW_EVIL)) {
			gameWorld.removeRoomContent(room, RoomContent::SUPMUW_EVIL);
			return RoomContent::SUPMUW;
		}
		else if (gameWorld.roomHasContent(room, RoomContent::WUMPUS)) {
			gameWorld.removeRoomContent(room, RoomContent::WUMPUS);
			return RoomContent::WUMPUS;
		}
		else if (gameWorld.roomHasContent(room, RoomContent::BLOCKADE)) {
			return RoomContent::BUMP;
		}

		room = getAdjacentRoomNumber(shootingRoom, directionShot);
	}
	return  RoomContent::BUMP;
}

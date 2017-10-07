
/* Generated from GenMyModel */

#include "AgentWorld.hpp"

AgentWorld::AgentWorld() : World() {
	gameWorld = GameWorld();
	worldProbability = 0.0;
}

AgentWorld::AgentWorld(const GameWorld &gw) : World(gw) {
	gameWorld = gw;
	worldProbability = 1.0;
}

AgentWorld::~AgentWorld() {
}

AgentWorld* AgentWorld::clone() {
}

void AgentWorld::perceptWorld(int room) {
	Room currentRoom = gameWorld.getRoom(room);
	std::vector<RoomContent> content = currentRoom.getRoomContent();

	for (RoomContent c : content){
		world.at(room).addRoomContent(c);
	}
}

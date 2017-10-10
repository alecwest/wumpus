
/* Generated from GenMyModel */

#include "AgentWorld.hpp"
#include "AgentRoom.hpp"

AgentWorld::AgentWorld() : World() {
	gameWorld = GameWorld();
	worldProbability = 0.0;
}

AgentWorld::AgentWorld(const GameWorld &gw) : World() {
	gameWorld = gw;
	gridSize = gameWorld.getGridSize();
	worldProbability = 1.0;
	for (unsigned int i = 0; i < gridSize * gridSize; i++) {
		world.push_back(AgentRoom(i, gridSize));
	}
}

AgentWorld::~AgentWorld() {
}

AgentWorld* AgentWorld::clone() {
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
	world.at(room).setRoomStatus(RoomStatus::VISITED);
	return content;
}

int AgentWorld::adjacentRoom(int room, Direction dir) {
	return world.at(room).adjacentRoom(dir);
}

std::vector<int> AgentWorld::adjacentRooms(int room) {
	std::vector<int> rooms = std::vector<int>();
//	std::cout << "Testing:: Trying to get adjacent rooms to " << room << " in a world with " << world.size() << " rooms.\n";
	int northRoom = world.at(room).adjacentRoom(Direction::NORTH);
	int eastRoom = 	world.at(room).adjacentRoom(Direction::EAST);
	int southRoom = world.at(room).adjacentRoom(Direction::SOUTH);
	int westRoom = world.at(room).adjacentRoom(Direction::WEST);
	if (northRoom > -1){
		rooms.push_back(northRoom);
	}
	if (eastRoom > -1){
		rooms.push_back(eastRoom);
	}
	if (southRoom > -1){
		rooms.push_back(southRoom);
	}
	if (westRoom > -1){
		rooms.push_back(westRoom);
	}
	return rooms;
}

std::vector<int> AgentWorld::adjacentDiagonalRooms(int room) {
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

Room AgentWorld::getRoom(int room) {
	if (room >= 0 && room < (int) world.size()) {
		return world.at(room);
	}
	else {
		return Room();
	}
}

RoomStatus AgentWorld::getRoomStatus(int room) {
	return world.at(room).getRoomStatus();
}

bool AgentWorld::roomHasContent(int room, RoomContent rc) {
	return getRoom(room).hasContent(rc);
}

void AgentWorld::addRoomContent(int room, RoomContent rc) {
	world.at(room).addRoomContent(rc);
}

bool AgentWorld::removeRoomContent(int room, RoomContent rc) {
	return world.at(room).removeRoomContent(rc);
}

void AgentWorld::printWorld() {
	if (world.size() < 0 || gridSize < 0){
		std::cout << "Could not print this world!";
		return;
	}
	RoomStatus rs;
	printDividingLine();
	for (int i = world.size() - gridSize; i >= 0; i -= gridSize) {
		// First line prints physical objects
		for (int j = 0; j < gridSize; j++) {
			std::cout << "|" <<
						 (roomHasContent(i + j, RoomContent::GOLD) ? "G " : "  ") <<
						 (roomHasContent(i + j, RoomContent::PIT) ? "P " : "  ") <<
					     (roomHasContent(i + j, RoomContent::WUMPUS) ? "W " : "  ");
			if(roomHasContent(i + j, RoomContent::SUPMUW)) {
				std::cout << "S" <<
							 (roomHasContent(i + j, RoomContent::FOOD) ? "F" : " ");
				// Food should only exist in room if SUPMUW does
			}
			else if (roomHasContent(i + j, RoomContent::SUPMUW_EVIL)) {
				std::cout << "E ";
			}
			else {
				std::cout << "  ";
			}
		}
		std::cout << "|" << std::endl;
		// Second line prints sensations
		for (int j = 0; j < gridSize; j++) {
			std::cout << "|" <<
						 (roomHasContent(i + j, RoomContent::BREEZE) ? " B" : "  ") <<
						 (roomHasContent(i + j, RoomContent::GLITTER) ? " G" : "  ") <<
						 (roomHasContent(i + j, RoomContent::MOO) ? " M" : "  ") <<
						 (roomHasContent(i + j, RoomContent::STENCH) ? " S" : "  ");
		}
		std::cout << "|" << std::endl;
		// Third line prints the agent and the room status
		for (int j = 0; j < gridSize; j++) {
			rs = getRoomStatus(i + j);
			std::cout << "|";
			if (world.at(i + j).hasContent(RoomContent::AGENT_NORTH)) {
				std::cout << "   ^^  ";
			}
			else if (world.at(i + j).hasContent(RoomContent::AGENT_EAST)) {
				std::cout << "   >>  ";
			}
			else if (world.at(i + j).hasContent(RoomContent::AGENT_SOUTH)) {
				std::cout << "   vv  ";
			}
			else if (world.at(i + j).hasContent(RoomContent::AGENT_WEST)) {
				std::cout << "   <<  ";
			}
			else if (world.at(i + j).hasContent(RoomContent::AGENT_DEAD)) {
				std::cout << "  RIP  ";
			}
			else {
				std::cout << "       ";
			}
			if (rs == RoomStatus::VISITED) {
				std::cout << "*";
			}
			else if (rs == RoomStatus::FRINGE) {
				std::cout << "?";
			}
			else {
				std::cout << " ";
			}

		}
		std::cout << "|" << std::endl;

		printDividingLine();
	}
}

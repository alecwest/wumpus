
/* Generated from GenMyModel */

#include "AgentWorld.hpp"
#include "AgentRoom.hpp"

AgentWorld::AgentWorld() : World() {
	gameWorld = GameWorld();
}

AgentWorld::AgentWorld(const GameWorld &gw) : World() {
	gameWorld = gw;
	gridSize = gameWorld.getGridSize();
	for (int i = 0; i < gridSize * gridSize; i++) {
		world.push_back(AgentRoom(i, gridSize));
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

int AgentWorld::adjacentRoom(int room, Direction dir) {
	return world.at(room).adjacentRoom(dir);
}

std::vector<int> AgentWorld::adjacentRooms(int room) {
	std::vector<int> rooms = std::vector<int>();
	for (Direction d : directionVector()) {
		int r = adjacentRoom(room, d);
		if (r > -1) rooms.push_back(r);
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

std::vector<int> AgentWorld::allAdjacentRooms(int room) {
	std::vector<int> adjRooms = adjacentRooms(room);
	std::vector<int> diagRooms = adjacentDiagonalRooms(room);
	std::vector<int> allRooms;
	allRooms.reserve( adjRooms.size() + diagRooms.size() ); // preallocate memory
	allRooms.insert( allRooms.end(), adjRooms.begin(), adjRooms.end() );
	allRooms.insert( allRooms.end(), diagRooms.begin(), diagRooms.end() );
	return allRooms;
}

Room AgentWorld::getRoom(int room) {
	if (room >= 0 && room < (int) world.size()) {
		return world.at(room);
	}
	else {
		return Room();
	}
}

int AgentWorld::getNumRooms() {
	return world.size();
}

RoomStatus AgentWorld::getRoomStatus(int room) {
	if (room >= 0 && room < (int) world.size())
		return world.at(room).getRoomStatus();
	return static_cast<RoomStatus>(NULL);
}

bool AgentWorld::roomHasContent(int room, RoomContent rc) {
	if (room < 0 || room > getNumRooms()) return false;
	return getRoom(room).hasContent(rc);
}

bool AgentWorld::roomBlockaded(int room) {
	if (room < 0 || room > getNumRooms()) return false;
	return getRoom(room).roomBlockaded();
}

bool AgentWorld::roomIsEmpty(int room) {
	if (room < 0 || room > getNumRooms()) return false;
	return getRoom(room).roomEmpty();
}

void AgentWorld::addRoomContent(int room, RoomContent rc) {
	if (room < 0 || room > getNumRooms()) return;
	world.at(room).addRoomContent(rc);
}

bool AgentWorld::removeRoomContent(int room, RoomContent rc) {
	if (room < 0 || room > getNumRooms()) return false;
	return world.at(room).removeRoomContent(rc);
}

std::vector<Inference> AgentWorld::getInferences(int room) {
	return world.at(room).getInferences();
}

void AgentWorld::addInference(int room, Inference i) {
	if (room < 0 || room > getNumRooms()) return;
		world.at(room).addInference(i);
}

void AgentWorld::removeInference(int room, Inference i) {
	if (room < 0 || room > getNumRooms()) return;
	world.at(room).removeInference(i);
}

bool AgentWorld::hasInference(int room, Inference i) {
	if (room < 0 || room > getNumRooms()) return false;
	return world.at(room).hasInference(i);
}

bool AgentWorld::safeUnvisitedRoom(int room) {
	if (room < 0 || room > getNumRooms()) return false;
	return world.at(room).safeUnvisitedRoom();
}

bool AgentWorld::safeRoom(int room) {
	if (room < 0 || room > getNumRooms()) return false;
	return world.at(room).safeRoom();
}

bool AgentWorld::roomVisited(int room) {
	return world.at(room).roomVisited();
}

void AgentWorld::setRoomStatus(int room, RoomStatus rs) {
	world.at(room).setRoomStatus(rs);
}

RoomContent AgentWorld::agentShot(int shootingRoom, Direction directionShot) {
	int room = adjacentRoom(shootingRoom, directionShot);
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

		room = adjacentRoom(shootingRoom, directionShot);
	}
	return  RoomContent::BUMP;
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
			if (roomBlockaded(i + j)) {
				std::cout << "|XXXXXXXX";
			}
			else {
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
		}
		std::cout << "|" << std::endl;
		// Second line prints sensations
		for (int j = 0; j < gridSize; j++) {
			if (roomBlockaded(i + j)) {
				std::cout << "|XXXXXXXX";
			}
			else {
				std::cout << "|" <<
							 (roomHasContent(i + j, RoomContent::BREEZE) ? " B" : "  ") <<
							 (roomHasContent(i + j, RoomContent::GLITTER) ? " G" : "  ") <<
							 (roomHasContent(i + j, RoomContent::MOO) ? " M" : "  ") <<
							 (roomHasContent(i + j, RoomContent::STENCH) ? " S" : "  ");
			}
		}
		std::cout << "|" << std::endl;
		// Third line prints the agent and the room status
		for (int j = 0; j < gridSize; j++) {
			if (roomBlockaded(i + j)) {
				std::cout << "|XXXXXXXX";
			}
			else {
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
		}
		std::cout << "|" << std::endl;

		printDividingLine();
	}
}

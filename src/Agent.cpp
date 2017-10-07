
/* Generated from GenMyModel */

#include "Agent.hpp"

Agent::Agent() {
	GameWorld gw = GameWorld();
	world = AgentWorld(gw);
	dir = Direction::EAST;
	room = 0;
	maxArrows = 1;
	world.addRoomContent(room, getAgentRoomContent());
}

Agent::Agent(const GameWorld &gw) {
	world = AgentWorld(gw);
	dir = Direction::EAST;
	room = 0;
	maxArrows = 1;
	world.addRoomContent(room, getAgentRoomContent());
}

Agent::~Agent() {}

int Agent::calculateScore() {
	int score = 0;
	score += info.arrowShot * -10;
	score += info.foodShared * 100;
	score += info.goldFound * 1000;
	score += info.movesTaken * -1;
	return score;
}

void Agent::printWorld() {
	world.printWorld();
}

void Agent::turnLeft() {
	switch(dir) {
	case Direction::NORTH:
		dir = Direction::WEST;
		break;
	case Direction::EAST:
		dir = Direction::NORTH;
		break;
	case Direction::SOUTH:
		dir = Direction::EAST;
		break;
	case Direction::WEST:
		dir = Direction::SOUTH;
		break;
	default:
		dir = Direction::EAST;
	}
}

void Agent::turnRight() {
	switch(dir) {
	case Direction::NORTH:
		dir = Direction::EAST;
		break;
	case Direction::EAST:
		dir = Direction::SOUTH;
		break;
	case Direction::SOUTH:
		dir = Direction::WEST;
		break;
	case Direction::WEST:
		dir = Direction::NORTH;
		break;
	default:
		dir = Direction::EAST;
	}
}

void Agent::forward() {
	RoomContent agentDirection = getAgentRoomContent();
	world.removeRoomContent(room, agentDirection);
	world.addRoomContent(world.adjacentRoom(room, dir), agentDirection);
}

void Agent::grab() {
	if(world.roomHasContent(room, RoomContent::GLITTER) && world.roomHasContent(room, RoomContent::GOLD)) {
		info.goldFound++;
		world.removeRoomContent(room, RoomContent::GOLD);
	}
}

RoomContent Agent::shoot() {
	RoomContent objectHit = RoomContent::BUMP;
	std::vector<int> traveledRooms = std::vector<int>();
	if (info.arrowShot >= maxArrows){
		return objectHit;
	}

	int r = world.adjacentRoom(room, dir);
	while(r != -1){
		traveledRooms.push_back(r);
		if (world.roomHasContent(r, RoomContent::WUMPUS)) {
			objectHit = RoomContent::WUMPUS;
			info.wumpusKilled++;
			noWumpusRooms = traveledRooms;
			break;
		}
		if (world.roomHasContent(r, RoomContent::SUPMUW)) {
			objectHit = RoomContent::SUPMUW;
			info.supmuwKilled++;
			noSupmuwRooms = traveledRooms;
			break;
		}
		r = world.adjacentRoom(r, dir);
	}
	info.arrowShot++;
	std::cout << "Arrow traveled from " << room << " to " << r << std::endl;
	return objectHit;
}

RoomContent Agent::getAgentRoomContent() {
	switch(dir) {
	case Direction::NORTH:
		return RoomContent::AGENT_NORTH;
	case Direction::EAST:
		return RoomContent::AGENT_EAST;
	case Direction::SOUTH:
		return RoomContent::AGENT_SOUTH;
	case Direction::WEST:
		return RoomContent::AGENT_WEST;
	default:
		return RoomContent::AGENT_EAST;
	}
}

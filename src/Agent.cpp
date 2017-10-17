
/* Generated from GenMyModel */

#include "Agent.hpp"

Agent::Agent() {
	GameWorld gw = GameWorld();
	world = AgentWorld(gw);
	dir = Direction::EAST;
	room = 0;
	maxArrows = 1;
	world.addRoomContent(room, getAgentRoomContent());
	world.perceptWorld(room);
}

Agent::Agent(const GameWorld &gw) {
	world = AgentWorld(gw);
	dir = Direction::EAST;
	room = 0;
	maxArrows = 1;
	world.addRoomContent(room, getAgentRoomContent());
	world.perceptWorld(room);
}

Agent::~Agent() {}

int Agent::calculateScore() {
	int score = 0;
	score += info.arrowShot * -10;
	score += info.foodShared * 100;
	score += info.goldFound * 1000;
	score += info.movesTaken * -1;
	score += !info.alive * -1000;
	return score;
}

void Agent::printWorld() {
	world.printWorld();
}

// Determine if anything  should happen based on the Room's Contents
void Agent::processPerception(std::vector<RoomContent> rc) {
	bool wumpus_attack = false;
	bool supmuw_attack = false;
	bool supmuw_exists = false;
	bool pit_exists = false;
	bool food_shared = false;
	bool breeze_felt = false;
	bool moo_heard = false;
	bool stench_smelled = false;
	bool glitter_seen = false;
	bool bump_felt = false;
	std::string message = "";
	for (auto c : rc) {
		switch (c) {
		case RoomContent::WUMPUS:
			wumpus_attack = true; break;
		case RoomContent::SUPMUW_EVIL:
			supmuw_attack = true; break;
		case RoomContent::SUPMUW:
			supmuw_exists = true; break;
		case RoomContent::FOOD:
			food_shared = true; break;
		case RoomContent::PIT:
			pit_exists = true; break;
		case RoomContent::BREEZE:
			breeze_felt = true; break;
		case RoomContent::MOO:
			moo_heard = true; break;
		case RoomContent::STENCH:
			stench_smelled = true; break;
		case RoomContent::GLITTER:
			glitter_seen = true; break;
		case RoomContent::BUMP:
			bump_felt = true; break;
		default:
			break;
		}
	}
	// If attacked or fell in a PIT not occupied by SUPMUW, agent is dead
	if (wumpus_attack || supmuw_attack || (pit_exists && !supmuw_exists)) {
		info.alive = false;
		world.removeRoomContent(room, getAgentRoomContent());
		world.addRoomContent(room, RoomContent::AGENT_DEAD);
	}
	else if (food_shared) {
		info.foodShared++;
		std::cout << "Testing:: Removing Food Content\n";
		world.removeRoomContent(room, RoomContent::FOOD);
	}
	// Process your move's encounter message
	if (breeze_felt) {
		message += "You feel a cold chill. Where is it coming from?\n";
	}
	if (moo_heard) {
		message += "You hear the moo of a sad cow.\n";
	}
	if (stench_smelled) {
		message += "Something smells terrible.\n";
	}
	if (glitter_seen) {
		message += "Something is glittering on the floor...\n";
	}
	if (bump_felt) {
		message += "You ran into a wall. Don't do that again.\n";
	}
	if (supmuw_exists) {
		message += "You encounter a Supmuw. He seems friendly.\n";
		message += food_shared ? "He gave you a piece of food! How sweet.\n" : "";
		message += pit_exists ? "The Supmuw is so big he kept you from falling into the pit!\n" : "";
	}
	else if (pit_exists) {
		message += wumpus_attack ? "You step on a Wumpus' head. He doesn't seem too happy about it.\n" : ""; // TODO remove this and any ability to place Wumpus in pits
		message += supmuw_attack ? "You step on a Supmuw's head. He doesn't seem too happy about it.\n" : "You tripped and fell into a pit.\n";
	}
	else if (wumpus_attack) {
		message += "You encountered the Wumpus! He's too quick to be stopped!\n";
	}
	else if (supmuw_attack) {
		message = "The Supmuw is evil! You can't escape quick enough!\n";
	}
	else if (message.length() == 0){
		message = "This room seems clear...\n";
	}
	if (!info.alive) info.gameOver = true;
	std::cout << message;
}

void Agent::turnLeft() {
	world.removeRoomContent(room, getAgentRoomContent());
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
	world.addRoomContent(room, getAgentRoomContent());
	info.movesTaken++;
}

void Agent::turnRight() {
	world.removeRoomContent(room, getAgentRoomContent());
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
	world.addRoomContent(room, getAgentRoomContent());
	info.movesTaken++;
}

void Agent::forward() {
	int nextRoom = world.adjacentRoom(room, dir);
	if (nextRoom < 0) {
		world.addRoomContent(room, RoomContent::BUMP);
		processPerception(world.perceptWorld(room));
		world.removeRoomContent(room, RoomContent::BUMP);
		info.movesTaken++; // TODO add assumption that a bump counts as a move taken.
		return;
	}
	RoomContent agentDirection = getAgentRoomContent();
	world.removeRoomContent(room, agentDirection);
	world.addRoomContent(nextRoom, agentDirection);
	room = nextRoom;
	info.movesTaken++;
	processPerception(world.perceptWorld(room));
}

void Agent::grab() {
	if(world.roomHasContent(room, RoomContent::GLITTER) && world.roomHasContent(room, RoomContent::GOLD)) {
		info.goldFound = true;
		world.removeRoomContent(room, RoomContent::GOLD);
	}
}

void Agent::exit() {
	if (room == info.safeRoom) {
		gameOver();
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
		if (world.roomHasContent(r, RoomContent::SUPMUW_EVIL)) {
			objectHit= RoomContent::SUPMUW_EVIL;
			info.supmuwKilled++;
			noSupmuwRooms = traveledRooms;
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

void Agent::gameOver() {
	info.gameOver = true;
	std::cout << "Game Over!" << std::endl << "Your score: " << calculateScore() << std::endl;
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

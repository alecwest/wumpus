
/* Generated from GenMyModel */

#include "Agent.hpp"

Agent::Agent() {
	GameWorld gw = GameWorld();
	world = AgentWorld(gw);
	dir = Direction::EAST;
	room = 0;
	world.addRoomContent(room, getAgentRoomContent());
	world.perceptWorld(room);
	printFrequency = PrintFrequency::FINAL_RESULT_ONLY;
}

Agent::Agent(const GameWorld &gw) {
	world = AgentWorld(gw);
	dir = Direction::EAST;
	room = 0;
	world.addRoomContent(room, getAgentRoomContent());
	world.perceptWorld(room);
	printFrequency = PrintFrequency::FINAL_RESULT_ONLY;
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
	if((printFrequency != PrintFrequency::FINAL_RESULT_ONLY
			&& printFrequency != PrintFrequency::FINAL_RESULT_WITH_TEXT_WALKTHROUGH)
			|| info.gameOver) {
		world.printWorld();
	}
}

void Agent::printMessage(std::string message) {
	if(printFrequency != PrintFrequency::FINAL_RESULT_ONLY || info.gameOver) {
		std::cout << message;
	}
}

// Determine if anything  should happen based on the Room's Contents
void Agent::processPerception(std::vector<RoomContent> rc) {
	PerceptionInfo perceptionInfo = setPerceptionInfo(rc);
	std::string message = "";

	// If attacked or fell in a PIT not occupied by SUPMUW, agent is dead
	if (perceptionInfo.wumpus_attack
			|| perceptionInfo.supmuw_attack
			|| (perceptionInfo.pit_exists
					&& !perceptionInfo.supmuw_exists)) {
		info.alive = false;
		info.gameOver = true;
		world.removeRoomContent(room, getAgentRoomContent());
		world.addRoomContent(room, RoomContent::AGENT_DEAD);
	}
	else if (perceptionInfo.food_shared) {
		info.foodShared = true;
		world.removeRoomContent(room, RoomContent::FOOD);
	}

	// Skip encounter message depending on print frequency
	if (printFrequency == PrintFrequency::FINAL_RESULT_ONLY) return;

	// Process your move's encounter message
	if (perceptionInfo.breeze_felt) {
		message += "You feel a cold chill. Where is it coming from?\n";
	}
	if (perceptionInfo.moo_heard) {
		message += "You hear the moo of a sad cow.\n";
	}
	if (perceptionInfo.stench_smelled) {
		message += "Something smells terrible.\n";
	}
	if (perceptionInfo.glitter_seen) {
		message += "Something is glittering on the floor...\n";
	}
	if (perceptionInfo.bump_felt) {
		message += "You ran into a wall. Don't do that again.\n";
	}
	if (perceptionInfo.blockade_hit) {
		message += "You ran into a blockade. Who put that there?\n";
	}
	if (perceptionInfo.supmuw_exists) {
		message += "You encounter a Supmuw. He seems friendly.\n";
		message += perceptionInfo.food_shared ? "He gave you a piece of food! How sweet.\n" : "";
		message += perceptionInfo.pit_exists ? "The Supmuw is so big he kept you from falling into the pit!\n" : "";
	}
	else if (perceptionInfo.pit_exists) {
		message += perceptionInfo.supmuw_attack ? "You step on a Supmuw's head. He doesn't seem too happy about it.\n" : "You tripped and fell into a pit.\n";
	}
	else if (perceptionInfo.wumpus_attack) {
		message += "You encountered the Wumpus! He's too quick to be stopped!\n";
	}
	else if (perceptionInfo.supmuw_attack) {
		message += "The Supmuw is evil! You can't escape quick enough!\n";
	}
	else if (message.length() == 0){
		message = "This room seems clear...\n";
	}
	printMessage(message);
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
	int nextRoom = world.getAdjacentRoomNumber(room, dir);
	info.movesTaken++;
	if (nextRoom < 0) {
		world.addRoomContent(room, RoomContent::BUMP);
		processPerception(world.perceptWorld(room));
		world.removeRoomContent(room, RoomContent::BUMP);
		return;
	}
	else {
		processPerception(world.perceptWorld(nextRoom));
		if (world.roomBlockaded(nextRoom)) {
			world.setRoomStatus(nextRoom, RoomStatus::VISITED);
			return;
		}
		RoomContent agentDirection = getAgentRoomContent();
		world.removeRoomContent(room, agentDirection);
		world.addRoomContent(nextRoom, agentDirection);
		room = nextRoom;
	}
}

void Agent::grab() {
	if(world.roomHasContent(room, RoomContent::GLITTER) && world.roomHasContent(room, RoomContent::GOLD)) {
		info.goldFound = true;
		world.removeRoomContent(room, RoomContent::GOLD);
	}
}

void Agent::exit() {
	if (room == info.safeRoom) {
		info.gameOver = true;
	}
}

RoomContent Agent::shoot() {
	if (info.arrowShot){
		return RoomContent::BUMP;
	}

	RoomContent hit = world.agentShot(room, dir);

	if (hit == RoomContent::WUMPUS) {
		info.wumpusKilled = true;
		printMessage("You hear a piercing scream. You killed the Wumpus!\n");
	}
	else if (hit == RoomContent::SUPMUW) {
		info.supmuwKilled = true;
		printMessage("You hear an anguished moo. You killed the Supmuw!\n");
	}
	else {
		printMessage("Your arrow vanishes into the darkness and nothing is heard.\n");
	}

	info.arrowShot = true;
	return hit;
}

void Agent::gameOver() {
	info.gameOver = true;
	std::cout << "Game Over!\nYour score: " << calculateScore() << "\n";
	printMessage(std::to_string(calculateScore()));
}

void Agent::setPrintFrequency(PrintFrequency freq) {
	printFrequency = freq;
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

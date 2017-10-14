
/* Generated from GenMyModel */

#include "IntelligentAgent.hpp"


void IntelligentAgent::turn(Direction d) {
	Direction agentDir = dir;
	while(agentDir != d) {
		moves.push(Move::RIGHT); // TODO can be improved to turn left when goal direction is one to the left
		agentDir = right(agentDir);
	}
}

void IntelligentAgent::processMoves() {
	while (!moves.empty()) {
		Move move = moves.front();
		moves.pop();
		switch(move) {
		case Move::FORWARD: std::cout << "Moving forward\n"; forward(); break;
		case Move::LEFT: std::cout << "Turning left\n"; turnLeft(); break;
		case Move::RIGHT: std::cout << "Turning right\n"; turnRight(); break;
		case Move::GRAB: std::cout << "Grabbing\n"; grab(); break;
		case Move::SHOOT: std::cout << "Shooting\n"; shoot(); break;
		case Move::EXIT: std::cout << "Exiting\n"; exit(); break;
		}
		printWorld();
	}
}

IntelligentAgent::IntelligentAgent() : Agent() {
	moves = std::queue<Move>();
//	kb = KnowledgeBase(world.getGridSize());
}

IntelligentAgent::IntelligentAgent(const GameWorld &gw) : Agent(gw) {
	moves = std::queue<Move>();
//	kb = KnowledgeBase(world.getGridSize());
}

IntelligentAgent::~IntelligentAgent() {}

void IntelligentAgent::markSafe(int r) {
	if (r < 0 || r > world.getGridSize()) return;
	// No need to infer about the room if we've already visited it
	if (world.getRoomStatus(r) != RoomStatus::VISITED) {
		world.setRoomStatus(r, RoomStatus::FRINGE);
		world.addInference(r, Inference::PIT_FREE);
		world.addInference(r, Inference::SUPMUW_EVIL_FREE);
		world.addInference(r, Inference::WUMPUS_FREE);
	}
}

void IntelligentAgent::markRoom(int r, Inference i) {
	if (r < 0 || r > world.getGridSize()) return;
	// No need to infer about the room if we've already visited it
	if (world.getRoomStatus(r) != RoomStatus::VISITED) {
		world.setRoomStatus(r, RoomStatus::FRINGE);
		world.addInference(r, i);
	}
}

// TODO can be refactored
void IntelligentAgent::inferRooms() {
	std::cout << "Inferring rooms surrounding room " << room << std::endl;
	std::vector<int> adjRooms = world.adjacentRooms(room);
	if (world.roomIsEmpty(room)) {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getGridSize()) continue;
			// Mark this room as completely safe. Orthogonal adjacency to a empty space guarantees safety
			markSafe(r);
		}
	}
	if (world.roomHasContent(room, RoomContent::BREEZE)) {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getGridSize()) continue;
			// Mark adjacent rooms as possible containers for pit
			markRoom(r, Inference::CONTAINS_PIT);
		}
	}
	if (world.roomHasContent(room, RoomContent::BUMP)) {
		int r = room;
		markRoom(r, Inference::EDGE);
		if (dir == Direction::NORTH || dir == Direction::SOUTH) {
			while(world.adjacentRoom(r, Direction::EAST) > -1) {
				r--;
				markRoom(r, Inference::EDGE);
			}
			r = room;
			while (world.adjacentRoom(r, Direction::WEST) > -1) {
				r++;
				markRoom(r, Inference::EDGE);
			}
		} else if (dir == Direction::EAST || dir == Direction::WEST) {
			while (r > -1) {
				r = world.adjacentRoom(r, Direction::NORTH);
				markRoom(r, Inference::EDGE);
			}
			r = room;
			while (r > -1) {
				r = world.adjacentRoom(r, Direction::SOUTH);
				markRoom(r, Inference::EDGE);
			}
		}
	}
	if (world.roomHasContent(room, RoomContent::MOO)) {
		std::vector<int> diagRooms = world.adjacentDiagonalRooms(room);
		for (int r : adjRooms) {
			if (r < 0 || r > world.getGridSize()) continue;
			// Mark adjacent rooms as possible containers for supmuw or supmuw_evil
			markRoom(r, Inference::CONTAINS_SUPMUW);
			markRoom(r, Inference::CONTAINS_SUPMUW_EVIL);
		}
		for (int r : diagRooms) {
			if (r < 0 || r > world.getGridSize()) continue;
			// Mark diagonal rooms as possible containers for supmuw or supmuw_evil
			markRoom(r, Inference::CONTAINS_SUPMUW);
			markRoom(r, Inference::CONTAINS_SUPMUW_EVIL);
		}
	}
	if (world.roomHasContent(room, RoomContent::STENCH)) {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getGridSize()) continue;
			// Mark adjacent rooms as possible containers for wumpus
			markRoom(r, Inference::CONTAINS_WUMPUS);
		}
	}
}

void IntelligentAgent::faceRoom(int r) {
	for (auto d : directionVector()) {
		if (world.adjacentRoom(room, d) == r) {
			turn(d);
			return;
		}
	}
}

// TODO when bump is encountered, knowledge about an edge should be added
// TODO can assume only one wumpus, supmuw
// TODO wumpus can't be in a pit
void IntelligentAgent::makeMove() {
	std::vector<int> adjRooms;
	printWorld();
	while (!info.gameOver) {
		adjRooms = world.adjacentRooms(room);
		inferRooms();
		for (auto r : adjRooms) {
			if (r < 0 || r > world.getGridSize()) continue;
			// Find room that is known to be safe, and move there
			if(world.safeRoom(r)) {
				std::cout << "Room " << r << " is safe\n";
				faceRoom(r);
				moves.push(Move::FORWARD);
				break;
			}
		}
//				moves.push(Move::FORWARD);
//				break;
//		else {
//			if (world.roomHasContent(room, RoomContent::BREEZE)) {

//			}
//		}

		processMoves();
		getchar();
	}
}

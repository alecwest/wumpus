
/* Generated from GenMyModel */

#include "IntelligentAgent.hpp"


void IntelligentAgent::turn(Direction d) {
	while(d != dir) {
		moves.push(Move::RIGHT); // TODO can be improved to turn left when goal direction is one to the left
		d = right(d);
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
		world.addInference(r, Inference::PIT_FREE);
		world.addInference(r, Inference::SUPMUW_EVIL_FREE);
		world.addInference(r, Inference::WUMPUS_FREE);
		world.setRoomStatus(r, RoomStatus::FRINGE);
	}
}

// TODO when bump is encountered, knowledge about an edge should be added
// TODO can assume only one wumpus, supmuw
// TODO wumpus can't be in a pit
void IntelligentAgent::makeMove() {
	printWorld();
	while (!info.gameOver) {
		if (world.roomIsEmpty(room)) {
			std::vector<int> adjRooms = world.adjacentRooms(room);
			for (int r : adjRooms) {
				if (r < 0 || r > world.getGridSize()) continue;
				// Mark this room as completely safe. Orthogonal adjacency to a empty space guarantees safety
				std::cout << "Testing:: marking room " << r << " as safe\n";
				markSafe(r);
			}
		}
		else if (world.roomHasContent(room, RoomContent::BREEZE)) {

		}
			// Find adjacent room that has not yet been visited, with a preference for moving North
//				turn(d);
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

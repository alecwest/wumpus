
/* Generated from GenMyModel */

#include "IntelligentAgent.hpp"


void IntelligentAgent::turn(Direction d) {
	Direction agentDir = dir;
	while(agentDir != d) {
		Move bestMove = bestDirectionToTurn(dir, d);
		moves.push(bestMove);
		agentDir = (bestMove == Move::RIGHT ? right(agentDir) : left(agentDir));
	}
}

void IntelligentAgent::goBack(int n) {
	moves.push(Move::RIGHT);
	moves.push(Move::RIGHT);
	for (int i = 0; i < n; i++) {
		moves.push(Move::FORWARD);
	}
}

void IntelligentAgent::processMoves() {
	while (!moves.empty() && !info.gameOver) {
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
}

IntelligentAgent::IntelligentAgent(const GameWorld &gw) : Agent(gw) {
	moves = std::queue<Move>();
}

IntelligentAgent::~IntelligentAgent() {}

void IntelligentAgent::markSafe(int r) {
	if (r < 0 || r > world.getNumRooms()) return;
	// No need to infer about the room if we've already visited it
	if (world.getRoomStatus(r) != RoomStatus::VISITED) {
		world.setRoomStatus(r, RoomStatus::FRINGE);
		world.addInference(r, Inference::PIT_FREE);
		world.addInference(r, Inference::SUPMUW_EVIL_FREE);
		world.addInference(r, Inference::WUMPUS_FREE);
	}
}

void IntelligentAgent::markRoom(int r, Inference i) {
	if (r < 0 || r > world.getNumRooms()) return;
	// No need to infer about the room if we've already visited it
	if (world.getRoomStatus(r) != RoomStatus::VISITED) {
		world.setRoomStatus(r, RoomStatus::FRINGE);
		world.addInference(r, i);
	}
}

// TODO can be refactored
void IntelligentAgent::inferRooms() {
	std::vector<int> adjRooms = world.adjacentRooms(room);
	if (world.roomIsEmpty(room)) {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			// Mark this room as completely safe. Orthogonal adjacency to a empty space guarantees safety
			markSafe(r);
		}
	}
	if (world.roomHasContent(room, RoomContent::BREEZE)) {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			// Mark adjacent rooms as possible containers for pit
			markRoom(r, Inference::CONTAINS_PIT);
		}
	}
	if (world.roomHasContent(room, RoomContent::BUMP)) { // TODO assuming agent knows the grid is a square, but not the size of it until it hits the edge
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
			if (r < 0 || r > world.getNumRooms()) continue;
			// Mark adjacent rooms as possible containers for supmuw or supmuw_evil
			markRoom(r, Inference::CONTAINS_SUPMUW);
			markRoom(r, Inference::CONTAINS_SUPMUW_EVIL);
		}
		for (int r : diagRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			// Mark diagonal rooms as possible containers for supmuw or supmuw_evil
			markRoom(r, Inference::CONTAINS_SUPMUW);
			markRoom(r, Inference::CONTAINS_SUPMUW_EVIL);
		}
	}
	if (world.roomHasContent(room, RoomContent::STENCH)) {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
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

void IntelligentAgent::goToRoom(int r) {
	for (auto d : directionVector()) {
		if (world.adjacentRoom(room, d) == r) {
			turn(d);
			moves.push(Move::FORWARD);
			return;
		}
	}
}

void IntelligentAgent::pathToFringe() {
	std::queue<Move> movesToAdd;
	movesToAdd = recursiveBestFirstSearch(room, dir, movesToAdd);
	moves = movesToAdd;
	/*
	 * If current node is Fringe, return movesToAdd
	 * init vector of successors <int>
	 * For each adjacent node (n),
	 * add n to successors vector
	 * if successors is empty, return NULL
	 * push the appropriate direction to move to get to that node as well as the movesToAdd vector
	 * Add these new directions, and then get the children
	 */
	std::cout << "Here's the added moves:\n";
	while (!movesToAdd.empty()) {
		Move m = moves.front();
		switch(m) {
		case Move::LEFT:
			std::cout << "Move left\n"; break;
		case Move::RIGHT:
			std::cout << "Move right\n"; break;
		case Move::FORWARD:
			std::cout << "Move forward\n"; break;
		default: break;
		}
		movesToAdd.pop();
	}
}

// TODO rename method
std::queue<Move> IntelligentAgent::recursiveBestFirstSearch(
				int currRoom, Direction currDir,
				std::queue<Move> pathMoves) {

	if (world.safeUnvisitedRoom(currRoom)) {
		std::cout << "Room " << currRoom << " is safe and unvisited\n";
		return pathMoves;
	}
	std::vector<int> successors = world.adjacentRooms(currRoom);
	if (successors.empty()) return std::queue<Move>();
	for (auto s : successors) {
		for (auto d : directionVector()) {
			if (world.adjacentRoom(currRoom, d) == s) {
				Direction agentDir = currDir;
				std::queue<Move> pathMovesAndNew = pathMoves;
				while(agentDir != d) {
					pathMovesAndNew.push(bestDirectionToTurn(dir, d));
					agentDir = right(agentDir);
				}
				pathMovesAndNew.push(Move::FORWARD);

				return recursiveBestFirstSearch(s, agentDir, pathMovesAndNew);
			}
		}
	}
	std::cout << "Reached the end. This shouldn't happen???\n";
	return pathMoves;
}

// TODO when bump is encountered, knowledge about an edge should be added
// TODO can assume only one wumpus, supmuw
// TODO wumpus can't be in a pit
void IntelligentAgent::makeMove() {
	std::vector<int> adjRooms;
	int numAdjVisited;
	printWorld();
	while (!info.gameOver) {
		numAdjVisited = 0;
		adjRooms = world.adjacentRooms(room);
		inferRooms();
		// Find a not yet visited room that is safe
		for (auto r : adjRooms) {
			// Find room that is known to be safe, and move there
			if(world.safeUnvisitedRoom(r)) {
				faceRoom(r);
				moves.push(Move::FORWARD);
				break;
			}
			if (world.getRoomStatus(r) == RoomStatus::VISITED) numAdjVisited++;
			// If all adjacent rooms are visited or unsafe, don't double back.
			if (numAdjVisited == (int)adjRooms.size()) {
				std::cout << "All surrounding options are visited\n";
				// Find the closest Fringe (that you can safely reach) and take that path
				pathToFringe();
				std::cout << "Moves has " << moves.size() << " moves in it\n";
			}
		}
		// If no move was determined, double back to the previous room
		if (moves.size() == 0) {
			goBack(1);
		}

		processMoves();
		getchar();
	}
	gameOver();
}

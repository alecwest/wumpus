
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
	stenchesFound = std::vector<int>();
	moosFound = std::vector<int>();
	wumpusRoom = -1;
	supmuwRoom = -1;
}

IntelligentAgent::IntelligentAgent(const GameWorld &gw) : Agent(gw) {
	moves = std::queue<Move>();
	stenchesFound = std::vector<int>();
	moosFound = std::vector<int>();
	wumpusRoom = -1;
	supmuwRoom = -1;
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
	if (!world.roomVisited(r)) {
		world.setRoomStatus(r, RoomStatus::FRINGE);
		world.addInference(r, i);
	}
}

// TODO can be refactored
void IntelligentAgent::inferRooms() {
	/*
	 * If there is more than one wumpus square, we can attempt to identify it
	 * by checking rooms for having that many adjacent stench squares (and being unvisited). If only one such square exists, then that is the wumpus' location
	 * Do the same for the supmuw. If the Wumpus location is known, and the Supmuw is identified as adjacent to it or in the same block, mark it as evil
	 */

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
		std::vector<int> allRooms = world.allAdjacentRooms(room);
		if (std::find(moosFound.begin(), moosFound.end(), room) == moosFound.end()) moosFound.push_back(room);
		std::cout << "Testing:: Num moos found = " << moosFound.size() << std::endl;
		for (int r : allRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			// Mark adjacent rooms as possible containers for supmuw or supmuw_evil
			markRoom(r, Inference::CONTAINS_SUPMUW);
			markRoom(r, Inference::CONTAINS_SUPMUW_EVIL);
		}
	}
	if (world.roomHasContent(room, RoomContent::STENCH)) {
		if (std::find(stenchesFound.begin(), stenchesFound.end(), room) == stenchesFound.end()) stenchesFound.push_back(room);
		std::cout << "Testing:: Num stenches found = " << stenchesFound.size() << std::endl;
		for (int r : adjRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			// Mark adjacent rooms as possible containers for wumpus
			markRoom(r, Inference::CONTAINS_WUMPUS);
		}
	}

	// TODO the stench and moo checks can be refactored
	if (world.roomHasContent(room, RoomContent::STENCH) && (int)stenchesFound.size() > 1) {
		int roomsWithStench;
		std::vector<int> candidateRooms;
		std::vector<int> roomsToCheck = world.adjacentRooms(room);
		// Check rooms adjacent to stench room
		for (auto roomToCheck : roomsToCheck) {
			// If room is visited already, it can't contain the Wumpus
			if (world.roomVisited(roomToCheck)) continue;
			roomsWithStench = 0;
			std::vector<int> roomToCheckAdjRooms = world.adjacentRooms(roomToCheck);
			for (auto roomToCheckAdjRoom : roomToCheckAdjRooms) {
				if (world.roomHasContent(roomToCheckAdjRoom, RoomContent::STENCH)) {
					roomsWithStench++;
				}
			}
			if (roomsWithStench == (int)stenchesFound.size()) {
				candidateRooms.push_back(roomToCheck);
			}
		}
		if (candidateRooms.size() == 1) {
			wumpusRoom = candidateRooms.at(0);
			std::cout << "Testing:: Agent determined location of Wumpus to be " << wumpusRoom << std::endl;
		}
		else {
			std::cout << "Could not determine the location of the Wumpus\n";
		}
	}

	if (world.roomHasContent(room, RoomContent::MOO) && (int)moosFound.size() > 1) {
		int roomsWithMoo;
		std::vector<int> candidateRooms;
		std::vector<int> roomsToCheck = world.allAdjacentRooms(room);
		// Check rooms adjacent to moo room
		for (auto roomToCheck : roomsToCheck) {
			// If room is visited already, it can't contain the Supmuw
			if (world.roomVisited(roomToCheck)) continue;
			roomsWithMoo = 0;
			std::vector<int> roomToCheckAdjRooms = world.allAdjacentRooms(roomToCheck);
			for (auto roomToCheckAdjRoom : roomToCheckAdjRooms) {
				if (world.roomHasContent(roomToCheckAdjRoom, RoomContent::MOO)) {
					roomsWithMoo++;
				}
			}
			if (roomsWithMoo == (int)moosFound.size()) {
				candidateRooms.push_back(roomToCheck);
			}
		}
		if (candidateRooms.size() == 1) {
			supmuwRoom = candidateRooms.at(0);
			std::cout << "Testing:: Agent determined location of Supmuw to be " << supmuwRoom << std::endl;
		}
		else {
			std::cout << "Could not determine the location of the Supmuw\n";
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
	movesToAdd = depthLimitedSearch(room, dir, movesToAdd, 0);
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
	// TODO remove this
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
std::queue<Move> IntelligentAgent::depthLimitedSearch(
				int currRoom, Direction currDir,
				std::queue<Move> pathMoves, int depth) {
//	std::cout << "Testing:: Recursive depth is " << depth << std::endl;
	if (world.safeUnvisitedRoom(currRoom)) {
//		std::cout << "Testing:: Room " << currRoom << " is safe and unvisited\n";
		return pathMoves;
	}
	if (depth == 5) return std::queue<Move>();

	std::vector<std::queue<Move>> paths;
	std::vector<int> successors = world.adjacentRooms(currRoom);
	if (successors.empty()) return std::queue<Move>();
	for (auto s : successors) {
		for (auto d : directionVector()) {
			if (world.adjacentRoom(currRoom, d) == s && world.safeRoom(s)) {
				Direction agentDir = currDir;
				std::queue<Move> pathMovesAndNew = pathMoves;
				// TODO If this algorithm can't find a path, then it's probably time to give up
				// TODO Test this is the *only* path finder
				// TODO Doesn't seem terribly effective as the only finder... yet. It walked into the wumpus
				// TODO Add some logic that helps pinpoint the location of wumpus and supmuw (and if its evil) based on multiple stench or moo markers
				while(agentDir != d) {
					pathMovesAndNew.push(bestDirectionToTurn(dir, d));
					agentDir = right(agentDir);
				}
				pathMovesAndNew.push(Move::FORWARD);

				paths.push_back(depthLimitedSearch(s, agentDir, pathMovesAndNew, depth+1));
			}
		}
	}
	// Determine the best, non-zero-length solution found so far
	int bestPathIndex = 1000;
	int bestPathSize = 1000;
	for (int i = 0; i < (int)paths.size(); i++) {
		if ((int)paths.at(i).size() < bestPathSize && (int)paths.at(i).size() != 0) {
			bestPathIndex = i;
			bestPathSize = paths.at(i).size();
		}
	}
	if (bestPathIndex == 1000) return std::queue<Move>();
//	std::queue<Move> best = paths.at(bestPathIndex);
	// TODO remove this
//	std::cout << "Here are the proposed moves being returned at this path\n";
//	while (!best.empty()) {
//		Move m = best.front();
//		best.pop();
//		if (m == Move::LEFT) std::cout << "Move left\n";
//		else if (m == Move::RIGHT) std::cout << "Move right\n";
//		else if (m == Move::FORWARD) std::cout << "Move forward\n";
//	}
//	std::cout << "Reached the end.\n";
	return paths.at(bestPathIndex);
}

bool IntelligentAgent::wumpusRoomFound() {
	return wumpusRoom >= 0;
}

bool IntelligentAgent::supmuwRoomFound() {
	return supmuwRoom >= 0;
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
				// Find the closest Fringe (that you can safely reach) and take that path
				pathToFringe();
				std::cout << "Testing:: Moves has " << moves.size() << " moves in it\n";
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

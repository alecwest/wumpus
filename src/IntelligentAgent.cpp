
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
	supmuwEvil = true;
}

IntelligentAgent::IntelligentAgent(const GameWorld &gw) : Agent(gw) {
	moves = std::queue<Move>();
	stenchesFound = std::vector<int>();
	moosFound = std::vector<int>();
	wumpusRoom = -1;
	supmuwRoom = -1;
	supmuwEvil = true;
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

bool IntelligentAgent::safeUnvisitedRoom(int r) {
	return ((wumpusRoom != r || info.wumpusKilled) &&
			((supmuwRoom != r || info.supmuwKilled) || (supmuwRoom == r && !supmuwEvil)) &&
			world.safeUnvisitedRoom(r));
}

bool IntelligentAgent::safeRoom(int r) {
	return ((r == info.safeRoom) ||
			((wumpusRoom != r || info.wumpusKilled) &&
					((supmuwRoom != r || info.supmuwKilled) && (supmuwRoom == r && !supmuwEvil)) &&
					world.safeRoom(r)));
}

// TODO can be refactored
void IntelligentAgent::inferRooms() {

	///////////////////////////////////////////////
	// ******* Identify Possible Content ******* //
	///////////////////////////////////////////////

	std::vector<int> adjRooms = world.adjacentRooms(room);

	if (not info.goldFound and world.roomHasContent(room, RoomContent::GLITTER)) {
		moves.push(Move::GRAB);
	}

	if (world.roomHasContent(room, RoomContent::BREEZE)) {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			// Mark adjacent rooms as possible containers for pit
			markRoom(r, Inference::CONTAINS_PIT);
		}
	}
	else {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			markRoom(r, Inference::PIT_FREE);
		}
	}

	if (world.roomHasContent(room, RoomContent::BUMP)) { // TODO assuming agent knows the grid is a square, but not the size of it until it hits the edge (I don't think I'm assuming this anymore...lol)
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
	else {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			markRoom(r, Inference::SUPMUW_FREE);
			markRoom(r, Inference::SUPMUW_EVIL_FREE);
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
	else {
		for (int r : adjRooms) {
			if (r < 0 || r > world.getNumRooms()) continue;
			markRoom(r, Inference::WUMPUS_FREE);
		}
	}


	///////////////////////////////////////////
	// ******* Begin Danger Location ******* //
	///////////////////////////////////////////

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
			else {
				// If room doesn't have all the identified stench rooms adjacent to it, it can't be a wumpus room
				world.addInference(roomToCheck, Inference::WUMPUS_FREE);
			}
		}
		if (candidateRooms.size() == 1) {
			wumpusRoom = candidateRooms.at(0);
			std::cout << "Testing:: Agent determined location of Wumpus to be " << wumpusRoom << std::endl;
			if (wumpusRoomFound() && supmuwRoomFound()) {
				if (std::abs(wumpusRoom - supmuwRoom) != 1 && std::abs(wumpusRoom - supmuwRoom) != 10) {
					std::cout << "Testing:: Supmuw is not evil!\n";
					world.addInference(supmuwRoom, Inference::SUPMUW_EVIL_FREE);
					supmuwEvil = false;
				}
			}
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
			else {
				// If room doesn't have all the identified moo rooms adjacent to it, it can't be a supmuw room
				world.addInference(roomToCheck, Inference::SUPMUW_FREE);
				world.addInference(roomToCheck, Inference::SUPMUW_EVIL_FREE);
			}
		}
		if (candidateRooms.size() == 1) {
			supmuwRoom = candidateRooms.at(0);
//			std::cout << "Testing:: Agent determined location of Supmuw to be " << supmuwRoom << std::endl;
			if (wumpusRoomFound() && supmuwRoomFound()) {
				if (std::abs(wumpusRoom - supmuwRoom) != 1 && std::abs(wumpusRoom - supmuwRoom) != 10) {
//					std::cout << "Testing:: Supmuw is not evil!\n";
					world.addInference(supmuwRoom, Inference::SUPMUW_EVIL_FREE);
					supmuwEvil = false;
				}
			}
		}
		else {
//			std::cout << "Testing:: Could not determine the location of the Supmuw\n";
		}
	}
	if (supmuwRoomFound() && !wumpusRoomFound() && supmuwEvil) {
		bool noChanceForNearbyWumpus = true;
		std::vector<int> roomsToCheck = world.adjacentRooms(supmuwRoom);
		roomsToCheck.push_back(supmuwRoom);
		for (auto roomToCheck : roomsToCheck) {
			if (world.hasInference(roomToCheck, Inference::CONTAINS_WUMPUS)) {
//				std::cout << "Testing:: Could not yet determine if the supmuw is evil\n";
				noChanceForNearbyWumpus = false;
			}
		}
		if (noChanceForNearbyWumpus) {
//			std::cout << "Testing:: Agent determined that supmuw room and it's adjacent rooms are wumpus free, making him friendly!\n";
			supmuwEvil = false;
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
	movesToAdd = depthLimitedSearch(room, dir, movesToAdd, 0, false);
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
	std::cout << "Testing:: Here's the added moves:\n";
	while (!movesToAdd.empty()) {
		Move m = movesToAdd.front();
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
				std::queue<Move> pathMoves, int depth, bool targetHome) {
//	std::cout << "Testing:: Recursive depth is " << depth << std::endl;
	bool finalCheck = world.safeUnvisitedRoom(currRoom);
	if (targetHome) finalCheck = (currRoom == info.safeRoom);
	if (finalCheck) {
//		std::cout << "Testing:: Room " << currRoom << " is safe and unvisited\n";
		return pathMoves;
	}
	// TODO is changing the depthLimit a good idea?
	// TODO instead, just trying comparing distances from home
	int depthLimit = targetHome ? 10 : 5;
	if (depth == depthLimit) return std::queue<Move>();

	std::vector<std::queue<Move>> paths;
	std::vector<int> successors = world.adjacentRooms(currRoom);
	if (successors.empty()) return std::queue<Move>();
	for (auto s : successors) {
		for (auto d : directionVector()) {
			if (world.adjacentRoom(currRoom, d) == s
					&& world.safeRoom(s)
					&& (world.getRoomStatus(s) == RoomStatus::VISITED || !targetHome)) {
				Direction agentDir = currDir;
				std::queue<Move> pathMovesAndNew = pathMoves;
				while(agentDir != d) {
					Move nextMove = bestDirectionToTurn(agentDir, d);
					pathMovesAndNew.push(nextMove);
					if (nextMove == Move::LEFT) agentDir = left(agentDir);
					else agentDir = right(agentDir);
				}
				pathMovesAndNew.push(Move::FORWARD);

				paths.push_back(depthLimitedSearch(s, agentDir, pathMovesAndNew, depth+1, targetHome));
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
	return paths.at(bestPathIndex);
}

bool IntelligentAgent::wumpusRoomFound() {
	return wumpusRoom >= 0;
}

bool IntelligentAgent::supmuwRoomFound() {
	return supmuwRoom >= 0;
}

void IntelligentAgent::returnToSafeRoom() {
	std::queue<Move> movesToAdd;
	movesToAdd = depthLimitedSearch(room, dir, movesToAdd, 0, true);
	moves = movesToAdd;
	// TODO remove this
	std::cout << "Testing:: Here's the added moves to get home:\n";
	while (!movesToAdd.empty()) {
		Move m = movesToAdd.front();
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

// TODO when bump is encountered, knowledge about an edge should be added
// TODO wumpus can't be in a pit
void IntelligentAgent::makeMove() {
	std::vector<int> adjRooms;
	int numAdjChecked;
	printWorld();
	while (!info.gameOver) {
		numAdjChecked = 0;
		adjRooms = world.adjacentRooms(room);
		inferRooms();

		/*
		 * Priorities:
		 * 1. Visit friendly Supmuw if closeby TODO ONLY ONCE. IT SEEMS TO BE CONTINUOUSLY GIVING FOOD OUT
		 * 2. Leave if gold is found
		 * 3. TODO shoot wumpus if location is known
		 * 4. Continue exploring
		 * 5. Leave if all fringe rooms are unsafe
		 */
		if (supmuwRoomFound()
				and not supmuwEvil
				and std::find(adjRooms.begin(), adjRooms.end(), supmuwRoom) != adjRooms.end()) {
			goToRoom(supmuwRoom);
		}
		else if (info.goldFound) {
			if (room == info.safeRoom) {
				moves.push(Move::EXIT);
			}
			else {
				// TODO current implementation seems to want to continue exploring unvisited rooms?
				// TODO instead of returning the shortest non zero route, return the route that brings us closest to home (distance formula)
				returnToSafeRoom();
				// TODO need a better way to get out than just moving as far west and south as possible
				if (moves.size() == 0){
					if (world.safeRoom(world.adjacentRoom(room, Direction::WEST))) {
						turn(Direction::WEST);
						moves.push(Move::FORWARD);
					}
					else if (world.safeRoom(world.adjacentRoom(room, Direction::SOUTH))) {
						turn(Direction::SOUTH);
						moves.push(Move::FORWARD);
					}
				}
			}
		}
//		if (not world.roomHasContent(room, RoomContent::GLITTER))
		else {
			// Find a not yet visited room that is safe
			for (auto r : adjRooms) {
				// Find room that is known to be safe, and move there
				if(world.safeUnvisitedRoom(r)) {
					goToRoom(r);
					break;
				}
				if (world.getRoomStatus(r) == RoomStatus::VISITED || !world.safeRoom(r)) numAdjChecked++;
				// If all adjacent rooms are visited or unsafe, don't double back.
				if (numAdjChecked == (int)adjRooms.size()) {
					// Find the closest Fringe (that you can safely reach) and take that path
					pathToFringe();
	//				std::cout << "Testing:: Moves has " << moves.size() << " moves in it\n";
				}
			}
			// If no move was determined, double back to the previous room
			if (moves.size() == 0) {
				goBack(1);
			}
		}

		processMoves();
		getchar();
	}
	gameOver();
}

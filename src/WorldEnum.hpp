/*
 * WorldEnum.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#ifndef SRC_WORLDENUM_HPP_
#define SRC_WORLDENUM_HPP_

#include <vector>

enum class Direction { NORTH, EAST, SOUTH, WEST };
inline std::vector<Direction> directionVector() {
	std::vector<Direction> d = std::vector<Direction>();
	d.push_back(Direction::NORTH);
	d.push_back(Direction::EAST);
	d.push_back(Direction::SOUTH);
	d.push_back(Direction::WEST);
	return d;
}

inline Direction right(Direction d) {
	switch(d) {
	case Direction::NORTH: return Direction::EAST;
	case Direction::EAST: return Direction::SOUTH;
	case Direction::SOUTH: return Direction::WEST;
	case Direction::WEST: return Direction::NORTH;
	default: return Direction::EAST; // Not necessary, just trying to appease eclipse
	}
}
inline Direction left(Direction d) {
	switch(d) {
	case Direction::NORTH: return Direction::WEST;
	case Direction::EAST: return Direction::NORTH;
	case Direction::SOUTH: return Direction::EAST;
	case Direction::WEST: return Direction::SOUTH;
	default: return Direction::EAST; // Not necessary, just trying to appease eclipse
	}
}

enum class RoomContent { AGENT_DEAD, AGENT_EAST, AGENT_NORTH, AGENT_SOUTH, AGENT_WEST,
						 BREEZE, BUMP, FOOD, GLITTER, GOLD, MOO, PIT, STENCH,
						 SUPMUW_EVIL, SUPMUW, WUMPUS };

enum class Inference { CONTAINS_PIT, CONTAINS_SUPMUW, CONTAINS_SUPMUW_EVIL, CONTAINS_WUMPUS,
					   PIT_FREE, SUPMUW_FREE, SUPMUW_EVIL_FREE, WUMPUS_FREE, EDGE };

enum class RoomStatus { FRINGE, UNKNOWN, VISITED };

enum class Move { LEFT, RIGHT, FORWARD, SHOOT, GRAB, EXIT };
inline Move bestDirectionToTurn(Direction currDir, Direction targetDir) {
	if (currDir == Direction::NORTH) {
		switch (targetDir) {
		case Direction::EAST: return Move::RIGHT;
		case Direction::WEST: return Move::LEFT;
		default: return Move::RIGHT;
		}
	}
	else if (currDir == Direction::EAST) {
		switch (targetDir) {
		case Direction::SOUTH: return Move::RIGHT;
		case Direction::NORTH: return Move::LEFT;
		default: return Move::RIGHT;
		}
	}
	else if (currDir == Direction::SOUTH) {
		switch (targetDir) {
		case Direction::WEST: return Move::RIGHT;
		case Direction::EAST: return Move::LEFT;
		default: return Move::RIGHT;
		}
	}
	else { // currDir == Direction::WEST
		switch (targetDir) {
		case Direction::NORTH: return Move::RIGHT;
		case Direction::SOUTH: return Move::LEFT;
		default: return Move::RIGHT;
		}
	}
}

struct ExplorationStats {
	bool alive = true;
	bool gameOver = false;
	bool arrowShot = false; // TODO assuming one arrow
	int foodShared = false; // TODO assuming only one food can exist
	bool goldFound = false;
	int movesTaken = 0;
	bool supmuwKilled = false; // TODO assuming one supmuw and wumpus
	bool wumpusKilled = false;
	int safeRoom = 0;
};

#endif /* SRC_WORLDENUM_HPP_ */

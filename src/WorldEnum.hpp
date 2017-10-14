/*
 * WorldEnum.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#ifndef SRC_WORLDENUM_HPP_
#define SRC_WORLDENUM_HPP_

enum class Direction { NORTH, EAST, SOUTH, WEST };

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
					   PIT_FREE, SUPMUW_FREE, SUPMUW_EVIL_FREE, WUMPUS_FREE };

enum class RoomStatus { FRINGE, UNKNOWN, VISITED };

enum class Move { LEFT, RIGHT, FORWARD, SHOOT, GRAB, EXIT };

struct ExplorationStats {
	bool alive = true;
	bool gameOver = false;
	int arrowShot = 0;
	int foodShared = 0;
	int goldFound = 0;
	int movesTaken = 0;
	int supmuwKilled = 0;
	int wumpusKilled = 0;
	int safeRoom = 0;
};

#endif /* SRC_WORLDENUM_HPP_ */

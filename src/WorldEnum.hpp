/*
 * WorldEnum.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#ifndef SRC_WORLDENUM_HPP_
#define SRC_WORLDENUM_HPP_

enum class Direction { NORTH, EAST, SOUTH, WEST };

enum class RoomContent { AGENT_NORTH, AGENT_EAST, AGENT_SOUTH, AGENT_WEST, BREEZE, BUMP, GLITTER, GOLD, MOO, PIT, STENCH, SUPMUW, WUMPUS };

enum class RoomStatus { FRINGE, UNKNOWN, VISITED };

struct ExplorationStats {
	bool alive = true;
	int arrowShot = 0;
	int foodShared = 0;
	int goldFound = 0;
	int movesTaken = 0;
	int supmuwKilled = 0;
	int wumpusKilled = 0;
};

#endif /* SRC_WORLDENUM_HPP_ */

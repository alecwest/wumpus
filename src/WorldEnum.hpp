/*
 * WorldEnum.hpp
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#ifndef SRC_WORLDENUM_HPP_
#define SRC_WORLDENUM_HPP_

enum class Direction { NORTH, EAST, SOUTH, WEST };

enum class RoomContent { AGENT_DEAD, AGENT_EAST, AGENT_NORTH, AGENT_SOUTH, AGENT_WEST,
						 BREEZE, BUMP, FOOD, GLITTER, GOLD, MOO, PIT, STENCH,
						 SUPMUW_EVIL, SUPMUW, WUMPUS };

enum class RoomStatus { FRINGE, UNKNOWN, VISITED };

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

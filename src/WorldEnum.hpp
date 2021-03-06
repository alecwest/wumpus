#ifndef SRC_WORLDENUM_HPP_
#define SRC_WORLDENUM_HPP_

#include <vector>
#include <map>

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

enum class RoomContent { AGENT_DEAD, AGENT_EAST, AGENT_NONE, AGENT_NORTH, AGENT_SOUTH, AGENT_WEST,
						 BLOCKADE, BREEZE, BUMP, FOOD, GLITTER, GOLD, MOO, PIT, STENCH,
						 SUPMUW_EVIL, SUPMUW, WUMPUS };

inline std::string GetRoomContentStringMap(RoomContent rc) {
	static const std::map<RoomContent, std::string> roomContentToString = {
			{ RoomContent::AGENT_DEAD, "RIP" },
			{ RoomContent::AGENT_EAST, ">>" },
			{ RoomContent::AGENT_NONE, "" },
			{ RoomContent::AGENT_NORTH, "^^" },
			{ RoomContent::AGENT_SOUTH, "vv" },
			{ RoomContent::AGENT_WEST, "<<" },
			{ RoomContent::BLOCKADE, "XXXXXXXX" },
			{ RoomContent::BREEZE, "B" },
			{ RoomContent::BUMP, "" },
			{ RoomContent::FOOD, "F" },
			{ RoomContent::GLITTER, "G" },
			{ RoomContent::GOLD, "G" },
			{ RoomContent::MOO, "M" },
			{ RoomContent::PIT, "P" },
			{ RoomContent::STENCH, "S" },
			{ RoomContent::SUPMUW_EVIL, "E" },
			{ RoomContent::SUPMUW, "S" },
			{ RoomContent::WUMPUS, "W" }
	};

	return roomContentToString.find(rc)->second;
}

enum class Inference { CONTAINS_PIT, CONTAINS_SUPMUW, CONTAINS_SUPMUW_EVIL, CONTAINS_WUMPUS,
					   PIT_FREE, SUPMUW_FREE, SUPMUW_EVIL_FREE, WUMPUS_FREE, BLOCKADE };

enum class RoomStatus { FRINGE, UNKNOWN, VISITED };

inline std::string GetRoomStatusStringMap(RoomStatus rs) {
	static const std::map<RoomStatus, std::string> roomStatusToString = {
			{ RoomStatus::FRINGE, "?" },
			{ RoomStatus::UNKNOWN, "" },
			{ RoomStatus::VISITED, "*" }
	};

	return roomStatusToString.find(rs)->second;
}

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

enum class PrintFrequency { FINAL_RESULT_ONLY, FINAL_RESULT_WITH_TEXT_WALKTHROUGH,
							EVERYTHING_WITHOUT_PAUSE, EVERYTHING_WITH_PAUSE };

struct ExplorationStats {
	bool alive = true;
	bool gameOver = false;
	bool arrowShot = false;
	int foodShared = false;
	bool goldFound = false;
	int movesTaken = 0;
	bool supmuwKilled = false;
	bool wumpusKilled = false;
	int safeRoom = 0;
};

struct PerceptionInfo {
	bool wumpus_attack = false;
	bool supmuw_attack = false;
	bool supmuw_exists = false;
	bool pit_exists = false;
	bool food_shared = false;
	bool breeze_felt = false;
	bool moo_heard = false;
	bool stench_smelled = false;
	bool glitter_seen = false;
	bool bump_felt = false;
	bool blockade_hit = false;
};

inline PerceptionInfo setPerceptionInfo(std::vector<RoomContent> rc) {
	PerceptionInfo pi = PerceptionInfo();
	for (auto c : rc) {
		switch (c) {
		case RoomContent::WUMPUS:
			pi.wumpus_attack = true; break;
		case RoomContent::SUPMUW_EVIL:
			pi.supmuw_attack = true; break;
		case RoomContent::SUPMUW:
			pi.supmuw_exists = true; break;
		case RoomContent::FOOD:
			pi.food_shared = true; break;
		case RoomContent::PIT:
			pi.pit_exists = true; break;
		case RoomContent::BREEZE:
			pi.breeze_felt = true; break;
		case RoomContent::MOO:
			pi.moo_heard = true; break;
		case RoomContent::STENCH:
			pi.stench_smelled = true; break;
		case RoomContent::GLITTER:
			pi.glitter_seen = true; break;
		case RoomContent::BUMP:
			pi.bump_felt = true; break;
		case RoomContent::BLOCKADE:
			pi.blockade_hit = true; break;
		default:
			break;
		}
	}
	return pi;
}

#endif

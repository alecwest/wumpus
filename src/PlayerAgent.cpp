
/* Generated from GenMyModel */

#include "PlayerAgent.hpp"

PlayerAgent::PlayerAgent() : Agent() {}

PlayerAgent::PlayerAgent(const GameWorld &gw) : Agent(gw) {}

PlayerAgent::~PlayerAgent() {}

void PlayerAgent::makeMove() {
	char *move = NULL;
	while (strcmp(move, "q") != 0) {
		printWorld();
		std::cout << "Where to next?";
		std::cin >> move;
		if (strcmp(move, "r") == 0)
			turnRight();
		else if (strcmp(move, "l") == 0)
			turnLeft();
		else if (strcmp(move, "f") == 0)
			forward();
		else if (strcmp(move, "s") == 0)
			shoot();
	}
}

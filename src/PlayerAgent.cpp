
/* Generated from GenMyModel */

#include "PlayerAgent.hpp"

PlayerAgent::PlayerAgent() : Agent() {}

PlayerAgent::PlayerAgent(const GameWorld &gw) : Agent(gw) {}

PlayerAgent::~PlayerAgent() {}

void PlayerAgent::makeMove() {
	std::string move;
	printWorld();
	while (move != "q") {
		std::cout << "Where to next? ";
		std::getline(std::cin, move);
		if (move == "r")
			turnRight();
		else if (move == "l")
			turnLeft();
		else if (move == "f")
			forward();
		else if (move == "s")
			shoot();
		else if (move == "g")
			grab();
		printWorld();
	}
}

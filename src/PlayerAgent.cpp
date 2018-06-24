
/* Generated from GenMyModel */

#include "PlayerAgent.hpp"

PlayerAgent::PlayerAgent() : Agent() {printFrequency = PrintFrequency::EVERYTHING_WITH_PAUSE;}

PlayerAgent::PlayerAgent(const GameWorld &gw) : Agent(gw) {printFrequency = PrintFrequency::EVERYTHING_WITH_PAUSE;}

PlayerAgent::~PlayerAgent() {}

void PlayerAgent::makeMove() {
	std::string move;
	printWorld();
	while (move != "q" && !info.gameOver) {
		if (info.alive) {
			std::cout << "Where to next? (r)ight, (l)eft, (f)orward, (s)hoot, (g)rab, (e)xit, (q)uit ";
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
			else if (move == "e") {
				exit();
				continue;
			}

			printWorld();
		}
		else {
			gameOver();
		}
	}
}

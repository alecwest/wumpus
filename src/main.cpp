/*
 * main.cpp
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#include <ctime>

#include "GameWorld.hpp"
#include "IntelligentAgent.hpp"
#include "PlayerAgent.hpp"

int main() {
	srand(time(NULL));

	// Init world
//	GameWorld gw = GameWorld("in.txt"); // Text file generation
	GameWorld gw = GameWorld(); // Random terrain generation

	// Init player
//	PlayerAgent a = PlayerAgent(gw);
	IntelligentAgent a = IntelligentAgent(gw);

	a.setPrintFrequency(PrintFrequency::EVERYTHING_WITHOUT_PAUSE);

	gw.printWorld();
	a.makeMove();
	return 0;
}


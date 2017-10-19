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
//	GameWorld gw = GameWorld("../in.txt"); // Text file generation
	GameWorld gw = GameWorld(); // Random terrain generation

	// Init player
//	PlayerAgent a = PlayerAgent(gw);
	IntelligentAgent a = IntelligentAgent(gw);

	// 0 = final result only, 1 = final result and text walkthrough, 2 = everything without stopping, 3 = everything with getchar() pause
	a.setPrintFrequency(2);

	gw.printWorld();
	a.makeMove();
	return 0;
}


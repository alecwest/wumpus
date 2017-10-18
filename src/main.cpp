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
//	GameWorld gw = GameWorld("../test.txt");
	GameWorld gw = GameWorld();
//	PlayerAgent a = PlayerAgent(gw);
	IntelligentAgent a = IntelligentAgent(gw);
	gw.printWorld();
	a.makeMove();
	return 0;
}


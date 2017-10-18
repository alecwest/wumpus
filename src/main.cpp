/*
 * main.cpp
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#include "GameWorld.hpp"
#include "IntelligentAgent.hpp"
#include "PlayerAgent.hpp"

int main() {
	GameWorld gw = GameWorld("../test.txt");
//	PlayerAgent a = PlayerAgent(gw);
	IntelligentAgent a = IntelligentAgent(gw);
	gw.printWorld();
	a.makeMove();
	return 0;
}


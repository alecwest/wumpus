/*
 * main.cpp
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#include "GameWorld.hpp"
#include "PlayerAgent.hpp"

int main() {
	GameWorld gw = GameWorld("test.txt");
	PlayerAgent a = PlayerAgent(gw);
//	std::cout << "Before init playerAgent" << std::endl;
//	PlayerAgent a = PlayerAgent();
//	std::cout << "After init playerAgent" << std::endl;
	gw.printWorld();
	a.printWorld();
//	a.makeMove();
	std::cout << "Heyyyy\n";
	return 0;
}


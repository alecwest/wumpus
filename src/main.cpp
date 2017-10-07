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
	std::cout << "Main1" << std::endl;
//	std::cout << "Before init playerAgent" << std::endl;
//	PlayerAgent a = PlayerAgent();
//	std::cout << "After init playerAgent" << std::endl;
//	gw.printWorld();
	std::cout << "Heyyyy\n";
//	a.printWorld();
//	a.makeMove();
	return 0;
}


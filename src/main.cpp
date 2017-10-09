/*
 * main.cpp
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#include "GameWorld.hpp"
#include "PlayerAgent.hpp"

int main() {
	std::cout << "Testing1\n";
	GameWorld gw = GameWorld("test.txt");
	std::cout << "Testing2\n";
	PlayerAgent a = PlayerAgent(gw);
	std::cout << "Testing3\n";
//	std::cout << "Before init playerAgent" << std::endl;
//	PlayerAgent a = PlayerAgent();
//	std::cout << "After init playerAgent" << std::endl;
	gw.printWorld();
	a.makeMove();
	std::cout << "Heyyyy\n";
	return 0;
}


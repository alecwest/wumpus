
/* Generated from GenMyModel */

#ifndef DEF_PLAYERAGENT
#define DEF_PLAYERAGENT

#include "Agent.hpp"

class PlayerAgent : public Agent
{
public:
	PlayerAgent();
	PlayerAgent(const GameWorld &gw);
	virtual ~PlayerAgent();
	void makeMove();
};


#endif

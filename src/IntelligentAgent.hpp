
/* Generated from GenMyModel */

#ifndef DEF_INTELLIGENTAGENT
#define DEF_INTELLIGENTAGENT

#include <queue>

#include "Agent.hpp"
#include "KnowledgeBase.hpp"

class IntelligentAgent : public Agent
{
private:
//	KnowledgeBase kb;
	std::queue<Move> moves;
	std::vector<Direction> directions;
	// Pushes the appropriate number of turns onto the moves queue
	void turn(Direction d);
	// Iterates through accumulated moves and executes each one
	void processMoves();
	// Mark a room as completely safe to visit
	void markSafe(int r);
public:
	IntelligentAgent();
	IntelligentAgent(const GameWorld &gw);
	virtual ~IntelligentAgent();
	void makeMove();
};


#endif

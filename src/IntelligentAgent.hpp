
/* Generated from GenMyModel */

#ifndef DEF_INTELLIGENTAGENT
#define DEF_INTELLIGENTAGENT

#include <queue>

#include "Agent.hpp"
#include "KnowledgeBase.hpp"

class IntelligentAgent : public Agent
{
private:
	std::queue<Move> moves;

	// Pushes the appropriate number of turns onto the moves queue
	void turn(Direction d);
	// Iterates through accumulated moves and executes each one
	void processMoves();
	// Mark a room as completely safe to visit
	void markSafe(int r);
	// Mark a room with a single inference
	void markRoom(int r, Inference i);
	// Make inferences about surrounding rooms based on what is seen in the current room
	void inferRooms();
	// Determine which direction to turn and face it
	void faceRoom(int r);
public:
	IntelligentAgent();
	IntelligentAgent(const GameWorld &gw);
	virtual ~IntelligentAgent();
	void makeMove();
};


#endif

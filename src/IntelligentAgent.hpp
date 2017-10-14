
/* Generated from GenMyModel */

#ifndef DEF_INTELLIGENTAGENT
#define DEF_INTELLIGENTAGENT

#include <climits>
#include <queue>

#include "Agent.hpp"
#include "KnowledgeBase.hpp"

class IntelligentAgent : public Agent
{
private:
	std::queue<Move> moves;

	// Pushes the appropriate number of turns onto the moves queue
	void turn(Direction d);
	// Turns agent around and back tracks n rooms
	void goBack(int n);
	// Iterates through accumulated moves and executes each one
	void processMoves();
	// Mark a room as completely safe to visit
	void markSafe(int r);
	// Mark a room with a single inference
	void markRoom(int r, Inference i);
	// Make inferences about surrounding rooms based on what is seen in the current room
	void inferRooms();
	// Determine which direction to turn and face it (assuming r is an adjacent room)
	void faceRoom(int r);
	// Go to adjacent room
	void goToRoom(int r);
	// Push a series of steps that leads to the closest accessible fringe room
	void pathToFringe();
	// Finds the fastest path to a safe Fringe room
	std::queue<Move> recursiveBestFirstSearch(int currRoom, Direction currDir, std::queue<Move> movesToAdd);
public:
	IntelligentAgent();
	IntelligentAgent(const GameWorld &gw);
	virtual ~IntelligentAgent();
	void makeMove();
};


#endif

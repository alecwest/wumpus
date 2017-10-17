
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
	std::vector<int> stenchesFound;
	std::vector<int> moosFound;
	int wumpusRoom;
	int supmuwRoom;
	bool supmuwEvil;
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
	bool safeUnvisitedRoom(int r);
	bool safeRoom(int r);
	// Make inferences about surrounding rooms based on what is seen in the current room
	void inferRooms();
	// Determine which direction to turn and face it (assuming r is an adjacent room)
	void faceRoom(int r);
	// Go to adjacent room
	void goToRoom(int r);
	// Find the closest FRINGE room (that the agent can safely reach) and take that path
	void pathToFringe();
	// Finds the fastest path to a safe Fringe room
	std::queue<Move> depthLimitedSearch(int currRoom, Direction currDir, std::queue<Move> movesToAdd, int depth, bool targetHome);
	// Returns true if wumpus room has been identified
	bool wumpusRoomFound();
	// Returns true if supmuw room has been identified
	bool supmuwRoomFound();
	// Finds path back to safe room
	void returnToSafeRoom();
public:
	IntelligentAgent();
	IntelligentAgent(const GameWorld &gw);
	virtual ~IntelligentAgent();
	void makeMove();
};


#endif

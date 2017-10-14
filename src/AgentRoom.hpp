
/* Generated from GenMyModel */

#ifndef DEF_AGENTROOM
#define DEF_AGENTROOM

#include "Room.hpp"
#include "WorldEnum.hpp"

class AgentRoom : public Room
{
protected:
	RoomStatus status;
	std::vector<Inference> inferences;

public:
	AgentRoom(int room, int worldSize);
	virtual ~AgentRoom();
	AgentRoom* clone();
	RoomStatus getRoomStatus();
	void setRoomStatus(RoomStatus rs);
	std::vector<Inference> getInferences();
	/*
	 * If a free inference is added, remove the corresponding contains inferences if it exists
	 * If a contains inference is added, but it's corresponding free inference is already here, ignore it
	 */
	void addInference(Inference i);
	void removeInference(Inference i);
	bool hasInference(Inference i);
	/*
	 * Returns true if room is pit free, wumpus free, and evil supmuw free and NOT yet visited
	 */
	bool safeUnvisitedRoom();
	/*
	 * Returns true if room is visited
	 */
	bool roomVisited();
};


#endif


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
	void addInference(Inference i);
	bool hasInference(Inference i);
	bool safeRoom();
};


#endif

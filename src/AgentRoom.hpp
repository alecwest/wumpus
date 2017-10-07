
/* Generated from GenMyModel */

#ifndef DEF_AGENTROOM
#define DEF_AGENTROOM

#include "Room.hpp"
#include "WorldEnum.hpp"

class AgentRoom : public Room
{
protected:
	RoomStatus status;

public:
	AgentRoom(int room, int worldSize);
	virtual ~AgentRoom();
	AgentRoom* clone();
	RoomStatus getRoomStatus();
	void setRoomStatus(RoomStatus rs);
};


#endif

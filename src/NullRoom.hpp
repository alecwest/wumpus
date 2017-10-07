/*
 * NullRoom.h
 *
 *  Created on: Oct 5, 2017
 *      Author: alecw
 */

#ifndef SRC_NULLROOM_HPP_
#define SRC_NULLROOM_HPP_

#include "Room.hpp"

class NullRoom : public Room {
public:
	NullRoom();
	virtual ~NullRoom();
};

#endif /* SRC_NULLROOM_HPP_ */

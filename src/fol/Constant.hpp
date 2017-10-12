/*
 * Constant.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_FOL_CONSTANT_HPP_
#define SRC_FOL_CONSTANT_HPP_

#include <sstream>

#include "Term.hpp"

class Constant: public Term {
private:
	int constant;

public:
	Constant();
	Constant(int constant);
	virtual ~Constant();
	void setConstant(int constant);
	int getConstant();
	bool operator==(Term *c);
	std::string toString();
};

#endif /* SRC_FOL_CONSTANT_HPP_ */

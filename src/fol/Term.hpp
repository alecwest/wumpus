/*
 * Term.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_FOL_TERM_HPP_
#define SRC_FOL_TERM_HPP_

#include <iostream>
#include <typeinfo>

class Term {
public:
	Term();
	virtual ~Term();
	virtual bool operator ==(Term *t) = 0;
	virtual std::string toString() = 0;
};

#endif /* SRC_FOL_TERM_HPP_ */

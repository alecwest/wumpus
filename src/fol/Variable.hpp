/*
 * Variable.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_FOL_VARIABLE_HPP_
#define SRC_FOL_VARIABLE_HPP_

#include "Term.hpp"

class Variable: public Term {
private:
	std::string varName;
public:
	Variable();
	Variable(std::string variable);
	virtual ~Variable();
	void setVariable(std::string var);
	std::string getVariable();
	bool operator==(Term *t);
	std::string toString();
};

#endif /* SRC_FOL_VARIABLE_HPP_ */

/*
 * Function.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_FOL_FUNCTION_HPP_
#define SRC_FOL_FUNCTION_HPP_

#include <iostream>

#include "Constant.hpp"
#include "Term.hpp"
#include "Variable.hpp"

class Function: public Term {
private:
	// The name of the function i.e. ADD or SUB
	std::string functionName;
	// The first argument of the function. It can be either a Variable or a Constant.
	Term *first;
	// The second argument is a constant value.
	Constant *second;
public:
	Function();
	Function(std::string name, Term *var, Constant *value);
	virtual ~Function();
	void setName(std::string s);
	std::string getName();
	void setFirst(Term *first);
	Term* getFirst();
	void setSecond(Constant *con);
	Term* getSecond();
	int getAbsoluteNumber();
	bool operator==(Term *f);
	std::string toString();
};

#endif /* SRC_FOL_FUNCTION_HPP_ */

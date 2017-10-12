/*
 * HornLiteral.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_FOL_HORNLITERAL_HPP_
#define SRC_FOL_HORNLITERAL_HPP_

#include <string>
#include <vector>

#include "FOLTools.hpp"
#include "Term.hpp"

class HornLiteral {
private:
	// The name of the current literal
	std::string name;
	// The negation of the clause
	bool negation;
	// Can be either a Variable or a Constant or a Function
	std::vector<Term *>terms;

public:
	HornLiteral();
	HornLiteral(std::string name, bool negation, Term *term1);
	HornLiteral(std::string name, bool negation, Term *term1, Term *term2);
	virtual ~HornLiteral();
	void setName(std::string name);
	std::string getName();
	void setNegation(bool negation);
	bool getNegation();
	void addTerm(Term *term);
	std::vector<Term *> getTerms();
	bool isNegation(HornLiteral literal);
	bool isFact();
	std::string toString();
	bool operator==(HornLiteral hl);
};

#endif /* SRC_FOL_HORNLITERAL_HPP_ */

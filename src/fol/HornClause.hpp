/*
 * HornClause.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_FOL_HORNCLAUSE_HPP_
#define SRC_FOL_HORNCLAUSE_HPP_

#include <vector>

#include "HornLiteral.hpp"

class HornClause {
private:
	std::vector<HornLiteral> myList;
	int numLiterals;

public:
	HornClause();
	virtual ~HornClause();
	void addLiteral(HornLiteral lit);
	int getNumLiterals();
	HornLiteral getLiteralAt(int i);
	// Returns true if the clause is a definite horn clause, false otherwise
	bool isDefiniteHornClause();
	std::string toString();
};

#endif /* SRC_FOL_HORNCLAUSE_HPP_ */

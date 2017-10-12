/*
 * KnowledgeBase.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_KNOWLEDGEBASE_HPP_
#define SRC_KNOWLEDGEBASE_HPP_

#include <vector>

#include "fol/HornClause.hpp"

class KnowledgeBase {
private:
	// Contains horn clauses that are the rules of the game
	std::vector<HornClause> rules;
	// Contains facts
	std::vector<HornLiteral> facts;
public:
	KnowledgeBase();
	virtual ~KnowledgeBase();
	void addFact(HornLiteral hl);
	void addClause(HornClause hc);
	int getNumRules();
	int getNumFacts();
	std::string toString();
};

#endif /* SRC_KNOWLEDGEBASE_HPP_ */

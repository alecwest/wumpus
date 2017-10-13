/*
 * KnowledgeBase.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_KNOWLEDGEBASE_HPP_
#define SRC_KNOWLEDGEBASE_HPP_

#include <vector>

#include "fol/Constant.hpp"
#include "fol/Function.hpp"
#include "fol/HornClause.hpp"
#include "fol/Unifier.hpp"
#include "fol/Variable.hpp"

class KnowledgeBase {
private:
	// Contains horn clauses that are the rules of the game
	std::vector<HornClause> rules;
	// Contains facts
	std::vector<HornLiteral> facts;
	// Contains length/width of grid
	int size;

	// checks if the literal is an already known fact
	bool knownFact(HornLiteral literal);
	// Checks if the square is an ok square
	bool ok(HornLiteral literal);
	// updates the knowledge base with new inferences
	void updateKnowledgeBase(int size);
	// check if there is a pit or a wumpus in the specified position
	void ask(HornLiteral literal);
	Unifier* unifyOps(LiteralSymbol x, LiteralSymbol y, Unifier *theta);
	Unifier* unifyVar(Variable *var, Term *x, Unifier *theta);
	// Checks if variavle var exists in clause x
	bool occurCheck(Variable *var, Term *x, Unifier *theta);
	Unifier* cascadeSubstitution(Unifier *theta, Variable *var, Term *x);
	void threeSquareCheck(HornLiteral curSquare, HornLiteral distantLit, HornLiteral distantCheck, HornLiteral closeCheck1, HornLiteral closeCheck2, HornLiteral toAdd);
	void twoSquareCheck(HornLiteral curSquare, HornLiteral distantLit, HornLiteral distantCheck, HornLiteral closeCheck, HornLiteral toAdd);
public:
	KnowledgeBase(int size);
	virtual ~KnowledgeBase();
	void addFact(HornLiteral hl);
	void addClause(HornClause hc);
	int getNumRules();
	int getNumFacts();
	std::string toString();
	Answer ask(HornLiteral literal, int size);
	// Returns the wumpus location, or -1 if we don't know it yet
	int wumpusPosition();
	// Tells the KB that wumpus is dead. We remove it and the stenches from KB
	// TODO do the same for Supmuw and moo
	void wumpusIsDead();
	template <class T>
	Unifier* unify(T ob1, T ob2, Unifier *theta);
	Unifier* unify(std::vector<Term *> list1, std::vector<Term *> list2, Unifier *theta);

};

#endif /* SRC_KNOWLEDGEBASE_HPP_ */

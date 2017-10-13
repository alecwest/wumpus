/*
 * HornLiteral.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#include "HornLiteral.hpp"

HornLiteral::HornLiteral() {
	name = LiteralSymbol::OK;
	negation = true;
	terms = std::vector<Term *>();
}

HornLiteral::HornLiteral(LiteralSymbol name, bool negation, Term* term1) {
	this->name = name;
	this->negation = negation;
	terms.push_back(term1);
}

HornLiteral::HornLiteral(LiteralSymbol name, bool negation, Term* term1,
		Term* term2) {
	this->name = name;
	this->negation = negation;
	terms.push_back(term1);
	terms.push_back(term2);
}

HornLiteral::~HornLiteral() {
	for (auto t : terms) {
		delete t;
	}
}

void HornLiteral::setName(LiteralSymbol name) {
	this->name = name;
}

LiteralSymbol HornLiteral::getName() {
	return name;
}

void HornLiteral::setNegation(bool negation) {
	this->negation = negation;
}

bool HornLiteral::getNegation() {
	return negation;
}

void HornLiteral::addTerm(Term* term) {
	terms.push_back(term);
}

std::vector<Term*> HornLiteral::getTerms() {
	return terms;
}

bool HornLiteral::isNegation(HornLiteral literal) {
	return (name == literal.getName())
			&& (compareVectors(terms, literal.getTerms()))
			&& (negation != literal.getNegation());
}

// Returns true if the literal is a horn fact
bool HornLiteral::isFact() {
	return negation;
}

std::string HornLiteral::toString() {
	std::string neg = (negation) ? "" : "not ";
	std::string args = "";

	for (auto t : terms) {
		if(args != "")
			args += ",";
		args += t->toString();
	}
	return neg + ::toString(name) + " (" + args + ")";
}

bool HornLiteral::operator ==(HornLiteral hl) {
	return (hl.getName() == name)
			&& (hl.getNegation() == negation)
			&& (compareVectors(terms, hl.getTerms()));
}

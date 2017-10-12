/*
 * KnowledgeBase.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#include "KnowledgeBase.hpp"

KnowledgeBase::KnowledgeBase() {
	rules = std::vector<HornClause>();
	facts = std::vector<HornLiteral>();
}

KnowledgeBase::~KnowledgeBase() {}

void KnowledgeBase::addFact(HornLiteral hl) {
	if (hl.isFact())
		facts.push_back(hl);
}

void KnowledgeBase::addClause(HornClause hc) {
	if (hc.isDefiniteHornClause())
		rules.push_back(hc);
}

int KnowledgeBase::getNumRules() {
	return rules.size();
}

int KnowledgeBase::getNumFacts() {
	return facts.size();
}

std::string KnowledgeBase::toString() {
	std::string result = "";
	result += "FACTS: ";
	for (auto hl : facts) {
		result += "\n\n\t" + hl.toString();
	}
	result += "\n\n\nRULES: ";
	for (auto hc : rules) {
		result += "\n\n\t" + hc.toString();
	}
	result += "\n\n";
	return result;
}

/*
 * HornClause.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#include "HornClause.hpp"

HornClause::HornClause() {
	myList = std::vector<HornLiteral>();
	numLiterals = 0;
}

HornClause::~HornClause() {}

void HornClause::addLiteral(HornLiteral lit) {
	myList.push_back(lit);
	numLiterals++;
}

int HornClause::getNumLiterals() {
	return numLiterals;
}

HornLiteral HornClause::getLiteralAt(int i) {
	return myList.at(i);
}

bool HornClause::isDefiniteHornClause() {
	int numFalseLiterals = 0;
	int numTrueLiterals = 0;

	for(auto literal : myList) {
		if (literal.getNegation())
			numTrueLiterals++;
		else
			numFalseLiterals++;
	}
	return (numTrueLiterals == 1)
			&& (numFalseLiterals == (int) myList.size() - 1);
}

std::string HornClause::toString() {
	std::string hypothesis = "";
	std::string inference = "";

	for (auto literal : myList) {
		if (!literal.getNegation()) {
			if (hypothesis != "")
				hypothesis += " ^ "; // TODO is this denoting AND?
			literal.setNegation(true);
			hypothesis += literal.toString();
			literal.setNegation(false);
		}
		else {
			inference += literal.toString();
		}
	}
	return hypothesis + " => " + inference;
}

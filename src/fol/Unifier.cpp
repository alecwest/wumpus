/*
 * Unifier.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#include "Unifier.hpp"

Unifier::Unifier() {
	theta = std::tr1::unordered_map<Term *, Term *>();
}

Unifier::~Unifier() {}

void Unifier::addSubstitution(Term* key, Term* subst) {
	theta.insert(std::make_pair(key, subst));
}

bool Unifier::containsSubstitution(Term* key) {
	return theta.find(key) != theta.end();
}

Term* Unifier::getSubstitution(Term* key) {
	std::tr1::unordered_map<Term *, Term *>::const_iterator r = theta.find(key);
	return r->second;
}

std::vector<Term*> Unifier::getKeys() {
	std::vector<Term *> keys;
	for (auto t : theta) {
		keys.push_back(t.first);
	}
	return keys;
}

std::string Unifier::toString() {
	std::string result = "";
	for (auto t : getKeys()) {
		if (result != "")
			result += " , ";
		result += t->toString() + "/" + getSubstitution(t)->toString();
	}
	return "{ " + result + " } ";
}

/*
 * Unifier.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_FOL_UNIFIER_HPP_
#define SRC_FOL_UNIFIER_HPP_

#include <tr1/unordered_map>
#include <vector>

#include "Term.hpp"

class Unifier {
private:
	std::tr1::unordered_map<Term *, Term *> theta;
public:
	Unifier();
	virtual ~Unifier();
	void addSubstitution(Term *key, Term *subst);
	bool containsSubstitution(Term *key);
	Term* getSubstitution(Term *key);
	std::vector<Term *> getKeys();
	std::string toString();
};

#endif /* SRC_FOL_UNIFIER_HPP_ */

/*
 * KnowledgeBase.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#include "KnowledgeBase.hpp"

KnowledgeBase::KnowledgeBase(int size) {
	this->size = size;
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

bool KnowledgeBase::knownFact(HornLiteral literal) {
	for (auto lit : facts) {
		if (lit == literal) return true;
	}
	return false;
}

bool KnowledgeBase::ok(HornLiteral literal) {
	LiteralSymbol litName = (literal.getName() == LiteralSymbol::WUMPUS) ?
			LiteralSymbol::WUMPUS_FREE : LiteralSymbol::PIT_FREE;
	Constant *const1 = (Constant *)literal.getTerms().at(0);
	Constant *const2 = (Constant *)literal.getTerms().at(1);

	HornLiteral lit1 = HornLiteral(LiteralSymbol::OK, true, const1, const2);
	HornLiteral lit2 = HornLiteral(litName, true, const1, const2);

	for (auto fact : facts) {
		if (fact == lit1 || fact == lit2) return true;
	}
	return false;
}

void KnowledgeBase::updateKnowledgeBase(int size) {
	LiteralSymbol toAsk[] = {LiteralSymbol::WUMPUS, LiteralSymbol::PIT,
							 LiteralSymbol::SUPMUW, LiteralSymbol::SUPMUW_EVIL};
	for(LiteralSymbol ls : toAsk) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				// TODO if the two constants here are a position, this needs to be changed to one dimensional
				HornLiteral literal = HornLiteral(ls, true, new Constant(i), new Constant(j));
				ask(literal);
			}
		}
	}
}

void KnowledgeBase::ask(HornLiteral literal) {
	LiteralSymbol toLook;
	LiteralSymbol litName;

	Constant *const1 = (Constant *)literal.getTerms().at(0);
	Constant *const2 = (Constant *)literal.getTerms().at(1);

	int row = const1->getConstant();
	int col = const2->getConstant();

	switch (literal.getName()) {
	case LiteralSymbol::WUMPUS:
		toLook = LiteralSymbol::STENCH;
		litName = LiteralSymbol::WUMPUS_FREE;
		break;
	case LiteralSymbol::PIT:
		toLook = LiteralSymbol::BREEZE;
		litName = LiteralSymbol::PIT_FREE;
		break;
	case LiteralSymbol::SUPMUW:
		toLook = LiteralSymbol::MOO;
		litName = LiteralSymbol::SUPMUW_FREE;
		break;
	case LiteralSymbol::SUPMUW_EVIL:
		toLook = LiteralSymbol::MOO;
		litName = LiteralSymbol::SUPMUW_EVIL_FREE;
		break;
	default:
		toLook = LiteralSymbol::OK;
		litName = LiteralSymbol::OK;
		break;
	}

	// Literals for Stench / Breeze/ Moo checks.
	// TODO add ne, nw, se, sw checks for Moo
	HornLiteral litNorth = HornLiteral(toLook, true, new Constant(row + 1), new Constant(col));
	HornLiteral litSouth = HornLiteral(toLook, true, new Constant(row - 1), new Constant(col));
	HornLiteral litEast = HornLiteral(toLook, true, new Constant(row), new Constant(col + 1));
	HornLiteral litWest = HornLiteral(toLook, true, new Constant(row + 1), new Constant(col - 1));

	// Literals for inBounds checks.
	HornLiteral inBoundsNN = HornLiteral(LiteralSymbol::IN_BOUNDS, true, new Constant(row + 2), new Constant(col));
	HornLiteral inBoundsSS = HornLiteral(LiteralSymbol::IN_BOUNDS, true, new Constant(row - 2), new Constant(col));
	HornLiteral inBoundsEE = HornLiteral(LiteralSymbol::IN_BOUNDS, true, new Constant(row), new Constant(col + 2));
	HornLiteral inBoundsWW = HornLiteral(LiteralSymbol::IN_BOUNDS, true, new Constant(row), new Constant(col - 2));

	// Square literals
	HornLiteral center = HornLiteral(LiteralSymbol::C_SQUARE, true, new Constant(row), new Constant(col));
	HornLiteral north = HornLiteral(LiteralSymbol::N_SQUARE, true, new Constant(row), new Constant(col));
	HornLiteral south = HornLiteral(LiteralSymbol::S_SQUARE, true, new Constant(row), new Constant(col));
	HornLiteral east = HornLiteral(LiteralSymbol::E_SQUARE, true, new Constant(row), new Constant(col));
	HornLiteral west = HornLiteral(LiteralSymbol::W_SQUARE, true, new Constant(row), new Constant(col));
	HornLiteral northEast = HornLiteral(LiteralSymbol::NE_SQUARE, true, new Constant(row), new Constant(col));
	HornLiteral northWest = HornLiteral(LiteralSymbol::NW_SQUARE, true, new Constant(row), new Constant(col));
	HornLiteral southEast = HornLiteral(LiteralSymbol::SE_SQUARE, true, new Constant(row), new Constant(col));
	HornLiteral southWest = HornLiteral(LiteralSymbol::SW_SQUARE, true, new Constant(row), new Constant(col));

	// Check center square
	if (knownFact(center)) {
		// Declare circle of literals for squares adjacent to center's orthogonally adjacent squares
		// TODO this checks diagonally adjacent squares which would be a problem for the Supmuw Moo
		HornLiteral tempSS = HornLiteral(litName, true, new Constant(row - 2), new Constant(col));
		HornLiteral tempSW = HornLiteral(litName, true, new Constant(row - 1), new Constant(col - 1));
		HornLiteral tempWW = HornLiteral(litName, true, new Constant(row), new Constant(col - 2));
		HornLiteral tempNW = HornLiteral(litName, true, new Constant(row + 1), new Constant(col - 1));
		HornLiteral tempNN = HornLiteral(litName, true, new Constant(row + 2), new Constant(col));
		HornLiteral tempNE = HornLiteral(litName, true, new Constant(row + 1), new Constant(col + 1));
		HornLiteral tempEE = HornLiteral(litName, true, new Constant(row), new Constant(col + 2));
		HornLiteral tempSE = HornLiteral(litName, true, new Constant(row - 1), new Constant(col + 1));

		threeSquareCheck(litNorth, inBoundsNN, tempNN, tempNW, tempNE, literal);
		threeSquareCheck(litSouth, inBoundsSS, tempSS, tempSW, tempSE, literal);
		threeSquareCheck(litEast, inBoundsEE, tempEE, tempNE, tempSE, literal);
		threeSquareCheck(litWest, inBoundsWW, tempWW, tempNW, tempSW, literal);
	}
	else if (knownFact(east)) {
		// TODO some of the logic in the original is kinda wack... the first else if is for the left side but declares
		// variables as if it were for the right side (east). I'm changing west to east in the conditional, nothing else
		HornLiteral tempSS = HornLiteral(litName, true, new Constant(row - 2), new Constant(col));
		HornLiteral tempSE = HornLiteral(litName, true, new Constant(row - 1), new Constant(col + 1));
		HornLiteral tempEE = HornLiteral(litName, true, new Constant(row), new Constant(col + 2));
		HornLiteral tempNE = HornLiteral(litName, true, new Constant(row + 1), new Constant(col + 1));
		HornLiteral tempNN = HornLiteral(litName, true, new Constant(row + 2), new Constant(col));

		twoSquareCheck(litSouth, inBoundsSS, tempSS, tempSE, literal);
		twoSquareCheck(litNorth, inBoundsNN, tempNN, tempNE, literal);
		threeSquareCheck(litEast, inBoundsEE, tempEE, tempSE, tempNE, literal); // TODO see line 425 if this misbehaves
	}
	else if (knownFact(west)) {
		// TODO changed from right side (east) to left side (west)
		HornLiteral tempNN = HornLiteral(litName, true, new Constant(row + 2), new Constant(col));
		HornLiteral tempSW = HornLiteral(litName, true, new Constant(row - 1), new Constant(col - 1));
		HornLiteral tempWW = HornLiteral(litName, true, new Constant(row), new Constant(col - 2));
		HornLiteral tempNW = HornLiteral(litName, true, new Constant(row + 1), new Constant(col - 1));
		HornLiteral tempSS = HornLiteral(litName, true, new Constant(row - 2), new Constant(col));

		twoSquareCheck(litSouth, inBoundsSS, tempSS, tempSW, literal);
		twoSquareCheck(litNorth, inBoundsNN, tempNN, tempNW, literal);
		threeSquareCheck(litWest, inBoundsWW, tempWW, tempSW, tempNW, literal); // TODO see line 495 if this misbehaves
	}
	else if (knownFact(north)) {
		// TODO this one seems to be right
		HornLiteral tempWW = HornLiteral(litName, true, new Constant(row), new Constant(col - 2));
		HornLiteral tempNW = HornLiteral(litName, true, new Constant(row + 1), new Constant(col - 1));
		HornLiteral tempNN = HornLiteral(litName, true, new Constant(row + 2), new Constant(col));
		HornLiteral tempNE = HornLiteral(litName, true, new Constant(row + 1), new Constant(col + 1));
		HornLiteral tempEE = HornLiteral(litName, true, new Constant(row), new Constant(col + 2));

		threeSquareCheck(litNorth, inBoundsNN, tempNN, tempNW, tempNE, literal); // TODO line 525
		twoSquareCheck(litEast, inBoundsEE, tempEE, tempNE, literal);
		twoSquareCheck(litWest, inBoundsWW, tempWW, tempNW, literal);
	}
	else if (knownFact(south)) {
		HornLiteral tempWW = HornLiteral(litName, true, new Constant(row), new Constant(col - 2));
		HornLiteral tempSW = HornLiteral(litName, true, new Constant(row - 1), new Constant(col - 1));
		HornLiteral tempSS = HornLiteral(litName, true, new Constant(row - 2), new Constant(col));
		HornLiteral tempSE = HornLiteral(litName, true, new Constant(row - 1), new Constant(col + 1));
		HornLiteral tempEE = HornLiteral(litName, true, new Constant(row), new Constant(col + 2));

		threeSquareCheck(litSouth, inBoundsSS, tempSS, tempSW, tempSE, literal); // TODO line 595
		twoSquareCheck(litEast, inBoundsEE, tempEE, tempSE, literal);
		twoSquareCheck(litWest, inBoundsWW, tempWW, tempSW, literal);
	}
	else if (knownFact(northEast)) {
		// TODO was top left (north west)
		HornLiteral tempEE = HornLiteral(litName, true, new Constant(row), new Constant(col + 2));
		HornLiteral tempNE = HornLiteral(litName, true, new Constant(row + 1), new Constant(col + 1));
		HornLiteral tempNN = HornLiteral(litName, true, new Constant(row + 2), new Constant(col));

		twoSquareCheck(litNorth, inBoundsNN, tempNN, tempNE, literal); // TODO line 659
		twoSquareCheck(litEast, inBoundsEE, tempEE, tempNE, literal); // TODO line 667
	}
	else if (knownFact(northWest)) {
		// TODO was top right (north east)
		HornLiteral tempWW = HornLiteral(litName, true, new Constant(row), new Constant(col - 2));
		HornLiteral tempNW = HornLiteral(litName, true, new Constant(row + 1), new Constant(col - 1));
		HornLiteral tempNN = HornLiteral(litName, true, new Constant(row + 2), new Constant(col));

		twoSquareCheck(litNorth, inBoundsNN, tempNN, tempNW, literal); // TODO line 691
		twoSquareCheck(litWest, inBoundsWW, tempWW, tempNW, literal); // TODO line 699
	}
	else if (knownFact(southEast)) {
		// TODO was bottom left (south west)
		HornLiteral tempSS = HornLiteral(litName, true, new Constant(row - 2), new Constant(col));
		HornLiteral tempSE = HornLiteral(litName, true, new Constant(row - 1), new Constant(col + 1));
		HornLiteral tempEE = HornLiteral(litName, true, new Constant(row), new Constant(col + 2));

		twoSquareCheck(litSouth, inBoundsSS, tempSS, tempSE, literal); // TODO line 725
		twoSquareCheck(litEast, inBoundsEE, tempEE, tempSE, literal); // TODO line 731
	}
	else if (knownFact(southWest)) {
		// TODO was bottom right (south east)
		HornLiteral tempWW = HornLiteral(litName, true, new Constant(row), new Constant(col - 2));
		HornLiteral tempSW = HornLiteral(litName, true, new Constant(row - 1), new Constant(col - 1));
		HornLiteral tempSS = HornLiteral(litName, true, new Constant(row - 2), new Constant(col));

		twoSquareCheck(litSouth, inBoundsSS, tempSS, tempSW, literal); // TODO line 755
		twoSquareCheck(litWest, inBoundsWW, tempWW, tempSW, literal); // TODO line 763
	}

}

// TODO was originally line 848
Unifier* KnowledgeBase::unify(std::vector<Term *> list1, std::vector<Term *> list2, Unifier *theta) {
	if(list1.size() != list2.size()) return NULL;
	else if (list1.size() == 0 && list2.size() == 0) return theta;
	else if (list1.size() == 1 && list2.size() == 1) return unify(list1.at(0), list2.at(0), theta);
	else {
		std::vector<Term *> a = std::vector<Term *>(list1.begin() + 1, list1.end());
		std::vector<Term *> b = std::vector<Term *>(list2.begin() + 1, list2.end());
		return unify(a, b, unify(list1.at(0), list2.at(0), theta));
	}
}

template<class T>
Unifier* KnowledgeBase::unify(T ob1, T ob2, Unifier *theta) {
	if (theta==NULL) return NULL;
	else if (ob1 == ob2) return theta;
	else if (typeid(ob1) == typeid(Variable)) return unifyVar((Variable*)ob1, (Term*)ob2, theta);
	else if (typeid(ob2) == typeid(Variable)) return unifyVar((Variable*)ob2, (Term*)ob1, theta);
	else if (typeid(ob1) == typeid(HornLiteral) && (typeid(HornLiteral) == typeid(ob2))) {
		HornLiteral *lit1 = (HornLiteral *)ob1;
		HornLiteral *lit2 = (HornLiteral *)ob2;

		std::vector<Term *> xList = std::vector<Term *>(lit1->getTerms());
		std::vector<Term *> yList = std::vector<Term *>(lit2->getTerms());

		return unify(&xList, &yList, unifyOps(lit1->getName(), lit2->getName(), theta));
	}
	else return NULL;
}

Unifier* KnowledgeBase::unifyOps(LiteralSymbol x, LiteralSymbol y, Unifier *theta) {
	if (theta == NULL) return NULL;
	else if (x == y) return theta;
	else return NULL;
}

Unifier* KnowledgeBase::unifyVar(Variable* var, Term* x, Unifier *theta) {
	if (theta->containsSubstitution(var)) {
		return unify(theta->getSubstitution(var), x, theta);
	}
	else if (theta->containsSubstitution(x)) {
		return unify(var, (Variable *)theta->getSubstitution(x), theta);
	}
	else if (occurCheck(var, x, theta)) {
		return NULL;
	}
	else {
		return cascadeSubstitution(theta, var, x);
	}

}

// This method checks if variable var exists in clause x
bool KnowledgeBase::occurCheck(Variable* var, Term* x, Unifier *theta) {
	if (var == (Variable *)x) return true;
	else if (theta->containsSubstitution(x)) {
		return occurCheck(var, theta->getSubstitution(x), theta);
	}
	else if (typeid(x) == typeid(Function)) {
		Function *f = (Function *)x;
		Term *ob1 = f->getFirst();
		Term *ob2 = f->getSecond();
		if (occurCheck(var, ob1, theta) || occurCheck(var, ob2, theta)) return true;
	}
	return false;
}

Unifier* KnowledgeBase::cascadeSubstitution(Unifier *theta, Variable* var,
		Term* x) {
	theta->addSubstitution(var, x);
	// update the variables into the functions if necessary
	for (Term *t : theta->getKeys()) {
		Term *term = theta->getSubstitution(t);
		if (typeid(term) == typeid(Function *)) {
			if (theta->containsSubstitution(((Function *) term)->getFirst())) {
				((Function *) term)->setFirst(theta->getSubstitution(((Function *) term)->getFirst()));
			}
		}
	}
	return theta;
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

Answer KnowledgeBase::ask(HornLiteral literal, int size) {
	updateKnowledgeBase(size);
	if (ok(literal)) return Answer::FALSE;
	if (knownFact(literal)) return Answer::TRUE;
	return Answer::UNKNOWN;
}

// returns the wumpus position, or -1 if we don't know it yet
int KnowledgeBase::wumpusPosition() {
	for (auto lit: facts) {
		if (lit.getName() == LiteralSymbol::WUMPUS && lit.getNegation()) {
			Constant *row = (Constant *) lit.getTerms().at(0);
			Constant *col = (Constant *) lit.getTerms().at(1);
			return (row->getConstant() - 1) * size + (col->getConstant() - 1);  // TODO instead of removing row/col mindset in knowledge base, translate it to one dimensional when returned
		}
	}
	return -1;
}

void KnowledgeBase::threeSquareCheck(HornLiteral curSquare,
		HornLiteral distantLit, HornLiteral distantCheck,
		HornLiteral closeCheck1, HornLiteral closeCheck2,
		HornLiteral toAdd) {
	if (knownFact(curSquare)) {
		if (knownFact(distantLit)) {
			if (knownFact(distantCheck) && knownFact(closeCheck1)  && knownFact(closeCheck2))
				addFact(toAdd);
		}
		else {
			if (knownFact(closeCheck1) && knownFact(closeCheck2))
				addFact(toAdd);
		}
	}
}

void KnowledgeBase::twoSquareCheck(HornLiteral curSquare,
		HornLiteral distantLit, HornLiteral distantCheck,
		HornLiteral closeCheck, HornLiteral toAdd) {
	if (knownFact(curSquare)) {
		if (knownFact(distantLit)) {
			if (knownFact(distantCheck) && knownFact(closeCheck))
				addFact(toAdd);
		}
		else {
			if (knownFact(closeCheck))
				addFact(toAdd);
		}
	}
}

// tells the KB that wumpus is dead. Remove it and the stenches from the KB
void KnowledgeBase::wumpusIsDead() {
	std::vector<HornLiteral> temp = std::vector<HornLiteral>();
	for(auto lit: facts) {
		if ((lit.getName() != LiteralSymbol::WUMPUS || !lit.getNegation())
				&& lit.getName() != LiteralSymbol::STENCH) {
			temp.push_back(lit);
		}
	}
	facts.clear();
	facts = temp;
}

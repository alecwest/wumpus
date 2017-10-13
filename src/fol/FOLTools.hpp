/*
 * FOLEnum.hpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#ifndef SRC_FOL_FOLTOOLS_HPP_
#define SRC_FOL_FOLTOOLS_HPP_

#include <algorithm>
#include <iostream>
#include <vector>

/*
 * TODO give credit to
 * https://stackoverflow.com/questions/17394149/how-to-efficiently-compare-vectors-with-c/17394298#17394298
 * for this method
 * May not be necessary to use if sorting isn't an actual issue
 */
template <class T>
static bool compareVectors(std::vector<T> a, std::vector<T> b)
{
   if (a.size() != b.size())
   {
      return false;
   }
   std::sort(a.begin(), a.end());
   std::sort(b.begin(), b.end());
   return (a == b);
}

enum class Answer { TRUE, FALSE, UNKNOWN };
inline std::string toString(Answer a) {
	switch (a) {
	case Answer::TRUE: return "True";
	case Answer::FALSE: return "False";
	case Answer::UNKNOWN: return "Unknown";
	default: return "";
	}
}

enum class FunctionSymbol { ADD, SUB };
inline std::string toString(FunctionSymbol fs) {
	switch(fs) {
	case FunctionSymbol::ADD: return "Add";
	case FunctionSymbol::SUB: return "Sub";
	default: return "";
	}
}

// TODO add scream here?
enum class LiteralSymbol {
	WUMPUS, SUPMUW, SUPMUW_EVIL, STENCH, PIT, BREEZE,
	MOO, OK, WUMPUS_FREE, SUPMUW_FREE, SUPMUW_EVIL_FREE, PIT_FREE,
	C_SQUARE, W_SQUARE, E_SQUARE, N_SQUARE, S_SQUARE,
	NE_SQUARE, NW_SQUARE, SE_SQUARE, SW_SQUARE,
	IN_BOUNDS, OUT_BOUNDS};
inline std::string toString(LiteralSymbol ls) {
	switch (ls) {
	case LiteralSymbol::WUMPUS: return "Wumpus";
	case LiteralSymbol::SUPMUW: return "Supmuw";
	case LiteralSymbol::SUPMUW_EVIL: return "Evil Supmuw";
	case LiteralSymbol::STENCH: return "Stench";
	case LiteralSymbol::PIT: return "Pit";
	case LiteralSymbol::BREEZE: return "Breeze";
	case LiteralSymbol::MOO: return "Moo";
	case LiteralSymbol::OK: return "Ok";
	case LiteralSymbol::WUMPUS_FREE: return "Wumpus Free";
	case LiteralSymbol::SUPMUW_FREE: return "Supmuw Free";
	case LiteralSymbol::SUPMUW_EVIL_FREE: return "Evil Supmuw Free";
	case LiteralSymbol::PIT_FREE: return "Pit Free";
	case LiteralSymbol::C_SQUARE: return "Center Square";
	case LiteralSymbol::W_SQUARE: return "West Square";
	case LiteralSymbol::E_SQUARE: return "East Square";
	case LiteralSymbol::N_SQUARE: return "North Square";
	case LiteralSymbol::S_SQUARE: return "South Square";
	case LiteralSymbol::NE_SQUARE: return "North East Square";
	case LiteralSymbol::NW_SQUARE: return "North West Square";
	case LiteralSymbol::SE_SQUARE: return "South East Square";
	case LiteralSymbol::SW_SQUARE: return "South West Square";
	case LiteralSymbol::IN_BOUNDS: return "In Bounds";
	case LiteralSymbol::OUT_BOUNDS: return "Out of Bounds";
	default: return "";
	}
}


#endif /* SRC_FOL_FOLTOOLS_HPP_ */

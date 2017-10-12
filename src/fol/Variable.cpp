/*
 * Variable.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#include "Variable.hpp"

Variable::Variable() {
	varName = "";
}

Variable::~Variable() {}

Variable::Variable(std::string variable) {
	varName = variable;
}

void Variable::setVariable(std::string var) {
	varName = var;
}

std::string Variable::getVariable() {
	return varName;
}

bool Variable::operator ==(Term* t) {
	if (typeid(this) != typeid(t))
		return false;
	Variable *v = (Variable *)t;
	return v->getVariable() == varName;
}

std::string Variable::toString() {
	return varName;
}

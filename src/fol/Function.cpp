/*
 * Function.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#include "Function.hpp"

Function::Function() {
	functionName = "ADD";
	first = NULL;
	second = NULL;
}

Function::Function(std::string name, Term *var, Constant *value) {
	functionName = name;
	first = var;
	second = value;
}

Function::~Function() {}

void Function::setName(std::string s) {
	functionName = s;
}

std::string Function::getName() {
	return functionName;
}

void Function::setFirst(Term *first) {
	this->first = first;
}

Term* Function::getFirst() {
	return first;
}

void Function::setSecond(Constant *con) {
	second = con;
}

Term* Function::getSecond() {
	return second;
}

int Function::getAbsoluteNumber() {
	// Can't return a value if first is a variable
	if (typeid(first) == typeid(Variable))
		return -1;
	else if (typeid(first) == typeid(Constant)) {
		Constant *t = (Constant *)first;
		return second->getConstant() + t->getConstant();
	}
	else
		return -1;
}

bool Function::operator ==(Term *t) {
	if (typeid(this) != typeid(t))
		return false;
	Function *f = (Function *)t;
	return (f->getName() == functionName)
			&& (f->getFirst() == first)
			&& (f->getSecond() == second);
}

std::string Function::toString() {
	return functionName + "(" + first->toString() + ", " + second->toString() + ")";
}

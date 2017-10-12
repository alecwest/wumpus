/*
 * Constant.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: alecwest
 */

#include "Constant.hpp"

Constant::Constant() {
	constant = 0;
}

Constant::Constant(int constant) {
	this->constant = constant;
}

Constant::~Constant() {}

void Constant::setConstant(int constant) {
	this->constant = constant;
}

int Constant::getConstant() {
	return constant;
}

bool Constant::operator ==(Term *t) {
	if (typeid(this) != typeid(t))
		return false;
	Constant *c = (Constant *)t;
	return c->getConstant() == this->constant;
}

std::string Constant::toString() {
	std::ostringstream ss;
	ss << constant;
	return ss.str();
}

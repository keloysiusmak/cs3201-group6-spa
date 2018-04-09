#pragma once

#include "Clause.h"
#include "Utils.h"

using namespace std;

Clause::Clause() {

}

Clause::Clause(RelRef relRef, Param firstParam, Param secondParam, bool invert) {
	_relRef = relRef;
	_leftParam = firstParam;
	_rightParam = secondParam;
	isInverted = invert;
}

void Clause::setClause(RelRef relRef, Param firstParam, Param secondParam, bool invert) {
	_relRef = relRef;
	_leftParam = firstParam;
	_rightParam = secondParam;
	isInverted = invert;
}

RelRef Clause::getRelRef() {
	return _relRef;
}

Param Clause::getLeftParam() {
	return _leftParam;
}

Param Clause::getRightParam() {
	return _rightParam;
}

bool Clause::getIsInverted() {
	return isInverted;
}

bool Clause::hasSynonymsNotALL() {
	if (Utils::isSynonym(_leftParam) && _leftParam.type != ALL) return true;
	if (Utils::isSynonym(_rightParam) && _rightParam.type != ALL) return true;

	if (_relRef == NONE) { // Pattern
		if (Utils::isSynonym(_entity) && _entity.type != ALL) return true;
	}

	return false;
}

void Clause::setRelRef(RelRef rel) {
	_relRef = rel;
};

void Clause::setLHS(Param lhs) {
	_leftParam = lhs;
};

void Clause::setRHS(Param rhs) {
	_rightParam = rhs;
};

void Clause::setInverted() {
	isInverted = !isInverted;
}

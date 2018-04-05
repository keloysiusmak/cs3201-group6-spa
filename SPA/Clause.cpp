#pragma once

#include "Clause.h"

using namespace std;

Clause::Clause() {

}

Clause::Clause(RelRef relRef, Param firstParam, Param secondParam) {
	_relRef = relRef;
	_leftParam = firstParam;
	_rightParam = secondParam;
}

void Clause::setClause(RelRef relRef, Param firstParam, Param secondParam) {
	_relRef = relRef;
	_leftParam = firstParam;
	_rightParam = secondParam;
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

bool Clause::hasSynonymsNotALL() {
	if (Utils::isSynonym(_leftParam) && _leftParam.type != ALL) return true;
	if (Utils::isSynonym(_rightParam) && _rightParam.type != ALL) return true;

	if (_relRef == NONE) { // Pattern
		if (Utils::isSynonym(_entity) && _entity.type != ALL) return true;
	}

	return false;
}

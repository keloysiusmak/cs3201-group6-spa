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

#pragma once

#include "Clause.h"

using namespace std;

Clause::Clause() {

}

Clause::Clause(RelRef relRef, Param firstParam, Param secondParam) {
	_relRef = relRef;
	_firstParam = firstParam;
	_secondParam = secondParam;
}

void Clause::setClause(RelRef relRef, Param firstParam, Param secondParam) {

}

RelRef Clause::getRelRef() {
	return _relRef;
}

Param Clause::getFirstParam() {
	return _firstParam;
}

Param Clause::getSecondParam() {
	return _secondParam;
}

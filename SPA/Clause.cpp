#pragma once

#include "SuchThat.h"

using namespace std;

Clause::Clause() {

}

void Clause::setClause(string relRef, string firstParam, string secondParam) {

}

string Clause::getRelRef() {
	return _relRef;
}

string Clause::getFirstParam() {
	return _firstParam;
}

string Clause::getSecondParam() {
	return _secondParam;
}

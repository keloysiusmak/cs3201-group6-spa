#pragma once

#include "Pattern.h"

using namespace std;

Pattern::Pattern() {

}

void Pattern::setPattern(string entityType, Param leftParam, Param rightParam) {

}

string Pattern::getEntityType() {
	return _entityType;
}

Param Pattern::getLeftParam() {
	return _leftParam;
}

Param Pattern::getRightParam() {
	return _rightParam;
}
#pragma once

#include "Pattern.h"

using namespace std;

Pattern::Pattern() {
	_relRef = None;
}

Pattern::Pattern(Param entity, Param leftParam, Param rightParam) {
	_relRef = None;
	_entity = entity;
	_leftParam = leftParam;
	_rightParam = rightParam;
}

void Pattern::setPattern(Param entity, Param leftParam, Param rightParam) {
	
}

Param Pattern::getEntity() {
	return this->_entity;
}

Param Pattern::getLeftParam() {
	return _leftParam;
}

Param Pattern::getRightParam() {
	return _rightParam;
}
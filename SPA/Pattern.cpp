#pragma once

#include "Pattern.h"

using namespace std;

Pattern::Pattern() {
	_relRef = None;
}

Pattern::Pattern(Param entity, Param leftParam, Param rightParam, bool invert) {
	_relRef = None;
	_entity = entity;
	_leftParam = leftParam;
	_rightParam = rightParam;
	isInverted = invert;
}

void Pattern::setPattern(Param entity, Param leftParam, Param rightParam, bool invert) {
	_relRef = None;
	_entity = entity;
	_leftParam = leftParam;
	_rightParam = rightParam;
	isInverted = invert;
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
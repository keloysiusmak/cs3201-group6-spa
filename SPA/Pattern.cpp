#pragma once

#include "Pattern.h"

using namespace std;

Pattern::Pattern() {

}

Pattern::Pattern(Param entity, Param leftParam, Param rightParam) {
	_entity = entity;
	_leftParam = leftParam;
	_rightParam = rightParam;
}

void Pattern::setPattern(Param entity, Param leftParam, Param rightParam) {
	
}

Param Pattern::getEntity() {
	return _entity;
}

Param Pattern::getLeftParam() {
	return _leftParam;
}

Param Pattern::getRightParam() {
	return _rightParam;
}
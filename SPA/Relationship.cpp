#include "Relationship.h"

Relationship::Relationship() {

}

Relationship::Relationship(int numOfArg, unordered_set<ParamType> arg1, unordered_set<ParamType> arg2) {
	_numOfArguments = numOfArg;
	leftArg = arg1;
	rightArg = arg2;
}

int Relationship::getNumOfArguments() {
	return _numOfArguments;
}

unordered_set<ParamType> Relationship::getLeftArg() {
	return leftArg;
}

unordered_set<ParamType> Relationship::getRightArg() {
	return rightArg;
}
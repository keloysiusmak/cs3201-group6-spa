#pragma once
#include <string>
#include <unordered_set>
#include "Param.h"

using namespace std;

class Relationship {
private:
	int _numOfArguments;
	unordered_set<ParamType> leftArg, rightArg;

public:
	Relationship();
	Relationship(int numOfArg, unordered_set<ParamType> arg1, unordered_set<ParamType> arg2);

	int getNumOfArguments();
	unordered_set<ParamType> getLeftArg();
	unordered_set<ParamType> getRightArg();
};
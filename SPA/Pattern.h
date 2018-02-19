#pragma once

#include <string>
#include "Param.h"

using namespace std;

class Pattern {
private:
	string _entityType;
	Param _leftParam;
	Param _rightParam;

public:
	//Constructors
	Pattern();

	//Methods
	void setPattern(string, Param, Param);

	string getEntityType();
	Param getLeftParam();
	Param getRightParam();
};
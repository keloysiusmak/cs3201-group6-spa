#pragma once

#include <string>
#include "Param.h"

using namespace std;

class Pattern {
private:
	Param _entity;
	Param _leftParam;
	Param _rightParam;

public:
	//Constructors
	Pattern();
	Pattern(Param, Param, Param);

	//Methods
	void setPattern(Param, Param, Param);

	Param getEntity();
	Param getLeftParam();
	Param getRightParam();
};
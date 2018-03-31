#pragma once

#include <string>
#include "Param.h"
#include "Clause.h"

using namespace std;

class Pattern : public Clause {
protected:
	Param _entity;

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
#pragma once

#include <string>
#include "Param.h"
#include "Clause.h"

using namespace std;

class Pattern : public Clause {
public:
	//Constructors
	Pattern();
	Pattern(Param, Param, Param, bool);

	//Methods
	void setPattern(Param, Param, Param, bool);

	Param getEntity();
	Param getLeftParam();
	Param getRightParam();
};
#pragma once

#include <string>
#include "Param.h"

using namespace std;

class Clause {
protected:
	RelRef _relRef;
	Param _entity;
	Param _leftParam;
	Param _rightParam;

public:
	//Constructors
	Clause();
	Clause(RelRef, Param, Param);

	//Methods
	void setClause(RelRef, Param, Param);

	RelRef getRelRef();
	Param getLeftParam();
	Param getRightParam();

	bool hasSynonymsNotALL();
};

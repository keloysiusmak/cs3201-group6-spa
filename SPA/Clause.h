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
	bool isInverted;

public:
	//Constructors
	Clause();
	Clause(RelRef, Param, Param, bool);

	//Methods
	void setClause(RelRef, Param, Param, bool);

	RelRef getRelRef();
	Param getLeftParam();
	Param getRightParam();
	bool getIsInverted();

	bool hasSynonymsNotALL();

	bool operator<(Clause other) const {
		return _relRef < other.getRelRef();
	}
};

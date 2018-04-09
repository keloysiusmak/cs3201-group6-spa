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
	void setRelRef(RelRef rel);
	void setLHS(Param lhs);
	void setRHS(Param rhs);
	void setInverted();

	RelRef getRelRef();
	Param getLeftParam();
	Param getRightParam();
	bool getIsInverted();

	bool hasSynonymsNotALL();

	bool operator<(Clause other) const {
		if (_relRef == other.getRelRef()) {
			if (_leftParam == other.getLeftParam()) {
				return _rightParam < other.getRightParam();
			} else {
				return _leftParam < other.getLeftParam();
			}
		} else {
			return _relRef < other.getRelRef();
		}
	}
};

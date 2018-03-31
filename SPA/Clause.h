#pragma once

#include <string>
#include "Param.h"

using namespace std;

enum RelRef { Modifies, Uses, Parent, ParentT, Follows, FollowsT,
				Next, NextT, Calls, CallsT, Affects, AffectsT, With, None };


class Clause {
protected:
	RelRef _relRef;
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
};

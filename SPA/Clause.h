#pragma once

#include <string>
#include "Param.h"

using namespace std;

enum RelRef { Modifies, Uses, Parent, ParentT, Follows, FollowsT,
				Next, NextT, Calls, CallsT, With };


class Clause {
private:
	RelRef _relRef;
	Param _firstParam;
	Param _secondParam;

public:
	//Constructors
	Clause();
	Clause(RelRef, Param, Param);

	//Methods
	void setClause(RelRef, Param, Param);

	RelRef getRelRef();
	Param getFirstParam();
	Param getSecondParam();
};

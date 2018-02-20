#pragma once

#include <string>
#include "Param.h"

using namespace std;

class Clause {
private:
	string _relRef;
	Param _firstParam;
	Param _secondParam;

public:
	//Constructors
	Clause();
	Clause(string, Param, Param);

	//Methods
	void setClause(string, Param, Param);

	string getRelRef();
	Param getFirstParam();
	Param getSecondParam();
};

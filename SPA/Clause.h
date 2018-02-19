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

	//Methods
	void setClause(string, Param, Param);

	string getRelRef();
	Param getFirstParam();
	Param getSecondParam();
};

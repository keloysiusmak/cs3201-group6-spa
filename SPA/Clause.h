#pragma once

#include <string>

using namespace std;

struct Param {
	string type;
	string name;
};

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

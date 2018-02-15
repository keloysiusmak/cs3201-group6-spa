#pragma once

#include <string>

using namespace std;

class Clause {
private:
	string _relRef;
	string _firstParam;
	string _secondParam;

public:
	//Constructors
	Clause();

	//Methods
	void setClause(string, string, string);

	string getRelRef();
	string getFirstParam();
	string getSecondParam();
};

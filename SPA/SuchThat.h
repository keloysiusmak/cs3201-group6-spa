#pragma once

#include <string>

using namespace std;

class SuchThat {
private:
	string _relRef;
	string _firstParam;
	string _secondParam;

public:
	//Constructors
	SuchThat();
	SuchThat(string, string, string);

	//Methods
	void setRelRef(string);
	void setFirstParam(string);
	void setSecondParam(string);

	string getRelRef();
	string getFirstParam();
	string getSecondParam();
};
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

	//Methods
	void setSuchThat(string, string, string);

	string getRelRef();
	string getFirstParam();
	string getSecondParam();
};
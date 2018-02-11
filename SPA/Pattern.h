#pragma once

#include <string>

using namespace std;

class Pattern {
private:
	string _entityType;
	string _leftParam;
	string _rightParam;

public:
	//Constructors
	Pattern();

	//Methods
	void setPattern(string, string, string);

	string getEntityType();
	string getLeftParam();
	string getRightParam();
};
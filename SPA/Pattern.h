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
	Pattern(string, string, string);

	//Methods
	void setEntityType(string);
	void setLeftParam(string);
	void setRightParam(string);

	string getEntityType();
	string getLeftParam();
	string getRightParam();
};
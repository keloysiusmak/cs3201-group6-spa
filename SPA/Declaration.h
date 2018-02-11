#pragma once

#include <string>

using namespace std;

class Declaration {
private:
	string _designEntity;
	string _synonym;

public:
	//Constructors
	Declaration();
	Declaration(string, string);

	//Methods
	void setDesignEntity(string);
	void setSynonym(string);

	string getDesignEntity();
	string getSynonym();
};
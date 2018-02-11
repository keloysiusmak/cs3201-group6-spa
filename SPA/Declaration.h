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

	//Methods
	void setDeclaration(string, string);

	string getDesignEntity();
	string getSynonym();
};
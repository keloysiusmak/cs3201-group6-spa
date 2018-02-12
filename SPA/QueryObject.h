#pragma once

#include <string>
#include <vector>
#include "Pattern.h"
#include "Declaration.h"
#include "SuchThat.h"

using namespace std;

class QueryObject {
private:
	vector<Declaration> _declarations;
	vector<SuchThat> _suchThats;
	vector<Pattern> _Patterns;
	string _selectStatement;

public:
	QueryObject();

	void insertDeclaration(string, string);
	void insertSuchThat(string, string, string);
	void insertPattern(string, string, string);
	void insertSelectStatement(string);

	vector<Declaration> getDeclarations();
	vector<SuchThat> getSuchThats();
	vector<Pattern> getPatterns();
	string getSelectStatement();
};
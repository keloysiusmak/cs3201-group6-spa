#pragma once

#include <string>
#include <vector>
#include "Pattern.h"
#include "Declaration.h"
#include "Clause.h"

using namespace std;

class QueryObject {
private:
	vector<Declaration> _declarations;
	vector<Clause> _clauses;
	vector<Pattern> _Patterns;
	string _selectStatement;

public:
	QueryObject();

	void insertDeclaration(string, string);
	void insertClause(string, string, string);
	void insertPattern(string, string, string);
	void insertSelectStatement(string);

	vector<Declaration> getDeclarations();
	vector<Clause> getClauses();
	vector<Pattern> getPatterns();
	string getSelectStatement();
};

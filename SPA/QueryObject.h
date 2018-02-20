#pragma once

#include <string>
#include <vector>
#include "Pattern.h"
#include "Declaration.h"
#include "Clause.h"

using namespace std;

class QueryObject {
private:
	vector<Clause> _clauses;
	vector<Pattern> _Patterns;
	Param _selectStmt;

public:
	QueryObject();
	QueryObject(string);

	void insertClause(string, string, string, string, string);
	void insertPattern(string, string, string, string, string, string);
	void insertSelectStmt(string, string);

	vector<Clause> getClauses();
	vector<Pattern> getPatterns();
	Param getSelectStatement();
};

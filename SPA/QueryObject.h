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

	void insertClause(RelRef, ParamType, string, ParamType, string);
	void insertPattern(ParamType, string, ParamType, string, ParamType, string);
	void insertSelectStmt(ParamType, string);

	vector<Clause> getClauses();
	vector<Pattern> getPatterns();
	Param getSelectStatement();
};

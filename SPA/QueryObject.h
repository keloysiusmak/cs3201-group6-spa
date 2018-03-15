#pragma once

#include <string>
#include <vector>
#include "Pattern.h"
#include "Clause.h"

using namespace std;

class QueryObject {
private:
	vector<Clause> _clauses;
	vector<Pattern> _Patterns;
	vector<Param> _selectStmts;

public:
	QueryObject();
	QueryObject(string);

	void insertClause(RelRef, ParamType, string, ParamType, string);
	void insertPattern(ParamType, string, ParamType, string, ParamType, string);
	void insertSelectStmtParam(ParamType, string);

	vector<Clause> getClauses();
	vector<Pattern> getPatterns();
	vector<Param> getSelectStatement();
};

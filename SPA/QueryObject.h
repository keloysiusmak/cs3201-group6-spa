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
	vector<Clause> _withClauses;
	vector<Param> _selectStmt;

public:
	QueryObject();
	QueryObject(string);

	void insertClause(RelRef, ParamType, string, ParamType, string);
	void insertPattern(ParamType, string, ParamType, string, ParamType, string);
	void insertSelectStmt(ParamType, string, AttrType);
	void insertWithClause(ParamType, string, AttrType, ParamType, string, AttrType);
	void setClause(vector<Clause>);
	void setWithClause(vector<Clause>);
	void setPattern(vector<Pattern>);

	vector<Clause> getClauses();
	vector<Pattern> getPatterns();
	vector<Clause> getWithClauses();
	vector<Param> getSelectStatements();

};

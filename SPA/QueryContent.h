#pragma once
#include "Constants.h";
#include <string>
#include <vector>
#include "Pattern.h"
#include "ClauseNode.h"

class QueryContent
{
private:
	vector<Param> _selectStmt;
	vector<ClauseNode> _clauses;
	vector<ClauseNode> _patterns;
	vector<ClauseNode> _withClauses;
public:
	void insertSelect(ParamType, string, AttrType);
	void insertClause(RelRef, ParamType, string, ParamType, string);
	void insertPattern(ParamType, string, ParamType, string, ParamType, string);
	void insertWithClause(ParamType, string, AttrType, ParamType, string, AttrType);
	void insertOperator(CLAUSE_NODE_TYPE, OPERATORS);
	vector<Param> getSelect();
	vector<ClauseNode> getClauses();
	vector<ClauseNode> getPattern();
	vector<ClauseNode> getWithClauses();
};


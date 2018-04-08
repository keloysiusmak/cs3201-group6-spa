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
	vector<QueryContent *> children;
public:
	void insertSelect(ParamType, string, AttrType);
	void insertClause(RelRef, ParamType, string, ParamType, string, bool);
	void insertPattern(ParamType, string, ParamType, string, ParamType, string, bool);
	void insertWithClause(ParamType, string, AttrType, ParamType, string, AttrType, bool);
	void insertOperator(CLAUSE_NODE_TYPE, OPERATORS);
	void setNegation(CLAUSE_NODE_TYPE, int);
	void setChildren(QueryContent *);
	void setClause(vector<ClauseNode>);
	void setWithClause(vector<ClauseNode>);
	void setPattern(vector<ClauseNode>);
	vector<Param> getSelect();
	vector<ClauseNode> getClauses();
	vector<ClauseNode> getPattern();
	vector<ClauseNode> getWithClauses();
	vector<QueryContent *> getChildren();
	
};


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
	void setSelect(vector<Param>);
	void insertClauses(ClauseNode);
	void insertPatterns(ClauseNode);
	void insertWithClauses(ClauseNode);
	vector<Param> getSelect();
	vector<ClauseNode> getClauses();
	vector<ClauseNode> getPattern();
	vector<ClauseNode> getWithClauses();
};


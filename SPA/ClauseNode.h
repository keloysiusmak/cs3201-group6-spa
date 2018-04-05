#pragma once
#include "Clause.h";
#include "Pattern.h";
#include "Constants.h";

class ClauseNode
{
private:
	CLAUSE_NODE_TYPE t;
	Clause c;
	Pattern p;
	Clause w;
	OPERATORS o;
	vector<ClauseNode> children;
public:
	ClauseNode();
	ClauseNode(Clause, CLAUSE_NODE_TYPE);
	ClauseNode(Pattern);
	ClauseNode(OPERATORS);
	void addChildren(ClauseNode);
	vector<ClauseNode> getChildren();
	CLAUSE_NODE_TYPE getClauseNodeType();
	Clause getClause();
	Clause getWithClause();
	Pattern getPattern();
	OPERATORS getOperators();
};


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
	OPERATORS o;
	vector<ClauseNode> children;
public:
	ClauseNode();
	ClauseNode(Clause);
	ClauseNode(Pattern);
	ClauseNode(OPERATORS);
	void addChildren(ClauseNode);
	vector<ClauseNode> getChildren();
};


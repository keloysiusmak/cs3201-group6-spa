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
	ClauseNode(OPERATORS);
	void setClauseNode(Clause);
	void setPatternNode(Pattern);
	void setWithClauseNode(Clause);
	void setOperatorNode(OPERATORS);
	void addChildren(ClauseNode);
	vector<ClauseNode> getChildren();
};


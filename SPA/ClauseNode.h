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
	ClauseNode &left;
	ClauseNode &right;
public:
	ClauseNode();
};


#pragma once

#include "../source/PKB.h"

class DesignExtractor
{
public:
	bool extract(PKB &);
	
private:
	void extractNext(PKB &);
	void processStatementList(PKB &, int, std::vector<int>, int);
	void extractNextStar(PKB &);
	void extractCallsInverse(PKB &);
	void extractCallsStar(PKB &);
	void extractUsesModifies(PKB &);
};


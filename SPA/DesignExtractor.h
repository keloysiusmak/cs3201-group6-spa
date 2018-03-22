#pragma once

#include "../source/PKB.h"

class DesignExtractor
{
public:
	bool extract(PKB &);
	
private:
	void extractNext(PKB &);
	void processStatementList(PKB &, int, int, int, int);
	void extractCallsInverse(PKB &);
	void extractCallsStar(PKB &);
	void extractCallStatements(PKB &);
	void extractUsesModifies(PKB &);
};


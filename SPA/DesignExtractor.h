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
	void countFollows(PKB &);
	void countFollowsStar(PKB &);
	void countUses(PKB &);
	void countModifies(PKB &);
	void countParent(PKB &);
	void countParentStar(PKB &);
	void countCalls(PKB &);
	void countCallsStar(PKB &);
	void countAffects(PKB &);
	void countAffectsStar(PKB &);
	void countNext(PKB &);
	void countNextStar(PKB &);
};


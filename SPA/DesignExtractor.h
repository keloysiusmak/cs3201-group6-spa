#pragma once

#include "../source/PKB.h"

class DesignExtractor
{
public:
	void extract(PKB &);
	
private:
	void extractNext(PKB &);
	void extractNextStar(PKB &);
	void extractCallsInverse(PKB &);
	void extractCallsStar(PKB &);
	void extractUsesModifies(PKB &);
};


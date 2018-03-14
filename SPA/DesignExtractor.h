#pragma once

#include "../source/PKB.h"

class DesignExtractor
{
public:
	void extract(PKB &);
	
private:
	void extractNext(PKB &);
	void extractNextStar(PKB &);
};


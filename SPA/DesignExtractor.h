#pragma once

#include "../source/PKB.h"

class DesignExtractor
{
public:
	PKB extract(PKB);
	
private:
	void extractCalls(PKB*);
};


#pragma once

#include "QueryObject.h"
#include "QueryEvaluator.h"
#include <string>
#include <vector>

class QueryQueuer
{

private:
	bool validQuery;
	QueryObject qo;
	QueryEvaluator _evaluator;
	list<string> invalidQueryMessage;

public:
	QueryQueuer();
	bool isValidQuery();
	void setQueryObject(QueryObject);
	QueryObject getQueryObject();
	void setEvaluator(QueryEvaluator &);
	void setInvalidQuery(string);
	list<string> evaluateQueries();
};


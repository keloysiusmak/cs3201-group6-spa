#pragma once

#include "QueryObject.h"
#include "QueryContent.h"
#include "QueryEvaluator.h"
#include <string>
#include <vector>

class QueryQueuer
{

private:
	bool validQuery;
	std::vector<QueryContent> qc;
	unordered_map<int, Param&> subQueryMapping;
	QueryEvaluator _evaluator;
	list<string> invalidQueryMessage;

public:
	QueryQueuer();
	bool isValidQuery();
	void setQueryContent(std::vector<QueryContent>);
	std::vector<QueryContent> getQueryContent();
	void setEvaluator(QueryEvaluator &);
	void setSubQueryMapping(unordered_map<int, Param&>);
	unordered_map<int, Param&> getSubQueryMapping();
	void setInvalidQuery(string);
	list<string> evaluateQueries();
	std::vector<QueryObject> parseQueryContent(QueryContent);
	std::vector<Clause> parseClauseTree(ClauseNode);
	std::vector<Clause> parseWithClauseTree(ClauseNode);
	std::vector<Pattern> parsePatternTree(ClauseNode);
};


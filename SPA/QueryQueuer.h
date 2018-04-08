#pragma once

#include "QueryObject.h"
#include "QueryContent.h"
#include "QueryEvaluator.h"
#include "Graph.h"
#include <string>
#include <vector>

class QueryQueuer
{

private:
	bool validQuery;
	std::vector<QueryContent> qc;
	QueryEvaluator _evaluator;
	list<string> invalidQueryMessage;
	unordered_map<QueryContent *, Param *> subQueryMapping;

public:
	QueryQueuer();
	bool isValidQuery();
	void setQueryContent(std::vector<QueryContent>);
	std::vector<QueryContent> getQueryContent();
	void setSubQueryMapping(unordered_map<QueryContent *, Param *>);
	unordered_map<QueryContent *, Param *> getSubQueryMapping();
	void setEvaluator(QueryEvaluator &);
	void setInvalidQuery(string);
	list<string> evaluateQueries();
	std::vector<QueryObject> parseQueryContent(QueryContent);
	std::vector<Clause> parseClauseTree(ClauseNode);
	std::vector<Clause> parseWithClauseTree(ClauseNode);
	std::vector<Pattern> parsePatternTree(ClauseNode);
	std::vector<QueryContent> QueryQueuer::sortQueryContent();
};


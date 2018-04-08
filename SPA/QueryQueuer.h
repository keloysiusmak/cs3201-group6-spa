#pragma once

#include "QueryObject.h"
#include "QueryContent.h"
#include "QueryEvaluator.h"
#include "Graph.h"
#include <string>
#include <vector>

enum CLAUSE_SELECTOR {
	REPLACE_CLAUSE,
	REPLACE_WITH_CLAUSE,
	REPLACE_PATTERN
};
enum CLAUSE_LEFT_OR_RIGHT {
	LEFT_PARAM,
	RIGHT_PARAM
};

class QueryQueuer
{

private:
	bool validQuery;
	std::vector<QueryContent> qc;
	QueryEvaluator _evaluator;
	list<string> invalidQueryMessage;
	unordered_map<int, vector<int>> subQueryMapping;

public:
	QueryQueuer();
	bool isValidQuery();
	void setQueryContent(std::vector<QueryContent>);
	std::vector<QueryContent> getQueryContent();
	void setSubQueryMapping(unordered_map<int, vector<int>>);
	unordered_map<int, vector<int>> getSubQueryMapping();
	void setEvaluator(QueryEvaluator &);
	void setInvalidQuery(string);
	list<string> evaluateQueries();
	std::vector<QueryObject> parseQueryContent(QueryContent);
	std::vector<Clause> parseClauseTree(ClauseNode);
	std::vector<Clause> parseWithClauseTree(ClauseNode);
	std::vector<Pattern> parsePatternTree(ClauseNode);
	std::vector<int> sortQueryContent();
	std::vector<QueryContent> convertSortedToQC(std::vector<int>);
	QueryContent replaceClauseNode(QueryContent, CLAUSE_SELECTOR, int, CLAUSE_LEFT_OR_RIGHT, string);
};



#pragma once

#include "QueryObject.h"
#include "Evaluator.h"
#include "QueryObject.h"
#include "Utils.h"
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <regex>

using namespace std;

class Preprocessor {
private:
	unordered_map<string, string> declarationMap;
	Evaluator * _evaluator;

public:
	Preprocessor();
	Preprocessor(Evaluator &evaluator);
	void setEvaluator(Evaluator &evaluator);
	void insertDeclarationToMap(string synonym, string declaration);
	unordered_map<string, string> getDeclarationMap();
	void preprocessQuery(string query);
	bool isValidDeclaration(string declaration);
	bool isValidQuery(string query);
	bool isValidSynonym(string synonym);
	bool isValidStmtRef(string stmtRef);
	bool isValidEntRef(string entRef);
	bool isValidExpressSpec(string expressSpec);
	bool isDeclarationSynonymExist(string synonym);
	bool parseClauseArg1(QueryObject &qo, string relType, string arg1, string arg2);
	bool parseClauseArg2(QueryObject &qo, string relType, string arg1, string arg2);
	bool parsePattern(QueryObject &qo, ParamType entityType, string entity, string arg1, string arg2);
	int retrieveArgType(string arg);
	string retrieveParamFromQuery(vector<string> queryArr, int &paramLength, int pos, string end);
	bool isValidSuchThatKeyword(string query);
};

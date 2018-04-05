#pragma once

#include "QueryObject.h"
#include "QueryObject.h"
#include "Utils.h"
#include "RelationshipTable.h"
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <regex>

using namespace std;

class Preprocessor {
private:
	unordered_map<string, string> declarationMap;
	RelationshipTable relTable;
	string errorMessage;
	QueryObject qo;

	int retrieveClauseArgType(string arg);
	int retrieveExpressionType(string expression);
	string retrieveParamFromQuery(vector<string> queryArr, int &paramLength, int pos, string end);
	string retrievePatternFromQuery(vector<string> queryArr, int &patternLength, int pos, string end);
	void replaceStarWithT(string& source);
	string infixToPostFix(string infix);
	int higherPrecedenceValidate(char operator1, char operator2);
	int getPrecedence(char op);
	bool checkBoolStmt(string query);
public:
	Preprocessor();
	void insertDeclarationToMap(string synonym, string declaration);
	unordered_map<string, string> getDeclarationMap();
	void preprocessQuery(string query);
	bool isValidDeclaration(string declaration);
	bool isValidQuery(string query);
	bool isValidSynonym(string synonym);
	bool isValidIdent(string ident);
	bool isValidStmtRef(string stmtRef);
	bool isValidEntRef(string entRef);
	bool isValidVarRef(string varRef);
	bool isValidAttrRef(string attrRef);
	bool isValidAttrName(ParamType synonymType, string attrName);
	bool isValidRef(string ref);
	bool isDeclarationSynonymExist(string synonym);
	bool parseClauseArg(QueryObject &qo, string relType, string arg1, string arg2);
	bool parsePattern(QueryObject &qo, ParamType entityType, string entity, string arg1, string arg2);
	bool parseWithClause(QueryObject &qo, string leftRef, string rightRef);
	bool isValidSuchThatKeyword(string query);
	string getErrorMessage();
	QueryObject getQueryObject();
};

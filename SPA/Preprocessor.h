#pragma once

#include "QueryContent.h"
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
	bool isErrorExist;
	std::vector<QueryContent> vqc;

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
	bool isValidElem(vector<string> queryArr, int endOfSelectStatement, QueryContent &qc);
	bool isValidClause(vector<string> queryArr, int &clauseLength, int pos, QueryContent &qc);
	bool isValidPattern(vector<string> queryArr, int &patternLength, int pos, QueryContent &qc);
	bool isValidWithClause(vector<string> queryArr, int &withLength, int pos, QueryContent &qc);
	bool isDeclarationSynonymExist(string synonym);
	bool parseClauseArg(QueryContent &qc, string relType, string arg1, string arg2);
	bool parsePattern(QueryContent &qc, ParamType entityType, string entity, string arg1, string arg2);
	bool parseWithClause(QueryContent &qc, string leftRef, string rightRef);
	bool isValidSuchThatKeyword(string query);
	string getErrorMessage();
	std::vector<QueryContent> getQueryContent();
	bool getIsErrorExist();
};

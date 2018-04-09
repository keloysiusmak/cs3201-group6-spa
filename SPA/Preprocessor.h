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
	vector<QueryContent> vqc;
	unordered_map<int, vector<int>> subQueryMapping;

	int retrieveClauseArgType(string arg);
	int retrieveExpressionType(string expression);
	string retrieveParamFromQuery(vector<string> queryArr, int &paramLength, int pos, string end);
	string retrievePatternFromQuery(vector<string> queryArr, int &patternLength, int pos, string end);
	void replaceStarWithT(string& source);
	string infixToPostFix(string infix);
	bool convertToPostFix(vector<string> queryArr, int conditionType, int &queryLength, int pos, QueryContent &qc);
	int higherPrecedenceValidate(char operator1, char operator2);
	int getPrecedence(char op);
	bool checkBoolStmt(string query);
	string mapParamTypeToValue(ParamType);
	string getArgValue(vector<string> queryArr, int &queryLength, int pos, 
		string relationshipKey, CLAUSE_NODE_TYPE nodeType, CLAUSE_LEFT_OR_RIGHT paramPos, 
		string endPoint, QueryContent &qc);
public:
	Preprocessor();
	void insertDeclarationToMap(string synonym, string declaration);
	void insertSubQueryMap(int queryIndex, CLAUSE_NODE_TYPE nodeType, int nodeIndex, CLAUSE_LEFT_OR_RIGHT paramPos);
	unordered_map<string, string> getDeclarationMap();
	void preprocessQuery(string query);
	bool isValidDeclaration(string declaration);
	bool isValidQuery(string query);
	bool isValidSubQuery(vector<string> queryArr, int pos, int &queryLength, QueryContent &qc);
	bool isValidSynonym(string synonym);
	bool isValidIdent(string ident);
	bool isValidStmtRef(string stmtRef);
	bool isValidEntRef(string entRef);
	bool isValidVarRef(string varRef);
	bool isValidAttrRef(string attrRef);
	bool isValidAttrName(ParamType synonymType, string attrName);
	bool isValidRef(string ref);
	bool isValidElem(vector<string> queryArr, int endOfSelectStatement, QueryContent &qc);
	bool isValidClause(vector<string> queryArr, int &clauseLength, int pos, QueryContent &qc, bool invert);
	bool isValidPattern(vector<string> queryArr, int &patternLength, int pos, QueryContent &qc, bool invert);
	bool isValidWithClause(vector<string> queryArr, int &withLength, int pos, QueryContent &qc, bool invert);
	bool isDeclarationSynonymExist(string synonym);
	bool parseClauseArg(QueryContent &qc, string relType, string arg1, string arg2, bool invert);
	bool parsePattern(QueryContent &qc, ParamType entityType, string entity, string arg1, string arg2, bool invert);
	bool parseWithClause(QueryContent &qc, string leftRef, string rightRef, bool invert);
	bool isValidSuchThatKeyword(string query);
	string getErrorMessage();
	vector<QueryContent> getQueryContent();
	unordered_map<int, vector<int>> getSubQueryMapping();
	bool getIsErrorExist();
};

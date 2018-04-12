#pragma once
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <set>
#include "Param.h"
#include "PKB.h"
#include "QueryContent.h"
#include "QueryObject.h"

using namespace std;

class Utils {
public:
	static Param createParam(ParamType type, string value, AttrType attr);
	static string sanitise(string str);
	static vector<string> explode(const string&, const string, const vector<char>);
	static bool isValidName(string);
	static vector<string> split(const string& s, char delimiter);
	static string trim(const string& str);
	static void find_and_replace(string& source, string const& find, string const& replace);
	static string getWordAndPop(queue<string>&);
	static bool isInteger(const string& s);
	static bool isSynonym(Param p);
	static bool isSameParam(Param a, Param b);
	static bool isValidFactor(string);
	static bool isValidOperator(string);
	static bool isValidConstant(string);
	static bool isOpenBracket(string);
	static bool isCloseBracket(string);
	static bool compareClauses(QueryObject, QueryObject);
	static bool compareClause(Clause, Clause);
	static bool comparePatterns(QueryObject, QueryObject);
	static bool comparePattern(Pattern, Pattern);
	static bool compareWithClauses(QueryObject, QueryObject);
	static bool compareWithClause(Clause, Clause);
	static bool compareParams(QueryObject, QueryObject);
	static bool compareParam(Param, Param);
	static bool compareQueryObjectProperties(QueryObject, QueryObject);
	static bool compareQueryContentProperties(QueryContent, QueryContent);
	static bool compareClauseNode(ClauseNode, ClauseNode);
	static void intersectSets(PKB &pkb, set<int> &s1, set<int> &s2, Param &p1, Param &p2, vector<vector<int>> &results);
	static bool hasStringValues(Param &p1);
	static set<string> convertStringValues(Param &p1, set<int> &oldSet, PKB &pkb);
	static int convertIntValues(Param &p1, string &oldItem, PKB &pkb);
};

namespace UtilsConstants {
	const char SANITISED_CHAR[] = { '\t', '\n' };
	const unordered_map<string, int> OPERATOR_PRIORITIES({ { "+", 2 },{ "-", 2 },{ "*", 3 } });
};



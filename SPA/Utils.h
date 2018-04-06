#pragma once
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <queue>
#include "Param.h"
#include "QueryContent.h"
#include "QueryObject.h"

using namespace std;

class Utils {
public:
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
};

namespace UtilsConstants {
	const char SANITISED_CHAR[] = { '\t', '\n' };
	const unordered_map<string, int> OPERATOR_PRIORITIES({ { "+", 2 },{ "-", 2 },{ "*", 3 } });
};



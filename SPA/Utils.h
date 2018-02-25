#pragma once
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <queue>

#include "Param.h"

using namespace std;

class Utils {
public:
	static string sanitise(string str);
	static vector<string> explode(const string&, const string, const vector<char>);
	static bool isValidName(string);
	static vector<string> split(const string& s, char delimiter);
	static string trim(const string& str);
	static string getWordAndPop(queue<string>&);
	static bool isInteger(const string& s);
	static bool isSynonym(ParamType type);
	static bool isValidFactor(string);
	static bool isValidOperator(string);
	static bool isValidConstant(string);
	static bool isOpenBracket(string);
	static bool isCloseBracket(string);
};

namespace UtilsConstants {
	const char SANITISED_CHAR[] = { '\t', '\n' };
	const unordered_map<std::string, int> OPERATOR_PRIORITIES({ { "+", 2 },{ "-", 2 },{ "*", 3 } });
};



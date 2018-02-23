#pragma once
#include <string>
#include <vector>
#include <regex>
#include <sstream>

#include "Param.h"

using namespace std;

class Utils {
public:
	static string sanitise(string str);
	static vector<string> explode(const string&, const string, const vector<char>);
	static bool isValidName(string);
	static vector<string> split(const string& s, char delimiter);
	static string trim(const string& str);
	static bool isInteger(const string& s);
	static bool isSynonym(ParamType type);
};

namespace UtilsConstants {
	const char SANITISED_CHAR[] = { '\t', '\n' };
};



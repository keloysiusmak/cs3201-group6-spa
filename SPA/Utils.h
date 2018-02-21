#pragma once
#include <string>
#include <vector>
#include <regex>

using namespace std;

class Utils {
public:
	static string sanitise(string str);
	static vector<string> explode(const string&, const string, const vector<char>);
	static bool isValidName(string);


};

namespace UtilsConstants {
	const char SANITISED_CHAR[] = { '\t', '\n' };
};



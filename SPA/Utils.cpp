#include "Utils.h"

using namespace std;

string Utils::sanitise(string str)
{
	for (size_t i = 0; i < strlen(UtilsConstants::SANITISED_CHAR); i++) {
		char sanitisedChar = UtilsConstants::SANITISED_CHAR[i];
		str.erase(remove(str.begin(), str.end(), sanitisedChar), str.end());
	}
	return str;
}

/* Split string into a vector of tokens, depending on delim_string and delimiters */
vector<string> Utils::explode(const string &str, const string delim_string, const vector<char> delimiters)
{
	vector<string> ele;
	int pos;
	int prev = 0;

	while ((pos = str.find_first_of(delim_string, prev)) != string::npos) {
		if (pos > prev) {
			ele.push_back(str.substr(prev, pos - prev));
		}
		if (find(begin(delimiters), end(delimiters), str[pos]) != end(delimiters)) {
			ele.push_back(string(1, str[pos]));
		}
		prev = pos + 1;
	}
	return ele;
}

bool Utils::isValidName(string name)
{
	if (!isalpha(name[0])) {
		return false;
	}
	else {
		for (size_t i = 1; i < name.size(); i++) {
			if (!isalnum(name[i])) {
				return false;
			}
		}
		return true;
	}
}

//Check if a string is a valid constant
bool Utils::isValidConstant(string number) {
	for (size_t i = 0; i < number.size(); i++) {
		if (!isdigit(number[i])) {
			return false;
		}
	}
	return true;
}

//Check if a string is a valid factor (constant or name)
bool Utils::isValidFactor(string factor) {
	return isValidConstant(factor) || isValidName(factor);
}

bool Utils::isValidOperator(string symbol) {
	return UtilsConstants::OPERATOR_PRIORITIES.find(symbol) != UtilsConstants::OPERATOR_PRIORITIES.end();
}

bool Utils::isOpenBracket(string symbol) {
	return symbol == "(";
}

bool Utils::isCloseBracket(string symbol) {
	return symbol == ")";
}

vector<string> Utils::split(const string& s, char delimiter) {
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

string Utils::trim(const string& str) {
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

void Utils::find_and_replace(string& source, string const& find, string const& replace) {
	for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;) {
		source.replace(i, find.length(), replace);
		i += replace.length();
	}
}

//checks that the queue is not empty, then gets the next element of the queue and pop the head
string Utils::getWordAndPop(queue<string> &originalExpression) {
	if (originalExpression.empty()) {
		throw exception("Invalid Expression!");
	}
	else {
		string word = originalExpression.front();
		originalExpression.pop();
		return word;
	}
}

bool Utils::isInteger(const string& s)
{
	return !s.empty() && find_if(s.begin(),
		s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

bool Utils::isSynonym(Param p) {
	ParamType type = p.type;
	switch (type) {
	case INTEGER:
		return false;
	case CONSTANT:
		return false;
	case EXPR:
		return false;
	case EXPR_EXACT:
		return false;
	case VAR_IDENT:
		return false;
	case PROC_IDENT:
		return false;
	default:
		return true;
	}
}

bool Utils::isSameParam(Param a, Param b) {
	return a.type == b.type && a.value == b.value;
}

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

bool Utils::compareClauses(QueryObject qo1, QueryObject qo2) {
	bool isSameClauses = qo1.getClauses().size() == qo2.getClauses().size();

	if (isSameClauses) {
		for (size_t i = 0; i < qo1.getClauses().size(); i++) {
			Clause c1 = qo1.getClauses().at(i);
			Clause c2 = qo2.getClauses().at(i);

			isSameClauses = Utils::compareClause(c1, c2);
			if (!isSameClauses) {
				break;
			}
		}
	}
	return isSameClauses;
}
bool Utils::compareClause(Clause c1, Clause c2) {
	if (c1.getRelRef() != c2.getRelRef() ||
		c1.getLeftParam().type != c2.getLeftParam().type ||
		c1.getLeftParam().value.compare(c2.getLeftParam().value) != 0 ||
		c1.getLeftParam().attribute != c2.getLeftParam().attribute ||
		c1.getRightParam().type != c2.getRightParam().type ||
		c1.getRightParam().value.compare(c2.getRightParam().value) != 0 ||
		c1.getRightParam().attribute != c2.getRightParam().attribute ||
		c1.getIsInverted() != c2.getIsInverted()) {
		return false;
	}
	return true;
}
bool Utils::comparePatterns(QueryObject qo1, QueryObject qo2) {
	bool isSamePatterns = qo1.getPatterns().size() == qo2.getPatterns().size();

	if (isSamePatterns) {
		for (size_t i = 0; i < qo1.getPatterns().size(); i++) {
			Pattern p1 = qo1.getPatterns().at(i);
			Pattern p2 = qo2.getPatterns().at(i);

			isSamePatterns = Utils::comparePattern(p1, p2);
			if (!isSamePatterns) {
				break;
			}
		}
	}
	return isSamePatterns;
}
bool Utils::comparePattern(Pattern p1, Pattern p2) {
	if (p1.getEntity().type != p2.getEntity().type ||
		p1.getEntity().value.compare(p2.getEntity().value) != 0 ||
		p1.getLeftParam().type != p2.getLeftParam().type ||
		p1.getLeftParam().value.compare(p2.getLeftParam().value) != 0 ||
		p1.getLeftParam().attribute != p2.getLeftParam().attribute ||
		p1.getRightParam().type != p2.getRightParam().type ||
		p1.getRightParam().value.compare(p2.getRightParam().value) != 0 ||
		p1.getRightParam().attribute != p2.getRightParam().attribute ||
		p1.getIsInverted() != p2.getIsInverted()) {
		return false;
	}
	return true;
}
bool Utils::compareWithClauses(QueryObject qo1, QueryObject qo2) {
	bool isSameWithClauses = qo1.getWithClauses().size() == qo2.getWithClauses().size();

	if (isSameWithClauses) {
		for (size_t i = 0; i < qo1.getWithClauses().size(); i++) {
			Clause w1 = qo1.getWithClauses().at(i);
			Clause w2 = qo2.getWithClauses().at(i);

			isSameWithClauses = Utils::compareWithClause(w1, w2);
			if (!isSameWithClauses) {
				return false;
			}
		}
	}
	return isSameWithClauses;
}
bool Utils::compareWithClause(Clause w1, Clause w2) {
	if (w1.getRelRef() != w2.getRelRef() ||
		w1.getLeftParam().type != w2.getLeftParam().type ||
		w1.getLeftParam().value.compare(w2.getLeftParam().value) != 0 ||
		w1.getLeftParam().attribute != w2.getLeftParam().attribute ||
		w1.getRightParam().type != w2.getRightParam().type ||
		w1.getRightParam().value.compare(w2.getRightParam().value) != 0 ||
		w1.getRightParam().attribute != w2.getRightParam().attribute ||
		w1.getIsInverted() != w2.getIsInverted()) {
		return false;
	}
	return true;
}
bool Utils::compareParams(QueryObject qo1, QueryObject qo2) {
	bool isSameSelectStatement = qo1.getSelectStatements().size() == qo2.getSelectStatements().size();

	if (isSameSelectStatement) {
		for (size_t i = 0; i < qo1.getSelectStatements().size(); i++) {
			Param s1 = qo1.getSelectStatements().at(i);
			Param s2 = qo1.getSelectStatements().at(i);

			isSameSelectStatement = Utils::compareParam(s1, s2);
			if (!isSameSelectStatement) {
				break;
			}
		}
	}
	return isSameSelectStatement;
}
bool Utils::compareParam(Param s1, Param s2) {
	if (s1.type != s2.type ||
		s1.value.compare(s2.value) != 0 ||
		s1.attribute != s2.attribute) {
		return false;
	}
	return true;
}
bool Utils::compareQueryObjectProperties(QueryObject qo1, QueryObject qo2) {

	bool isSameSelectStatement = Utils::compareParams(qo1, qo2);

	bool isSameClauses = Utils::compareClauses(qo1, qo2);

	bool isSamePatterns = Utils::comparePatterns(qo1, qo2);

	bool isSameWithClauses = Utils::compareWithClauses(qo1, qo2);

	return isSameSelectStatement && isSameClauses && isSamePatterns && isSameWithClauses;
}
bool Utils::compareQueryContentProperties(QueryContent qc1, QueryContent qc2) {
	if (qc1.getSelect().size() != qc2.getSelect().size()) return false;
	if (qc1.getClauses().size() != qc2.getClauses().size()) return false;
	if (qc1.getPattern().size() != qc2.getPattern().size()) return false;
	if (qc1.getWithClauses().size() != qc2.getWithClauses().size()) return false;
	vector<Param> pr1 = qc1.getSelect();
	vector<Param> pr2 = qc2.getSelect();
	for (int i = 0; i < pr1.size(); i++) {
		if (!Utils::compareParam(pr1[i], pr2[i])) return false;
	}
	vector<ClauseNode> c1 = qc1.getClauses();
	vector<ClauseNode> c2 = qc2.getClauses();
	for (int i = 0; i < c1.size(); i++) {
		if (!Utils::compareClauseNode(c1[i], c2[i])) return false;
	}
	vector<ClauseNode> p1 = qc1.getPattern();
	vector<ClauseNode> p2 = qc2.getPattern();
	for (int i = 0; i < p1.size(); i++) {
		if (!Utils::compareClauseNode(p1[i], p2[i])) return false;
	}
	vector<ClauseNode> w1 = qc1.getWithClauses();
	vector<ClauseNode> w2 = qc2.getWithClauses();
	for (int i = 0; i < w1.size(); i++) {
		if (!Utils::compareClauseNode(w1[i], w2[i])) return false;
	}

	return true;
}
bool Utils::compareClauseNode(ClauseNode cn1, ClauseNode cn2) {
	if (cn1.getChildren().size() != cn2.getChildren().size()) return false;
	for (int i = 0; i < cn1.getChildren().size(); i++) {
		if (!Utils::compareClauseNode(cn1.getChildren()[i], cn2.getChildren()[i])) return false;
	}
	if (cn1.getClauseNodeType() != cn2.getClauseNodeType()) return false;
	if (cn1.getClauseNodeType() == CLAUSE) {
		if (!Utils::compareClause(cn1.getClause(), cn2.getClause())) return false;
	} 
	else if (cn1.getClauseNodeType() == PATTERN) {
		if (!Utils::compareClause(cn1.getPattern(), cn2.getPattern())) return false;
	}
	else if (cn1.getClauseNodeType() == WITH_CLAUSE) {
		if (!Utils::compareClause(cn1.getWithClause(), cn2.getWithClause())) return false;
	}

	return true;
};

Param Utils::createParam(ParamType type, string value, AttrType attr) {
	Param param;
	param.type = type;
	param.value = value;
	param.attribute = attr;
	return param;
};

void Utils::intersectSets(set<int> &p1, set<int> &p2, vector<vector<int>> &results) {
	for (int value : p1) {
		vector<int> withTableRow;
		if (p2.find(value) != p2.end()) {
			withTableRow.push_back(value);
			withTableRow.push_back(value);
			results.push_back(withTableRow);
		}
	}
};
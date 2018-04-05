#include "Preprocessor.h"

//All the necessary symbols require to process the query
const char SYMBOL_SEMICOLON = ';';
const char SYMBOL_SPACE = ' ';
const char SYMBOL_COMMA = ',';
const char SYMBOL_DOUBLE_QUOTE = '\"';
const char SYMBOL_OPEN_BRACKET = '(';
const char SYMBOL_CLOSE_BRACKET = ')';
const char SYMBOL_EQUALS = '=';
const char SYMBOL_FULL_STOP = '.';
const char SYMBOL_UNDERSCORE = '_';
const char SYMBOL_ANGLE_OPEN_BRACKETS = '<';
const char SYMBOL_ANGLE_CLOSE_BRACKETS = '>';
const char SYMBOL_PLUS = '+';
const char SYMBOL_MINUS = '-';
const char SYMBOL_MULTIPLICATION = '*';
const char SYMBOL_DIVIDER = '|';

const string EMPTY_STRING = "";
const string DELIM_STRING = " ,()<>=+-*";
const vector<char> DELIMITERS_DECLARATION{};
const vector<char> DELIMITERS_QUERY{ SYMBOL_COMMA, SYMBOL_OPEN_BRACKET, SYMBOL_CLOSE_BRACKET, SYMBOL_ANGLE_OPEN_BRACKETS,
SYMBOL_ANGLE_CLOSE_BRACKETS, SYMBOL_EQUALS, SYMBOL_PLUS, SYMBOL_MINUS, SYMBOL_MULTIPLICATION };

const string SELECT_WORD = "Select";
const string SUCH_WORD = "such";
const string THAT_WORD = "that";
const string PATTERN_WORD = "pattern";
const string BOOLEAN_WORD = "BOOLEAN";
const string AND_WORD = "and";
const string WITH_WORD = "with";

const string SYNONYM_WORD = "synonym";
const string ALL_WORD = "all";
const string INTEGER_WORD = "integer";
const string IDENT_WORD = "ident";
const string CONSTANT_WORD = "constant";
const string VAR_NAME_WORD = "var_name";
const string INVALID_EXPRESSION = "Invalid Expression";
const string FALSE_WORD = "false";

const vector<string> CLAUSES_WITH_STAR = { "Calls*", "Parent*", "Follows*", "Next*", "Affects*" };
const vector<string> CLAUSES_WITH_T = { "CallsT", "ParentT", "FollowsT", "NextT", "AffectsT" };
const unordered_set<string> KEYWORDS_PATTERN_TYPE = { "assign", "while", "if" };
const unordered_map<ParamType, string> KEYWORDS_PATTERN_REL = { { ASSIGN, "assignpt" },
{ WHILE, "whilept" }, { IF, "ifpt" } };
const unordered_map<string, RelRef> KEYWORDS_CLAUSES = { { "Modifies", Modifies }, { "Uses", Uses }, 
{ "Parent", Parent },{ "ParentT", ParentT },{ "Follows", Follows },{ "FollowsT", FollowsT }, 
{ "Next", Next },{ "NextT", NextT },{ "Calls", Calls },{ "CallsT", CallsT }, { "Affects", Affects },
{ "AffectsT", AffectsT } };

const unordered_map<string, AttrType> KEYWORDS_WITH_TYPE = { { "procName", PROCNAME },
{ "varName", VARNAME },
{ "value", VALUE },
{ "stmt#", STMT_NO } };

const unordered_map<string, ParamType> KEYWORDS_DECLARATIONS = { { "assign", ASSIGN },{ "stmt", STMT },
{ "variable", VARIABLE },{ "while", WHILE },
{ "constant", CONSTANT },{ "prog_line", PROG_LINE },
{ "if", IF },{ "stmtLst", STMTLST },
{ "procedure", PROCEDURE },{ "call", CALL } };

const unordered_map<int, ParamType> NUMBER_MAPPING_CLAUSE_ARG_TYPE = { { 1, INTEGER }, { 2, IDENT }, 
{ 3, SYNONYM }, { 4, ALL } };

const unordered_map<int, ParamType> NUMBER_MAPPING_EXPRESSION_TYPE = { { 1, EXPR },
{ 2, EXPR_EXACT }, { 3, ALL } };

const unordered_map<string, ParamType> REL_MAPPING_LEFT_IDENT = { { "Modifies", PROC_IDENT },
{ "Uses", PROC_IDENT }, { "Calls", PROC_IDENT }, { "CallsT", PROC_IDENT }, { "assignpt", VAR_IDENT } };

const unordered_map<string, ParamType> REL_MAPPING_RIGHT_IDENT = { { "Modifies", VAR_IDENT },
{ "Uses", VAR_IDENT },{ "Calls", PROC_IDENT },{ "CallsT", PROC_IDENT } };

const unordered_map<bool, string> WITH_RELTABLE = { { true, "withString" }, { false, "withNumber" } };

const regex synonymRegex("(^[a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)");
const regex identRegex("(^(\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$))");
const regex stmtRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|([0-9]+$)))");
const regex entRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$)|([0-9]+$))");
const regex varRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$))");
const regex attrRefRegex("(^[a-zA-Z]([a-zA-Z]|[0-9]|[#])*[.](procName|varName|value|stmt#)$)");
const regex refRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([0-9]+$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$|[a-zA-Z]([a-zA-Z]|[0-9]|[#])*[.](procName|varName|value|stmt#)))");

Preprocessor::Preprocessor() {
	errorMessage = "";
	qo = QueryObject();
}

string Preprocessor::getErrorMessage() {
	return errorMessage;
}

QueryObject Preprocessor::getQueryObject() {
	return qo;
}

void Preprocessor::insertDeclarationToMap(string synonym, string declaration) {
	declarationMap.insert({ synonym, declaration });
}

unordered_map<string, string> Preprocessor::getDeclarationMap() {
	return declarationMap;
}

void Preprocessor::preprocessQuery(string query) {

	//Clear the contents in declarationMap first before processing the query
	declarationMap.clear();

	//sanitise whitespace from a String
	string q = Utils::sanitise(query);

	//Split the string into different parts: declarations & query
	vector<string> declarations = Utils::split(q, SYMBOL_SEMICOLON);

	//The position of the query will always be the last element of the vector
	int queryIndex = declarations.size() - 1;

	//if queryIndex is 0, means no declarations at all
	if (queryIndex <= 0) {

		if (!isValidQuery(q)) {
			//Check if select statement requires return BOOLEAN
			if (checkBoolStmt(q)) {
				errorMessage = FALSE_WORD;
			}
			else {
				errorMessage = EMPTY_STRING;
			}
		}
		return;
	}

	for (int i = 0; i < queryIndex; i++) {
		bool validateDeclaration = isValidDeclaration(Utils::sanitise(declarations.at(i)));

		if (!validateDeclaration) {
			//Check if select statement requires return BOOLEAN
			if (checkBoolStmt(q)) {
				errorMessage = FALSE_WORD;
			}
			else {
				errorMessage = EMPTY_STRING;
			}
			return;
		}
	}

	string queryPortion = Utils::sanitise(declarations.at(queryIndex));

	//validate whether is a valid query
	bool validQuery = isValidQuery(queryPortion);

	if (!validQuery) {
		//Check if select statement requires return BOOLEAN
		if (checkBoolStmt(q)) {
			errorMessage = FALSE_WORD;
		}
		else {
			errorMessage = EMPTY_STRING;
		}
	}
};

bool Preprocessor::isValidDeclaration(string declaration) {

	vector<string> declarationArr = Utils::explode(declaration + SYMBOL_SPACE, DELIM_STRING, DELIMITERS_DECLARATION);
	//vector<string> declarationArr = Utils::split(declaration, SYMBOL_SPACE);

	//Check if there is a declarationType and synonym
	if (declarationArr.size() < 2) {
		return false;
	}

	//Check if declarationType exists and the spelling of it
	bool checkDeclarationType = KEYWORDS_DECLARATIONS.find(declarationArr.at(0)) != KEYWORDS_DECLARATIONS.end();

	if (!checkDeclarationType) {
		return false;
	}

	for (int i = 1; i < declarationArr.size(); i++) {
		//skip comma
		if (declarationArr.at(i).at(0) == SYMBOL_COMMA) {
			continue;
		}
		string s = Utils::sanitise(declarationArr.at(i));

		if (!isValidSynonym(s) || isDeclarationSynonymExist(s)) {
			return false;
		}
		insertDeclarationToMap(s, declarationArr.at(0));
	}

	return true;
}

bool Preprocessor::isValidQuery(string query) {
	string q = Utils::sanitise(query);

	//Check for the single space between 'such that' keyword
	if (!isValidSuchThatKeyword(q)) {
		return false;
	}

	//Replace all the clauses with * to T
	replaceStarWithT(q);

	QueryObject queryObject;
	vector<string> queryArr = Utils::explode(q + SYMBOL_SPACE, DELIM_STRING, DELIMITERS_QUERY);

	//Check if Select word exists and if there's at least 2 elements in the query (e.g. "select", "s")
	if (queryArr.at(0).compare(SELECT_WORD) != 0 || queryArr.size() < 2) {
		return false;
	}

	int endOfSelectStatement = 1;

	//if is a tuple
	if (queryArr.at(endOfSelectStatement).at(0) == SYMBOL_ANGLE_OPEN_BRACKETS) {

		endOfSelectStatement++;

		//iterate through the tuple
		while (queryArr.at(endOfSelectStatement).at(0) != SYMBOL_ANGLE_CLOSE_BRACKETS) {

			//even position must be elem
			if (endOfSelectStatement % 2 == 0) {
				if (!isValidElem(queryArr, endOfSelectStatement, queryObject)) {
					return false;
				}
			}
			//odd position must be comma
			else {
				//check if is comma
				if (queryArr.at(endOfSelectStatement).at(0) != SYMBOL_COMMA) {
					return false;
				}
			}
			endOfSelectStatement++;
		}

		// if is a tuple without any values in (e.g. <>), return false
		if (endOfSelectStatement == 2) {
			return false;
		}
	}
	//if is a BOOLEAN
	else if (Utils::sanitise(queryArr.at(endOfSelectStatement)).compare(BOOLEAN_WORD) == 0) {
		queryObject.insertSelectStmt(BOOLEAN, BOOLEAN_WORD, NONE);
	}
	//single elem
	else {
		if (!isValidElem(queryArr, endOfSelectStatement, queryObject)) {
			return false;
		}
	}

	endOfSelectStatement++;

	//Check if there is any such that or pattern clause
	if (queryArr.size() == endOfSelectStatement) {
		qo = queryObject;
		return true;
	}

	//prevAndClause will keep track the condition "and" should correspond to which clauses
	//1 == such that
	//2 == pattern
	//3 == with
	//0 == there is nothing to correspond
	int prevAndClause = 0;

	for (int i = endOfSelectStatement; i < queryArr.size(); i++) {

		//check "such" word exists
		if (queryArr.at(i).compare(SUCH_WORD) == 0) {

			//Keep track of the length of clause
			int clauseLength = 1;

			// check "that" word exists
			if ((i + 1) >= queryArr.size() || queryArr.at(i + 1).compare(THAT_WORD) != 0) {
				return false;
			}

			//Add "that"
			clauseLength++;

			if (!isValidClause(queryArr, clauseLength, i, queryObject)) {
				return false;
			}

			//Finish processing this clause
			i += (clauseLength - 1);

			prevAndClause = 1;
		}
		//check whether "pattern" word exists
		else if (queryArr.at(i).compare(PATTERN_WORD) == 0) {

			//Keep track of the length of pattern
			int patternLength = 1;

			if (!isValidPattern(queryArr, patternLength, i, queryObject)) {
				return false;
			}

			//Finish processing this pattern
			i += (patternLength - 1);

			prevAndClause = 2;
		}
		//check whether "with" word exists
		else if (queryArr.at(i).compare(WITH_WORD) == 0) {

			//Keep track of the length of with clause
			int withLength = 1;
			
			if (!isValidWithClause(queryArr, withLength, i, queryObject)) {
				return false;
			}

			i += withLength;

			prevAndClause = 3;
		}
		else if (queryArr.at(i).compare(AND_WORD) == 0) {

			// check whether "and" have continuation
			if ((i + 1) >= queryArr.size()) {
				return false;
			}

			//such that
			if (prevAndClause == 1) {

				int clauseLength = 1;

				if (!isValidClause(queryArr, clauseLength, i, queryObject)) {
					return false;
				}
				//Finish processing this clause
				i += (clauseLength - 1);

			}
			//pattern
			else if (prevAndClause == 2) {
				//Keep track of the length of pattern
				int patternLength = 1;

				if (!isValidPattern(queryArr, patternLength, i, queryObject)) {
					return false;
				}

				//Finish processing this pattern
				i += (patternLength - 1);
			}
			//with
			else if (prevAndClause == 3) {

				//Keep track of the length of with clause
				int withLength = 1;

				if (!isValidWithClause(queryArr, withLength, i, queryObject)) {
					return false;
				}

				i += withLength;

			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	qo = queryObject;
	return true;
};

bool Preprocessor::isValidSynonym(string synonym) {
	if (synonym.length() == 0) {
		return false;
	}

	return regex_match(synonym, synonymRegex);
}

bool Preprocessor::isValidIdent(string ident) {
	if (ident.length() == 0) {
		return false;
	}

	return regex_match(ident, identRegex);
}

bool Preprocessor::isValidStmtRef(string stmtRef) {
	if (stmtRef.length() == 0) {
		return false;
	}

	return regex_match(stmtRef, stmtRefRegex);
}

bool Preprocessor::isValidEntRef(string entRef) {
	if (entRef.length() == 0) {
		return false;
	}

	return regex_match(entRef, entRefRegex);
}

bool Preprocessor::isValidVarRef(string varRef) {
	if (varRef.length() == 0) {
		return false;
	}

	return regex_match(varRef, varRefRegex);
}

bool Preprocessor::isValidAttrRef(string attrRef) {
	if (attrRef.length() == 0) {
		return false;
	}

	return regex_match(attrRef, attrRefRegex);
}

bool Preprocessor::isValidAttrName(ParamType synonymType, string attrName) {
	auto checkAttrRef = KEYWORDS_WITH_TYPE.find(attrName);

	switch (checkAttrRef->second)
	{
	case PROCNAME:
		return synonymType == PROCEDURE || synonymType == CALL;
	case VARNAME:
		return synonymType == VARIABLE;
	case VALUE:
		return synonymType == CONSTANT;
	case STMT_NO:
		return (synonymType != PROCEDURE) && (synonymType != VARIABLE) 
				&& (synonymType != CONSTANT) && (synonymType != PROG_LINE);
	default:
		return false;
	}
}

bool Preprocessor::isValidRef(string ref) {
	if (ref.length() == 0) {
		return false;
	}

	return regex_match(ref, refRegex);
}

bool Preprocessor::isDeclarationSynonymExist(string synonym) {
	//Check if there exist a synonym in the declarationMap
	auto checkSynonymExist = declarationMap.find(synonym);

	if (checkSynonymExist == declarationMap.end()) {
		return false;
	}
	return true;
}

bool Preprocessor::isValidElem(vector<string> queryArr, int endOfSelectStatement, QueryObject &qo) {
	string elem = Utils::sanitise(queryArr.at(endOfSelectStatement));

	if (isValidAttrRef(elem)) {
		vector<string> attrRef = Utils::split(elem, SYMBOL_FULL_STOP);

		//check if select synonym exist in the declarationMap
		if (!isDeclarationSynonymExist(attrRef.at(0))) {
			return false;
		}

		auto searchSynonym = declarationMap.find(attrRef.at(0));
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		if (!isValidAttrName(searchDeclareType->second, attrRef.at(1))) {
			return false;
		}

		//insert selectType of QueryObject
		AttrType getAttrName = KEYWORDS_WITH_TYPE.find(attrRef.at(1))->second;
		qo.insertSelectStmt(searchDeclareType->second, searchSynonym->first, getAttrName);
	}
	else if (isValidSynonym(elem)) {
		//check if select synonym exist in the declarationMap
		if (!isDeclarationSynonymExist(elem)) {
			return false;
		}

		//Populate the selectType of QueryObject
		auto searchSynonym = declarationMap.find(elem);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
		qo.insertSelectStmt(searchDeclareType->second, searchSynonym->first, NONE);
	}
	else {
		return false;
	}

	return true;
}

bool Preprocessor::isValidClause(vector<string> queryArr, int &clauseLength, int pos, QueryObject &qo) {
	//check whether clause exists
	if ((pos + clauseLength) >= queryArr.size() || KEYWORDS_CLAUSES.find(queryArr.at(pos + clauseLength)) == KEYWORDS_CLAUSES.end()) {
		return false;
	}

	string clauseValue = queryArr.at(pos + clauseLength);

	//Add relRef
	clauseLength++;

	//check whether open_bracket exists
	if ((pos + clauseLength) >= queryArr.size() || queryArr.at(pos + clauseLength).at(0) != SYMBOL_OPEN_BRACKET) {
		return false;
	}

	//Add Open Bracket
	clauseLength++;

	//Add all the left Param
	string leftArg = retrieveParamFromQuery(queryArr, clauseLength, pos, string(1, SYMBOL_COMMA));

	//Add all the right Param
	string rightArg = retrieveParamFromQuery(queryArr, clauseLength, pos, string(1, SYMBOL_CLOSE_BRACKET));

	if (!parseClauseArg(qo, clauseValue, leftArg, rightArg)) {
		return false;
	}
	return true;
}

bool Preprocessor::isValidPattern(vector<string> queryArr, int &patternLength, int pos, QueryObject &qo) {
	if ((pos + patternLength) >= queryArr.size() || !isDeclarationSynonymExist(queryArr.at(pos + patternLength))) {
		return false;
	}

	auto searchSynonym = declarationMap.find(queryArr.at(pos + patternLength));

	//check whether patternType is valid
	if (KEYWORDS_PATTERN_TYPE.find(searchSynonym->second) == KEYWORDS_PATTERN_TYPE.end()) {
		return false;
	}

	auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

	//Add patternType
	patternLength++;

	//check whether open_bracket exists
	if ((pos + patternLength) >= queryArr.size() || queryArr.at(pos + patternLength).at(0) != SYMBOL_OPEN_BRACKET) {
		return false;
	}

	//Add Open Bracket
	patternLength++;

	//Add all the left Param
	string leftArg = retrieveParamFromQuery(queryArr, patternLength, pos, string(1, SYMBOL_COMMA));

	//Add all the right Param if is assignpt
	string rightArg;
	if (searchDeclareType->second == ASSIGN) {
		rightArg = retrievePatternFromQuery(queryArr, patternLength, pos, string(1, SYMBOL_CLOSE_BRACKET));
	}
	else {
		rightArg = string(1, SYMBOL_UNDERSCORE);
		//keep track of the number of underscore
		int countUnderscore = 0;

		if (searchDeclareType->second == IF) {
			//iterate through the syntax

			while (queryArr.at(pos + patternLength).at(0) != SYMBOL_CLOSE_BRACKET) {
				//Odd position must be underscore
				if (patternLength % 2 != 0) {
					if (queryArr.at(pos + patternLength).at(0) == SYMBOL_UNDERSCORE) {
						countUnderscore++;
					}
					else {
						return false;
					}
				}
				else {
					//check if is comma
					if (queryArr.at(pos + patternLength).at(0) != SYMBOL_COMMA) {
						return false;
					}
				}

				patternLength++;
			}

			//ifpt should have 2 underscore (center and right param)
			if (countUnderscore != 2) {
				return false;
			}
		}
		else {

			while (queryArr.at(pos + patternLength).at(0) != SYMBOL_CLOSE_BRACKET) {
				//Odd position must be underscore
				if (patternLength % 2 != 0) {
					if (queryArr.at(pos + patternLength).at(0) == SYMBOL_UNDERSCORE) {
						countUnderscore++;
					}
					else {
						return false;
					}
				}
				else {
					//check if is comma
					if (queryArr.at(pos + patternLength).at(0) != SYMBOL_COMMA) {
						return false;
					}
				}
				patternLength++;
			}

			//whilept should have 1 underscore (right param)
			if (countUnderscore != 1) {
				return false;
			}
		}
		patternLength++;
	}

	if (!parsePattern(qo, searchDeclareType->second, searchSynonym->first, leftArg, rightArg)) {
		return false;
	}
	return true;
}

bool Preprocessor::isValidWithClause(vector<string> queryArr, int &withLength, int pos, QueryObject &qo) {
	

	if ((pos + withLength) >= queryArr.size() ||
		(pos + withLength + 1) >= queryArr.size() ||
		(pos + withLength + 2) >= queryArr.size()) {
		return false;
	}

	string ref1 = Utils::sanitise(queryArr.at(pos + withLength));
	char equalSign = Utils::sanitise(queryArr.at(pos + withLength + 1)).at(0);
	string ref2 = Utils::sanitise(queryArr.at(pos + withLength + 2));

	withLength += 2;

	if (!isValidRef(ref1) ||
		equalSign != SYMBOL_EQUALS ||
		!isValidRef(ref2)) {
		return false;
	}

	if (!parseWithClause(qo, ref1, ref2)) {
		return false;
	}
	return true;
}

bool Preprocessor::parseClauseArg(QueryObject &qo, string relType, string arg1, string arg2) {

	//Clear all the unwanted spaces on relType, left and right arguments
	string rel = Utils::sanitise(relType);
	string leftArg = Utils::sanitise(arg1);
	string rightArg = Utils::sanitise(arg2);
	
	if (!relTable.isValidRelationship(rel) ||
		leftArg.length() < 1 ||
		rightArg.length() < 1) {
		return false;
	}

	auto searchRelType = KEYWORDS_CLAUSES.find(relType);

	int leftArgMappingNum = retrieveClauseArgType(leftArg);
	int rightArgMappingNum = retrieveClauseArgType(rightArg);

	if (leftArgMappingNum == 0 || rightArgMappingNum == 0) {
		return false;
	}

	ParamType leftArgType = NUMBER_MAPPING_CLAUSE_ARG_TYPE.find(leftArgMappingNum)->second;
	ParamType rightArgType = NUMBER_MAPPING_CLAUSE_ARG_TYPE.find(rightArgMappingNum)->second;

	bool sameSynonymValue = false;
	bool sameIntegerValue = false;

	//Check if both synonym are the same value
	if (leftArgType == SYNONYM && rightArgType == SYNONYM) {
		sameSynonymValue = leftArg.compare(rightArg) == 0;
	} else if (leftArgType == INTEGER && rightArgType == INTEGER) {
		sameIntegerValue = leftArg.compare(rightArg) == 0;
	}

	switch (searchRelType->second)
	{
	case Uses: case Modifies:
		if (!isValidEntRef(leftArg) || !isValidVarRef(rightArg)) {
			return false;
		}
		break;
	case Calls: case CallsT:
		if (!isValidEntRef(leftArg) || !isValidEntRef(rightArg)) {
			return false;
		}
		break;
	case Parent: case ParentT: case Follows: case FollowsT: case Next:
		if (!isValidStmtRef(leftArg) || !isValidStmtRef(rightArg) || sameSynonymValue || sameIntegerValue) {
			return false;
		}
		break;
	case NextT: case Affects: case AffectsT:
		if (!isValidStmtRef(leftArg) || !isValidStmtRef(rightArg)) {
			return false;
		}
		break;
	default:
		return false;
	}

	//Check if synonym exists in declarations and convert the type to the corresponding type
	if (leftArgType == SYNONYM) {
		if (!isDeclarationSynonymExist(leftArg)) {
			return false;
		}

		auto searchSynonym = declarationMap.find(leftArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		leftArgType = searchDeclareType->second;
	}
	//invalid if is 0 and below
	else if (leftArgType == INTEGER) {
		if (stoi(leftArg) < 1) {
			return false;
		}
	}
	else if (leftArgType == IDENT) {
		leftArg = Utils::sanitise((Utils::split(leftArg, SYMBOL_DOUBLE_QUOTE)).at(1));
		leftArgType = REL_MAPPING_LEFT_IDENT.find(rel)->second;
	}

	//Check if synonym exists in declarations and convert the type to the corresponding type
	if (rightArgType == SYNONYM) {
		if (!isDeclarationSynonymExist(rightArg)) {
			return false;
		}

		auto searchSynonym = declarationMap.find(rightArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		rightArgType = searchDeclareType->second;
	}
	//invalid if is 0 and below
	else if (rightArgType == INTEGER) {
		if (stoi(rightArg) < 1) {
			return false;
		}
	}
	else if (rightArgType == IDENT) {
		rightArg = Utils::sanitise((Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE)).at(1));
		rightArgType = REL_MAPPING_RIGHT_IDENT.find(rel)->second;
	}

	if (!relTable.isValidArg(rel, leftArgType, rightArgType)) {
		return false;
	}

	qo.insertClause(searchRelType->second, leftArgType, leftArg,
		rightArgType, rightArg);

	return true;
}

bool Preprocessor::parsePattern(QueryObject &qo, ParamType entityType, string entity, string arg1, string arg2)
{

	string leftArg = Utils::sanitise(arg1);
	string rightArg = Utils::sanitise(arg2);

	if (leftArg.length() < 1 || !isValidVarRef(leftArg) || rightArg.length() < 1) {
		return false;
	}

	//only for assignpt RHS argument
	ParamType rightArgType = ALL;

	if (entityType == ASSIGN) {
		int rightArgMappingNum = retrieveExpressionType(rightArg);

		if (rightArgMappingNum == 0) {
			return false;
		}

		rightArgType = NUMBER_MAPPING_EXPRESSION_TYPE.find(rightArgMappingNum)->second;

		if (rightArgType == EXPR || rightArgType == EXPR_EXACT) {
			vector<string> exprArr = Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE);
			string expression = exprArr.at(1);

			rightArg = infixToPostFix(expression);

			if (rightArg.compare(INVALID_EXPRESSION) == 0) {
				return false;
			}
		}
	}

	auto relPt = KEYWORDS_PATTERN_REL.find(entityType);

	//check whether leftArg is synonym, _ or identity
	int leftArgMappingNum = retrieveClauseArgType(leftArg);

	if (leftArgMappingNum == 0) {
		return false;
	}

	ParamType leftArgType = NUMBER_MAPPING_CLAUSE_ARG_TYPE.find(leftArgMappingNum)->second;

	if (leftArgType == SYNONYM) {
		if (!isDeclarationSynonymExist(leftArg)) {
			return false;
		}

		auto searchSynonym = declarationMap.find(leftArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		leftArgType = searchDeclareType->second;
	}
	else if (leftArgType == IDENT) {
		leftArg = Utils::sanitise((Utils::split(leftArg, SYMBOL_DOUBLE_QUOTE)).at(1));
		leftArgType = REL_MAPPING_LEFT_IDENT.find(relPt->second)->second;
	}

	if (!relTable.isValidArg(relPt->second, leftArgType, rightArgType)) {
		return false;
	}

	qo.insertPattern(entityType, entity, leftArgType,
		leftArg, rightArgType, rightArg);

	return true;
}

bool Preprocessor::parseWithClause(QueryObject &qo, string leftRef, string rightRef) {
	
	string leftArg;
	string rightArg;

	ParamType leftArgType;
	ParamType rightArgType;

	AttrType leftAttrType = NONE;
	AttrType rightAttrType = NONE;

	bool isWithString = false;

	bool isLeftProcIdent = false;
	bool isRightProcIdent = false;

	if (isValidAttrRef(leftRef)) {
		vector<string> leftAttrRef = Utils::split(leftRef, SYMBOL_FULL_STOP);

		if (!isDeclarationSynonymExist(leftAttrRef.at(0))) {
			return false;
		}

		auto searchSynonym = declarationMap.find(leftAttrRef.at(0));
		leftArgType = KEYWORDS_DECLARATIONS.find(searchSynonym->second)->second;

		//Check whether the declaration can have the attribute
		if (!isValidAttrName(leftArgType, leftAttrRef.at(1))) {
			return false;
		}

		leftAttrType = KEYWORDS_WITH_TYPE.find(leftAttrRef.at(1))->second;
		leftArg = leftAttrRef.at(0);

		switch (leftAttrType) {
		case PROCNAME:
			isWithString = true;
			isRightProcIdent = true;
			break;
		case VARNAME:
			isWithString = true;
			break;
		}
	}
	// if is synonym, it must be prog_line
	else if (isValidSynonym(leftRef)) {

		if (!isDeclarationSynonymExist(leftRef)) {
			return false;
		}
		auto searchSynonym = declarationMap.find(leftRef);
		leftArgType = KEYWORDS_DECLARATIONS.find(searchSynonym->second)->second;

		if (leftArgType != PROG_LINE) {
			return false;
		}

		leftArg = leftRef;
	}
	else if (Utils::isInteger(leftRef)) {
		leftArgType = INTEGER;
		leftArg = leftRef;
	}
	else if (isValidIdent(leftRef)) {
		leftArgType = IDENT;
		leftArg = Utils::sanitise((Utils::split(leftRef, SYMBOL_DOUBLE_QUOTE)).at(1));

		isWithString = true;
	}
	else {
		return false;
	}

	if (isValidAttrRef(rightRef)) {
		vector<string> rightAttrRef = Utils::split(rightRef, SYMBOL_FULL_STOP);

		if (!isDeclarationSynonymExist(rightAttrRef.at(0))) {
			return false;
		}

		auto searchSynonym = declarationMap.find(rightAttrRef.at(0));
		rightArgType = KEYWORDS_DECLARATIONS.find(searchSynonym->second)->second;

		//Check whether the declaration can have the attribute
		if (!isValidAttrName(rightArgType, rightAttrRef.at(1))) {
			return false;
		}
		rightAttrType = KEYWORDS_WITH_TYPE.find(rightAttrRef.at(1))->second;
		rightArg = rightAttrRef.at(0);

		switch (rightAttrType) {
		case PROCNAME:
			isLeftProcIdent = true;
		}
	}
	// if is synonym, it must be prog_line
	else if (isValidSynonym(rightRef)) {

		if (!isDeclarationSynonymExist(rightRef)) {
			return false;
		}

		auto searchSynonym = declarationMap.find(rightRef);
		rightArgType = KEYWORDS_DECLARATIONS.find(searchSynonym->second)->second;

		if (rightArgType != PROG_LINE) {
			return false;
		}

		rightArg = rightRef;
	}
	else if (Utils::isInteger(rightRef)) {
		rightArgType = INTEGER;
		rightArg = rightRef;
	}
	else if (isValidIdent(rightRef)) {
		rightArgType = IDENT;
		rightArg = Utils::sanitise((Utils::split(rightRef, SYMBOL_DOUBLE_QUOTE)).at(1));
	}
	else {
		return false;
	}
	
	//Convert either one side to the respective IDENT
	if (leftArgType == IDENT) {
		if (isLeftProcIdent) {
			leftArgType = PROC_IDENT;
		}
		else {
			leftArgType = VAR_IDENT;
		}
	}

	if (rightArgType == IDENT) {
		if (isRightProcIdent) {
			rightArgType = PROC_IDENT;
		}
		else {
			rightArgType = VAR_IDENT;
		}
	}

	if (!relTable.isValidArg(WITH_RELTABLE.find(isWithString)->second, leftArgType, rightArgType)) {
		return false;
	}

	//Evaluate both arguments if both arguments are constant value like INTEGER and IDENT
	if ((leftArgType == INTEGER && rightArgType == INTEGER) ||
		(leftArgType == VAR_IDENT && rightArgType == VAR_IDENT)) {
		return leftArg.compare(rightArg) == 0;
	}
	else {
		qo.insertWithClause(leftArgType, leftArg, leftAttrType,
			rightArgType, rightArg, rightAttrType);
	}

	return true;
}

/*
1 == INTEGER
2 == IDENT
3 == SYNONYM
4 == ALL
0 == INVALID
*/
int Preprocessor::retrieveClauseArgType(string arg) {
	if (Utils::isInteger(arg)) {
		return 1;
	}
	else if (isValidIdent(arg)) {
		return 2;
	}
	else if (isValidSynonym(arg)) {
		return 3;
	}
	else if (arg.find(SYMBOL_UNDERSCORE) != string::npos) {
		return 4;
	}
	else {
		return 0;
	}
}

/*
1 == EXPR
2 == EXPR_EXACT
3 == ALL
0 == INVALID
*/
int Preprocessor::retrieveExpressionType(string expression) {
	//check for underscore & double quote at the front and end
	if (expression.size() > 4 &&
		expression.at(0) == SYMBOL_UNDERSCORE &&
		expression.at(1) == SYMBOL_DOUBLE_QUOTE &&
		expression.at(expression.size() - 2) == SYMBOL_DOUBLE_QUOTE &&
		expression.at(expression.size() - 1) == SYMBOL_UNDERSCORE) {
		return 1;
	}
	//check for double quote at the front and end 
	else if (expression.size() > 2 &&
		expression.at(0) == SYMBOL_DOUBLE_QUOTE &&
		expression.at(expression.size() - 1) == SYMBOL_DOUBLE_QUOTE) {
		return 2;
	}
	else if (expression.size() == 1 && expression.at(0) == SYMBOL_UNDERSCORE) {
		return 3;
	}
	else {
		return 0;
	}
}

string Preprocessor::retrieveParamFromQuery(vector<string> queryArr, int &paramLength, int pos, string end) {

	//Add all the Param
	string arg;

	while (true) {
		//if counter exceeded array size
		if ((pos + paramLength) >= queryArr.size()) {
			return EMPTY_STRING;
		}

		if (queryArr.at(pos + paramLength).compare(end) == 0) {
			//end of left param
			paramLength++;
			break;
		}
		else {
			arg += queryArr.at(pos + paramLength);
		}
		paramLength++;
	}

	return arg;
}

string Preprocessor::retrievePatternFromQuery(vector<string> queryArr, int &patternLength, int pos, string end) {
	//Add all the Param
	string arg;
	stack<string> s;
	while (true) {
		//if counter exceeded array size
		if ((pos + patternLength) >= queryArr.size()) {
			return EMPTY_STRING;
		}

		string temp = Utils::sanitise(queryArr.at(pos + patternLength));

		if (temp.compare(end) == 0 && s.empty()) {
			//end of left param
			patternLength++;
			break;
		}
		else {

			if (temp.at(0) == SYMBOL_OPEN_BRACKET) {
				s.push(temp);
			}
			else if (temp.at(0) == SYMBOL_CLOSE_BRACKET) {
				s.pop();
			}

			arg += temp;
		}
		patternLength++;
	}

	return arg;
}

bool Preprocessor::isValidSuchThatKeyword(string query) {

	vector<size_t> positions;

	size_t pos = query.find(SUCH_WORD, 0);

	while (pos != string::npos) {
		positions.push_back(pos);
		pos = query.find(SUCH_WORD, pos + 1);
	}

	for (size_t i = 0; i < positions.size(); i++) {
		size_t spacePos = positions.at(i) + 4;
		size_t thatPos = positions.at(i) + 5;
		if (spacePos >= query.length() ||
			thatPos >= query.length() ||
			query.at(spacePos) != SYMBOL_SPACE ||
			query.substr(thatPos, 4).compare(THAT_WORD) != 0) {
			return false;
		}

	}

	return true;
}

void Preprocessor::replaceStarWithT(string& source) {
	for (size_t i = 0; i < CLAUSES_WITH_STAR.size(); i++) {
		Utils::find_and_replace(source, CLAUSES_WITH_STAR.at(i), CLAUSES_WITH_T.at(i));
	}
}

string Preprocessor::infixToPostFix(string infix) {

	vector<string> infixArr = Utils::explode(infix + SYMBOL_SPACE, DELIM_STRING, DELIMITERS_QUERY);

	stack<char> operators;
	bool isMathOperatorRepeated = false;
	bool isOperandRepeated = false;
	bool isOperatorAtTheStart = false;
	bool isOperatorAtTheEnd = false;
	bool isOperandBesideStartBracket = false;
	bool isCloseBracketBesideOperand = false;
	string postfix;

	for (size_t i = 0; i < infixArr.size(); i++) {
		//Checking operator
		if (infixArr.at(i).at(0) == SYMBOL_PLUS ||
			infixArr.at(i).at(0) == SYMBOL_MINUS ||
			infixArr.at(i).at(0) == SYMBOL_MULTIPLICATION) {

			//At the start, it cannot be an operator
			if (i == 0) {
				isOperatorAtTheStart = true;
			}

			if (i == (infixArr.size() - 1)) {
				isOperatorAtTheEnd = true;
			}

			if (isMathOperatorRepeated || isOperatorAtTheStart || isOperatorAtTheEnd) {
				postfix = INVALID_EXPRESSION;

				/*
				After this for loop there is while loop
				which is checking rest of the char and add it with postfix string.
				So this pushed char should be pop out
				because infix espression is wrong.
				*/
				while (!operators.empty()) {
					operators.pop();
				}
				break;
			}

			while (!operators.empty() && higherPrecedenceValidate(operators.top(), infixArr.at(i).at(0))) {
				postfix += operators.top();
				postfix += SYMBOL_DIVIDER;
				operators.pop();
			}

			operators.push(infixArr.at(i).at(0));
			isMathOperatorRepeated = true;
			isOperandRepeated = false;
			isOperatorAtTheStart = false;
			isOperatorAtTheEnd = true;
			isOperandBesideStartBracket = false;
			isCloseBracketBesideOperand = false;
		}
		//checking operand
		else if (Utils::isInteger(infixArr.at(i)) || isValidSynonym(infixArr.at(i))) {
			if (isOperandRepeated || isCloseBracketBesideOperand) {
				postfix = INVALID_EXPRESSION;

				/*
				After this for loop there is while loop
				which is checking rest of the char and add it with postfix string.
				So this pushed char should be pop out
				because infix espression is wrong.
				*/
				while (!operators.empty()) {
					operators.pop();
				}
				break;
			}
			postfix += infixArr.at(i);
			postfix += SYMBOL_DIVIDER;
			isMathOperatorRepeated = false;
			isOperandRepeated = true;
			isOperatorAtTheStart = false;
			isOperatorAtTheEnd = false;
			isOperandBesideStartBracket = true;
			isCloseBracketBesideOperand = false;
		}
		//Checking open bracket
		else if (infixArr.at(i).at(0) == SYMBOL_OPEN_BRACKET) {

			if (isOperandBesideStartBracket) {
				postfix = INVALID_EXPRESSION;

				/*
				After this for loop there is while loop
				which is checking rest of the char and add it with postfix string.
				So this pushed char should be pop out
				because infix espression is wrong.
				*/
				while (!operators.empty()) {
					operators.pop();
				}
				break;
			}

			operators.push(infixArr.at(i).at(0));
			isMathOperatorRepeated = false;
			isOperandRepeated = false;
			isOperatorAtTheEnd = false;
			isOperatorAtTheStart = true;
			isOperandBesideStartBracket = false;
			isCloseBracketBesideOperand = false;
		}
		//Checking closing bracket
		else if (infixArr.at(i).at(0) == SYMBOL_CLOSE_BRACKET) {
			while (!operators.empty() && operators.top() != SYMBOL_OPEN_BRACKET) {
				postfix += operators.top();
				postfix += SYMBOL_DIVIDER;
				operators.pop();
			}

			/*
			checking stack because we know
			that if the infix char is ')'
			and the stack is empty then the infix expression is wrong
			*/
			if (operators.empty()) {
				postfix = INVALID_EXPRESSION;
				break;
			}
			else {
				operators.pop();
			}

			//Check if operator is at the end before close bracket
			if (isOperatorAtTheEnd) {
				postfix = INVALID_EXPRESSION;

				/*
				After this for loop there is while loop
				which is checking rest of the char and add it with postfix string.
				So this pushed char should be pop out
				because infix espression is wrong.
				*/
				while (!operators.empty()) {
					operators.pop();
				}
				break;
			}

			//popping the opening bracket
			isMathOperatorRepeated = false;
			isOperandRepeated = false;
			isOperatorAtTheStart = false;
			isOperatorAtTheEnd = false;
			isOperandBesideStartBracket = false;
			isCloseBracketBesideOperand = true;
		}

		//checking that infix expression has invalid char
		else {
			postfix = INVALID_EXPRESSION;

			/*
			After this for loop there is while loop
			which is checking rest of the char and add it with postfix string.
			So this pushed char should be pop out
			because infix espression is wrong.
			*/
			while (!operators.empty()) {
				operators.pop();
			}
			break;
		}
	}

	// popping the rest of element from the stack..
	while (!operators.empty()) {
		if (operators.top() == SYMBOL_OPEN_BRACKET) {
			postfix = INVALID_EXPRESSION;
			break;
		}
		else {
			postfix += operators.top();
			postfix += SYMBOL_DIVIDER;
			operators.pop();
		}
	}
	return postfix;
}

int Preprocessor::higherPrecedenceValidate(char operator1, char operator2) {
	int op1 = getPrecedence(operator1);
	int op2 = getPrecedence(operator2);

	if (op1 == op2) {
		return true;
	}

	return op1 > op2 ? true : false;
}

int Preprocessor::getPrecedence(char op) {
	int weight = 0;
	switch (op) {
	case SYMBOL_PLUS:
	case SYMBOL_MINUS:
		weight = 1;
		break;
	case SYMBOL_MULTIPLICATION:
		weight = 2;
		break;
	}
	return weight;
}

bool Preprocessor::checkBoolStmt(string query) {
	vector<string> queryArr = Utils::explode(query + SYMBOL_SPACE, DELIM_STRING, DELIMITERS_QUERY);

	for (size_t i = 0; i < queryArr.size(); i++) {
		if (queryArr.at(i) == SELECT_WORD) {
			if (i + 1 < queryArr.size() && queryArr.at(i + 1).compare(BOOLEAN_WORD) == 0) {
				return true;
			}
		}
	}
	return false;
}

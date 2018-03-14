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

const string EMPTY_STRING = "";
const string DELIM_STRING = " ,()<>";
const vector<char> DELIMITERS_DECLARATION { };
const vector<char> DELIMITERS_QUERY { SYMBOL_COMMA, SYMBOL_CLOSE_BRACKET, SYMBOL_ANGLE_OPEN_BRACKETS,
										SYMBOL_ANGLE_CLOSE_BRACKETS };

const string SELECT_WORD = "Select";
const string SUCH_WORD = "such";
const string THAT_WORD = "that";
const string PATTERN_WORD = "pattern";
const string BOOLEAN_WORD = "BOOLEAN";

const string SYNONYM_WORD = "synonym";
const string ALL_WORD = "all";
const string INTEGER_WORD = "integer";
const string IDENT_WORD = "ident";
const string CONSTANT_WORD = "constant";
const string VAR_NAME_WORD = "var_name";

const unordered_set<string> KEYWORDS_PATTERN_TYPE = { "assign" };
const unordered_map<string, RelRef> KEYWORDS_CLAUSES_1 = { { "Modifies", Modifies },{ "Uses", Uses } };
const unordered_map<string, RelRef> KEYWORDS_CLAUSES_2 = { { "Parent", Parent }, { "Parent*", ParentT },
															{ "Follows", Follows }, { "Follows*", FollowsT },
															{ "Next", Next }, { "NextT", NextT } };
const unordered_map<string, RelRef> KEYWORDS_CLAUSES_3 = { { "Calls", Calls }, { "Calls*", CallsT } };

const unordered_map<string, ParamType> KEYWORDS_DECLARATIONS = { { "assign", ASSIGN }, { "stmt", STMT },
																{ "variable", VARIABLE }, { "while", WHILE },
																{ "constant", CONSTANT }, { "prog_line", PROG_LINE },
																{ "if", IF }, { "stmtLst", STMTLST },
																{ "procedure", PROCEDURE }, { "call", CALL } };

const unordered_map<int, ParamType> NUMBER_MAPPING_REF_TYPE = { { 1, INTEGER }, { 2, CONSTANT },
															{ 3, EXPR }, { 4, IDENT }, { 5, SYNONYM },
															{ 6, ALL } };

const regex synonymRegex("(^[a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)");
const regex stmtRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|([0-9]+$)))");
const regex entRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$)|([0-9]+$))");
const regex varRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$))");
const regex expressSpecRegex("(^((_\"(([a-zA-Z]([a-zA-Z]|[0-9])*)|([0-9]+))\"_$)|[_]$))");

Preprocessor::Preprocessor() {

}

//Preprocessor::Preprocessor(Evaluator &evaluator) {
//	_evaluator = &evaluator;
//}

//void Preprocessor::setEvaluator(Evaluator &evaluator) {
//	_evaluator = &evaluator;
//}

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
		// insert evaluator invalid query api here
		//(*_evaluator).setInvalidQuery("Invalid Query");
	}

	for (int i = 0; i < queryIndex; i++) {
		bool validateDeclaration = isValidDeclaration(Utils::sanitise(declarations.at(i)));

		if (!validateDeclaration) {
			// insert evaluator invalid query api here
			//(*_evaluator).setInvalidQuery("Invalid Query");
			return;
		}
	}

	string queryPortion = Utils::sanitise(declarations.at(queryIndex));

	//validate whether is a valid query
	bool validQuery = isValidQuery(queryPortion);
	
	if (!validQuery) {
		// insert evaluator invalid query api here
		//(*_evaluator).setInvalidQuery("Invalid Query");
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
		//remove comma
		vector<string> synonym = Utils::split(declarationArr.at(i), SYMBOL_COMMA);
		string s = Utils::sanitise(synonym.at(0));

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

			//even position must be synonym
			if (endOfSelectStatement % 2 == 0) {

				//check if select synonym exist in the declarationMap
				if (!isDeclarationSynonymExist(queryArr.at(endOfSelectStatement))) {
					return false;
				}

				//Populate the selectType of QueryObject
				auto searchSynonym = declarationMap.find(queryArr.at(endOfSelectStatement));
				auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
				queryObject.insertSelectStmt(searchDeclareType->second, searchSynonym->first);
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
	else if (queryArr.at(endOfSelectStatement).compare(BOOLEAN_WORD) == 0) {
		queryObject.insertSelectStmt(BOOLEAN, BOOLEAN_WORD);
	}
	else {
		//check if select synonym exist in the declarationMap
		if (!isDeclarationSynonymExist(queryArr.at(endOfSelectStatement))) {
			return false;
		}

		//Populate the selectType of QueryObject
		auto searchSynonym = declarationMap.find(queryArr.at(endOfSelectStatement));
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
		queryObject.insertSelectStmt(searchDeclareType->second, searchSynonym->first);
	}

	endOfSelectStatement++;

	//Check if there is any such that or pattern clause
	if (queryArr.size() == endOfSelectStatement) {
		// insert evaluator query api here
		//(*_evaluator).setQueryObject(queryObject);
		return true;
	}
	
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

			//check whether clause exists
			if ((i + 2) >= queryArr.size()) {
				return false;
			}

			//Add relRef
			clauseLength++;

			//Add all the left Param
			string leftArg = retrieveParamFromQuery(queryArr, clauseLength, i, string(1, SYMBOL_COMMA));

			//Add all the right Param
			string rightArg = retrieveParamFromQuery(queryArr, clauseLength, i, string(1, SYMBOL_CLOSE_BRACKET));

			//Check whether is a valid clause
			if (KEYWORDS_CLAUSES_1.find(queryArr.at(i + 2)) != KEYWORDS_CLAUSES_1.end()) {
				if (!parseClauseArg1(queryObject, queryArr.at(i + 2), leftArg, rightArg)) {
					return false;
				}
			}
			else if (KEYWORDS_CLAUSES_2.find(queryArr.at(i + 2)) != KEYWORDS_CLAUSES_2.end()) {
				if (!parseClauseArg2(queryObject, queryArr.at(i + 2), leftArg, rightArg)) {
					return false;
				}
			}
			else if (KEYWORDS_CLAUSES_3.find(queryArr.at(i + 2)) != KEYWORDS_CLAUSES_3.end()) {
				if (!parseClauseArg3(queryObject, queryArr.at(i + 2), leftArg, rightArg)) {
					return false;
				}
			}
			else {
				return false;
			}

			//Finish processing this clause
			i += (clauseLength - 1);
		}
		//check wether "pattern" word exists
		else if (queryArr.at(i).compare(PATTERN_WORD) == 0) {

			//Keep track of the length of pattern
			int patternLength = 1;

			//check whether pattern type exists and
			//the patternType should exist in the declarationMap
			if ((i + 1) >= queryArr.size() || !isDeclarationSynonymExist(queryArr.at(i + 1))) {
				return false;
			}

			auto searchSynonym = declarationMap.find(queryArr.at(i + 1));

			//check whether patternType is valid
			if (KEYWORDS_PATTERN_TYPE.find(searchSynonym->second) == KEYWORDS_PATTERN_TYPE.end()) {
				return false;
			}

			//Add patternType
			patternLength++;

			//Add all the left Param
			string leftArg = retrieveParamFromQuery(queryArr, patternLength, i, string(1, SYMBOL_COMMA));

			//Add all the right Param
			string rightArg = retrieveParamFromQuery(queryArr, patternLength, i, string(1, SYMBOL_CLOSE_BRACKET));

			auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

			if (!parsePattern(queryObject, searchDeclareType->second, queryArr.at(i + 1), leftArg, rightArg)) {
				return false;
			}

			//Finish processing this pattern
			i += (patternLength - 1);
		}
		else {
			return false;
		}
	}

	// insert evaluator query api here
	//(*_evaluator).setQueryObject(queryObject);
	return true;
};

bool Preprocessor::isValidSynonym(string synonym) {
	if (synonym.length() == 0) {
		return false;
	}

	return regex_match(synonym, synonymRegex);
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

bool Preprocessor::isValidExpressSpec(string expressSpec) {
	if (expressSpec.length() == 0) {
		return false;
	}

	return regex_match(expressSpec, expressSpecRegex);
}

bool Preprocessor::isDeclarationSynonymExist(string synonym) {
	//Check if there exist a synonym in the declarationMap
	auto checkSynonymExist = declarationMap.find(synonym);

	if (checkSynonymExist == declarationMap.end()) {
		return false;
	}
	return true;
}

/*UsesS, UsesP, ModifiesS, ModifiesP*/
bool Preprocessor::parseClauseArg1(QueryObject &qo, string relType, string arg1, string arg2) {

	//Clear all the unwanted spaces on left and right arguments
	string leftArg = Utils::sanitise(arg1);
	string rightArg = Utils::sanitise(arg2);

	if (leftArg.length() < 1 || !isValidEntRef(leftArg)) {
		return false;
	}

	if (rightArg.length() < 1 || !isValidVarRef(rightArg)) {
		return false;
	}

	auto leftArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(leftArg));
	ParamType insertLeftType = leftArgType->second;

	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertLeftType == SYNONYM) {
		if (!isDeclarationSynonymExist(leftArg)) {
			return false;
		}
		
		//Check if is a variable, constant declaration
		auto searchSynonym = declarationMap.find(leftArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		// if is a variable, constant declaration for the left Param, return false
		if (searchDeclareType->second == VARIABLE ||
			searchDeclareType->second == CONSTANT) {
			return false;
		}

		//Change the synonym to the declaration type with reference to the declarationMap
		insertLeftType = searchDeclareType->second;
	}
	//Check if is Integer
	else if (insertLeftType == INTEGER) {
		//invalid if is 0 and below
		if (stoi(leftArg) < 1) {
			return false;
		}
	}
	//Check if is Ident and store the content between the double quotes
	else if (insertLeftType == IDENT) {
		leftArg = (Utils::split(leftArg, SYMBOL_DOUBLE_QUOTE)).at(1);
	}
	// Underscore
	// Uses and Modifies left Param must not be underscore
	else {
		return false;
	}

	auto rightArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(rightArg));
	ParamType insertRightType = rightArgType->second;

	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertRightType == SYNONYM) {
		if (!isDeclarationSynonymExist(rightArg)) {
			return false;
		}

		//Change the synonym to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(rightArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		// if is not a variable declaration for the right Param, return false
		if (searchDeclareType->second != VARIABLE) {
			return false;
		}

		insertRightType = searchDeclareType->second;
	}
	//Check if is Ident and store the content between the double quotes
	else if (insertRightType == IDENT) {
		rightArg = (Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE)).at(1);
	}


	auto searchRelType = KEYWORDS_CLAUSES_1.find(relType);
	qo.insertClause(searchRelType->second, insertLeftType, leftArg,
		insertRightType, rightArg);

	return true;
}

/*Follows, FollowsT, Parent, ParentT, Next, NextT*/
bool Preprocessor::parseClauseArg2(QueryObject &qo, string relType, string arg1, string arg2) {

	string leftArg = Utils::sanitise(arg1);
	string rightArg = Utils::sanitise(arg2);

	if (leftArg.length() < 1 || !isValidStmtRef(leftArg)) {
		return false;
	}

	if (rightArg.length() < 1 || !isValidStmtRef(rightArg)) {
		return false;
	}

	auto leftArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(leftArg));
	ParamType insertLeftType = leftArgType->second;
	bool isLeftSynonym = false;
	bool isLeftInteger = false;
	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertLeftType == SYNONYM) {
		if (!isDeclarationSynonymExist(leftArg)) {
			return false;
		}

		//Change the synonym to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(leftArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		// if is a variable, constant declaration for the left Param, return false
		if (searchDeclareType->second == VARIABLE ||
			searchDeclareType->second == CONSTANT) {
			return false;
		}

		insertLeftType = searchDeclareType->second;
		isLeftSynonym = true;
	}
	//Check if is Integer
	else if (insertLeftType == INTEGER) {
		//invalid if is 0 and below
		if (stoi(leftArg) < 1) {
			return false;
		}
		isLeftInteger = true;
	}

	auto rightArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(rightArg));
	ParamType insertRightType = rightArgType->second;
	bool isRightSynonym = false;
	bool isRightInteger = false;

	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertRightType == SYNONYM) {
		if (!isDeclarationSynonymExist(rightArg)) {
			return false;
		}

		//Change the synonym to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(rightArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		// if is a variable, constant declaration for the left Param, return false
		if (searchDeclareType->second == VARIABLE ||
			searchDeclareType->second == CONSTANT) {
			return false;
		}

		insertRightType = searchDeclareType->second;
		isRightSynonym = true;
	}
	//Check if is Integer
	else if (insertRightType == INTEGER) {
		//invalid if is 0 and below
		if (stoi(rightArg) < 1) {
			return false;
		}
		isRightInteger = true;
	}

	auto searchRelType = KEYWORDS_CLAUSES_2.find(relType);

	//Check for same synonym or statement number on the left and right param
	//only for Follows/FollowsT, Parent/ParentT, Next
	if (searchRelType->second != NextT &&
		((isLeftSynonym && isRightSynonym) ||
		(isLeftInteger && isRightInteger)) &&
		leftArg.compare(rightArg) == 0) {
		return false;
	}

	
	qo.insertClause(searchRelType->second, insertLeftType, leftArg,
		insertRightType, rightArg);

	return true;
}

/*Calls, CallsT*/
bool Preprocessor::parseClauseArg3(QueryObject &qo, string relType, string arg1, string arg2) {
	string leftArg = Utils::sanitise(arg1);
	string rightArg = Utils::sanitise(arg2);

	if (leftArg.length() < 1 || !isValidEntRef(leftArg)) {
		return false;
	}

	if (rightArg.length() < 1 || !isValidEntRef(rightArg)) {
		return false;
	}

	auto leftArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(leftArg));
	ParamType insertLeftType = leftArgType->second;
	bool isLeftSynonym = false;
	bool isLeftIdent = false;

	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertLeftType == SYNONYM) {
		if (!isDeclarationSynonymExist(leftArg)) {
			return false;
		}

		//Change the synonym to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(leftArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		// if is not procedure synonym, return false
		if (searchDeclareType->second != PROCEDURE) {
			return false;
		}

		insertLeftType = searchDeclareType->second;
		isLeftSynonym = true;
	}
	//Check if is Ident and store the content between the double quotes
	else if (insertLeftType == IDENT) {
		leftArg = (Utils::split(leftArg, SYMBOL_DOUBLE_QUOTE)).at(1);
		isLeftIdent = true;
	}
	//Check if is Integer
	else if (insertLeftType == INTEGER) {
		return false;
	}

	auto rightArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(rightArg));
	ParamType insertRightType = rightArgType->second;
	bool isRightSynonym = false;
	bool isRightIdent = false;

	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertRightType == SYNONYM) {
		if (!isDeclarationSynonymExist(rightArg)) {
			return false;
		}

		//Change the synonym to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(rightArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		// if is not procedure synonym, return false
		if (searchDeclareType->second != PROCEDURE) {
			return false;
		}

		insertRightType = searchDeclareType->second;
		isRightSynonym = true;
	}
	//Check if is Ident and store the content between the double quotes
	else if (insertRightType == IDENT) {
		rightArg = (Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE)).at(1);
		isRightIdent = true;
	}
	//Check if is Integer
	else if (insertRightType == INTEGER) {
		return false;
	}

	auto searchRelType = KEYWORDS_CLAUSES_3.find(relType);

	//Check for same synonym or statement number on the left and right param
	if (((isLeftSynonym && isRightSynonym) ||
		(isLeftIdent && isRightIdent)) &&
		leftArg.compare(rightArg) == 0) {
		return false;
	}


	qo.insertClause(searchRelType->second, insertLeftType, leftArg,
		insertRightType, rightArg);

	return true;
}

bool Preprocessor::parsePattern(QueryObject &qo, ParamType entityType, string entity, string arg1, string arg2) {
	
	string leftArg = Utils::sanitise(arg1);
	string rightArg = Utils::sanitise(arg2);

	if (leftArg.length() < 1 || !isValidVarRef(leftArg)) {
		return false;
	}

	if (rightArg.length() < 1 || !isValidExpressSpec(rightArg)) {
		return false;
	}


	auto leftArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(leftArg));
	ParamType insertLeftArgType = leftArgType->second;

	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertLeftArgType == SYNONYM) {
		if (!isDeclarationSynonymExist(leftArg)) {
			return false;
		}

		//Change the synonym to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(leftArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		// if is not a variable declaration for the right Param, return false
		if (searchDeclareType->second != VARIABLE) {
			return false;
		}

		insertLeftArgType = searchDeclareType->second;
	}
	//Check if is Ident and store the content between the double quotes
	else if (leftArgType->second == IDENT) {
		leftArg = Utils::sanitise((Utils::split(leftArg, SYMBOL_DOUBLE_QUOTE)).at(1));
	}

	auto rightArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(rightArg));

	//Check if is factor expresson-spec and store the content between the double quotes
	if (rightArgType->second == EXPR) {
		rightArg = (Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE)).at(1);
	}

	qo.insertPattern(entityType, entity, insertLeftArgType,
					leftArg, rightArgType->second, rightArg);

	return true;
}

int Preprocessor::retrieveArgType(string arg) {
	if (Utils::isInteger(arg)) {
		return 1;
	}
	else if (arg.find(SYMBOL_UNDERSCORE) != string::npos && arg.find(SYMBOL_DOUBLE_QUOTE) != string::npos) {
		
		vector<string> argSplit = Utils::split(arg, SYMBOL_DOUBLE_QUOTE);

		if (Utils::isInteger(argSplit.at(1))) {
			return 2;
		}
		else {
			return 3;
		}
	}
	else if (arg.find(SYMBOL_DOUBLE_QUOTE) != string::npos) {
		return 4;
	}
	else if (arg.size() == 1 && arg.find(SYMBOL_UNDERSCORE) != string::npos) {
		return 6;
	}
	else {
		return 5;
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

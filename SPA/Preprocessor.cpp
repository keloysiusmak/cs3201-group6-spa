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

const string SELECT_WORD = "Select";
const string SUCH_WORD = "such";
const string THAT_WORD = "that";
const string PATTERN_WORD = "pattern";

const string SYNONYM_WORD = "synonym";
const string ALL_WORD = "all";
const string INTEGER_WORD = "integer";
const string IDENT_WORD = "ident";
const string CONSTANT_WORD = "constant";
const string VAR_NAME_WORD = "var_name";

const unordered_set<string> KEYWORDS_PATTERN_TYPE = { "assign" };
const unordered_set<string> KEYWORDS_CLAUSES_1 = { "Modifies", "Uses" };
const unordered_set<string> KEYWORDS_CLAUSES_2 = { "Parent", "Parent*", "Follows", "Follows*" };

const unordered_map<string, ParamType> KEYWORDS_DECLARATIONS = { { "assign", ASSIGN }, { "stmt", STMT },
																{ "variable", VARIABLE }, { "while", WHILE },
																{ "constant", CONSTANT }, { "prog_line", PROG_LINE } };

const unordered_map<int, ParamType> NUMBER_MAPPING_REF_TYPE = { { 1, INTEGER }, { 2, CONSTANT },
															{ 3, VAR_NAME }, { 4, IDENT }, { 5, SYNONYM },
															{ 6, ALL } };

const regex synonymRegex("(^[a-z]([a-z]|[0-9]|[#])*$)");
const regex stmtRefRegex("(^(([a-z]([a-z]|[0-9]|[#])*$)|([_]$)|([0-9]+$)))");
const regex entRefRegex("(^(([a-z]([a-z]|[0-9]|[#])*$)|([_]$)|\"([a-z]([a-z]|[0-9]|[#])*)\"$))");
const regex expressSpecRegex("(^((_\"(([a-z]([a-z]|[0-9])*)|([0-9]+))\"_$)|[_]$))");

Preprocessor::Preprocessor() {

}

Preprocessor::Preprocessor(Evaluator &evaluator) {
	_evaluator = &evaluator;
}

void Preprocessor::setEvaluator(Evaluator &evaluator) {
	_evaluator = &evaluator;
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

	//Trim whitespace from a String
	string q = Utils::trim(query);

	//Split the string into different parts: declarations & query
	vector<string> declarations = Utils::split(q, SYMBOL_SEMICOLON);
	
	//The position of the query will always be the last element of the vector
	int queryIndex = declarations.size() - 1;

	//if queryIndex is 0, means no declarations at all
	if (queryIndex <= 0) {
		// insert evaluator invalid query api here
		(*_evaluator).invalidQuery("Invalid Query");
	}

	for (int i = 0; i < queryIndex; i++) {
		bool validateDeclaration = isValidDeclaration(Utils::trim(declarations.at(i)));

		if (!validateDeclaration) {
			// insert evaluator invalid query api here
			(*_evaluator).invalidQuery("Invalid Query");
			return;
		}
	}

	string queryPortion = Utils::trim(declarations.at(queryIndex));

	//validate whether is a valid query
	bool validQuery = isValidQuery(queryPortion);
	
	if (!validQuery) {
		// insert evaluator invalid query api here
		(*_evaluator).invalidQuery("Invalid Query");
	}	
};

bool Preprocessor::isValidDeclaration(string declaration) {

	vector<string> declarationArr = Utils::split(declaration, SYMBOL_SPACE);

	//Check if there is a declarationType and synonym
	if (declarationArr.size() < 2) {
		return false;
	}
	
	//Check if declarationType exists and the spelling of it
	bool checkDeclarationType = KEYWORDS_DECLARATIONS.find(declarationArr.at(0)) != KEYWORDS_DECLARATIONS.end();

	if (!checkDeclarationType) {
		return false;
	}

	//Check if there is a synonym exists
	vector<string> synonyms = Utils::split(declarationArr.at(1), SYMBOL_COMMA);

	if (synonyms.size() < 1) {
		return false;
	}

	for (int i = 0; i < synonyms.size(); i++) {
		string s = Utils::trim(synonyms.at(i));

		if (!isValidSynonym(s) || isDeclarationSynonymExist(s)) {
			return false;
		}
		insertDeclarationToMap(s, declarationArr.at(0));
	}

	return true;
}

bool Preprocessor::isValidQuery(string query) {

	QueryObject queryObject;
	vector<string> queryArr = Utils::split(query, SYMBOL_SPACE);

	//Check if Select word exists and if there's at least 2 elements in the query (e.g. "select", "s")
	if (queryArr.at(0).compare(SELECT_WORD) != 0 || queryArr.size() < 2) {
		return false;
	}

	//check if select synonym exist in the declarationMap
	if (!isDeclarationSynonymExist(queryArr.at(1))) {
		return false;
	}
	//Populate the selectType of QueryObject
	auto searchSynonym = declarationMap.find(queryArr.at(1));
	auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
	queryObject.insertSelectStmt(searchDeclareType->second, searchSynonym->first);

	//Check if there is any such that or pattern clause
	if (queryArr.size() == 2) {
		// insert evaluator query api here
		(*_evaluator).setQueryObject(queryObject);
		return true;
	}
	
	for (int i = 2; i < queryArr.size(); i++) {

		//check "such" word exists
		if (queryArr.at(i).compare(SUCH_WORD) == 0) {
			// check "that" word exists
			if ((i + 1) >= queryArr.size() || queryArr.at(i + 1).compare(THAT_WORD) != 0) {
				return false;
			}

			//check whether clause and param exists
			if ((i + 2) >= queryArr.size() || (i + 3) >= queryArr.size()) {
				return false;
			}

			//Split the clause from the open bracket e.g. Parent*(4,
			vector<string> clause = Utils::split(queryArr.at(i + 2), SYMBOL_OPEN_BRACKET);

			//Should have two string e.g. {Parent*}, {4,}
			if (clause.size() != 2) {
				return false;
			}

			//Check whether is a valid clause
			if (KEYWORDS_CLAUSES_1.find(clause.at(0)) != KEYWORDS_CLAUSES_1.end()) {
				if (!parseClauseArg1(queryObject, clause.at(0), clause.at(1), queryArr.at(i + 3))) {
					return false;
				}
			}
			else if (KEYWORDS_CLAUSES_2.find(clause.at(0)) != KEYWORDS_CLAUSES_2.end()) {
				if (!parseClauseArg2(queryObject, clause.at(0), clause.at(1), queryArr.at(i + 3))) {
					return false;
				}
			}
			else {
				return false;
			}

			//Finish processing this clause
			i += 3;		
		}
		//check wether "pattern" word exists
		else if (queryArr.at(i).compare(PATTERN_WORD) == 0) {

			//check whether pattern type and param exists
			if ((i + 1) >= queryArr.size() && (i + 2) >= queryArr.size()) {
				return false;
			}

			//Split the pattern type from the open bracket e.g. a(_,
			vector<string> patternType = Utils::split(queryArr.at(i + 1), SYMBOL_OPEN_BRACKET);

			//Should have two string e.g. {a}, {_,}
			//the patternType should exist in the declarationMap
			if (patternType.size() != 2 ||  
				!isDeclarationSynonymExist(patternType.at(0))) {
				return false;
			}

			searchSynonym = declarationMap.find(patternType.at(0));

			//check whether patternType is valid
			if (KEYWORDS_PATTERN_TYPE.find(searchSynonym->second) == KEYWORDS_PATTERN_TYPE.end()) {
				return false;
			}

			searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

			if (!parsePattern(queryObject, searchDeclareType->second, patternType.at(0), patternType.at(1), queryArr.at(i + 2))) {
				return false;
			}

			//Finish processing this pattern
			i += 2;
		}
		else {
			return false;
		}
	}

	// insert evaluator query api here
	(*_evaluator).setQueryObject(queryObject);
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

bool Preprocessor::parseClauseArg1(QueryObject &qo, string relType, string arg1, string arg2) {

	vector<string> arg1Split = Utils::split(arg1, SYMBOL_COMMA);

	if (arg1Split.size() < 1 || !isValidStmtRef(Utils::trim(arg1Split.at(0)))) {
		return false;
	}

	vector<string> arg2Split = Utils::split(arg2, SYMBOL_CLOSE_BRACKET);

	if (arg2Split.size() < 1 || !isValidEntRef(Utils::trim(arg2Split.at(0)))) {
		return false;
	}

	string leftArg = Utils::trim(arg1Split.at(0));
	string rightArg = Utils::trim(arg2Split.at(0));

	auto leftArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(leftArg));
	ParamType insertLeftType = leftArgType->second;

	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertLeftType == SYNONYM) {
		if (!isDeclarationSynonymExist(leftArg)) {
			return false;
		}
		
		//Change the synonym to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(leftArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
		insertLeftType = searchDeclareType->second;
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
		insertRightType = searchDeclareType->second;
	}
	//Check if is Ident and store the content between the double quotes
	else if (insertRightType == IDENT) {
		rightArg = (Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE)).at(1);
	}

	qo.insertClause(relType, insertLeftType, leftArg,
		insertRightType, rightArg);

	return true;
}

bool Preprocessor::parseClauseArg2(QueryObject &qo, string relType, string arg1, string arg2) {

	vector<string> arg1Split = Utils::split(arg1, SYMBOL_COMMA);

	if (arg1Split.size() < 1 || !isValidStmtRef(Utils::trim(arg1Split.at(0)))) {
		return false;
	}

	vector<string> arg2Split = Utils::split(arg2, SYMBOL_CLOSE_BRACKET);

	if (arg2Split.size() < 1 || !isValidStmtRef(Utils::trim(arg2Split.at(0)))) {
		return false;
	}

	string leftArg = Utils::trim(arg1Split.at(0));
	string rightArg = Utils::trim(arg2Split.at(0));

	auto leftArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(leftArg));
	ParamType insertLeftType = leftArgType->second;

	//Check if is synonym and whether the synonym exists in declarationMap
	if (insertLeftType == SYNONYM) {
		if (!isDeclarationSynonymExist(leftArg)) {
			return false;
		}

		//Change the synonym to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(leftArg);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
		insertLeftType = searchDeclareType->second;
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
		insertRightType = searchDeclareType->second;
	}
	//Check if is Ident and store the content between the double quotes
	else if (insertRightType == IDENT) {
		rightArg = (Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE)).at(1);
	}

	qo.insertClause(relType, insertLeftType, leftArg,
		insertRightType, rightArg);

	return true;
}

bool Preprocessor::parsePattern(QueryObject &qo, ParamType entityType, string entity, string arg1, string arg2) {
	
	vector<string> arg1Split = Utils::split(arg1, SYMBOL_COMMA);

	if (arg1Split.size() < 1 || !isValidEntRef(Utils::trim(arg1Split.at(0)))) {
		return false;
	}

	vector<string> arg2Split = Utils::split(arg2, SYMBOL_CLOSE_BRACKET);

	if (arg2Split.size() < 1 || !isValidExpressSpec(Utils::trim(arg2Split.at(0)))) {
		return false;
	}

	string leftArg = Utils::trim(arg1Split.at(0));
	string rightArg = Utils::trim(arg2Split.at(0));

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
		insertLeftArgType = searchDeclareType->second;
	}
	//Check if is Ident and store the content between the double quotes
	else if (leftArgType->second == IDENT) {
		leftArg = (Utils::split(leftArg, SYMBOL_DOUBLE_QUOTE)).at(1);
	}

	auto rightArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(rightArg));

	//Check if is factor expresson-spec and store the content between the double quotes
	if (rightArgType->second == VAR_NAME || rightArgType->second == CONSTANT) {
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


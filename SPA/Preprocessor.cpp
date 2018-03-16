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
const string DELIM_STRING = " ,()<>=";
const vector<char> DELIMITERS_DECLARATION{};
const vector<char> DELIMITERS_QUERY{ SYMBOL_COMMA, SYMBOL_CLOSE_BRACKET, SYMBOL_ANGLE_OPEN_BRACKETS,
SYMBOL_ANGLE_CLOSE_BRACKETS, SYMBOL_EQUALS };

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

const unordered_set<string> KEYWORDS_PATTERN_TYPE = { "assign", "while", "if" };
const unordered_map<string, RelRef> KEYWORDS_CLAUSES = { { "Modifies", Modifies }, { "Uses", Uses }, 
{ "Parent", Parent },{ "Parent*", ParentT },{ "Follows", Follows },{ "Follows*", FollowsT }, 
{ "Next", Next },{ "NextT", NextT },{ "Calls", Calls },{ "Calls*", CallsT } };

const unordered_map<string, AttributeType> KEYWORDS_WITH_TYPE = { { "procName", PROCNAME },
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

const regex synonymRegex("(^[a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)");
const regex identRegex("(^(\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$))");
const regex stmtRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|([0-9]+$)))");
const regex entRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$)|([0-9]+$))");
const regex varRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$))");
const regex expressSpecRegex("(^((_\"(([a-zA-Z]([a-zA-Z]|[0-9])*)|([0-9]+))\"_$)|[_]$))");
const regex attrRefRegex("(^[a-zA-Z]([a-zA-Z]|[0-9]|[#])*[.](procName|varName|value|stmt#)$)");
const regex refRegex("(^(([0-9]+$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$|[a-zA-Z]([a-zA-Z]|[0-9]|[#])*[.](procName|varName|value|stmt#)))");

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

			//even position must be elem
			if (endOfSelectStatement % 2 == 0) {
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
					AttributeType getAttrName = KEYWORDS_WITH_TYPE.find(attrRef.at(1))->second;
					queryObject.insertSelectStmt(searchDeclareType->second, searchSynonym->first, getAttrName);
				}
				else if (isValidSynonym(elem)) {
					//check if select synonym exist in the declarationMap
					if (!isDeclarationSynonymExist(elem)) {
						return false;
					}

					//Populate the selectType of QueryObject
					auto searchSynonym = declarationMap.find(elem);
					auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
					queryObject.insertSelectStmt(searchDeclareType->second, searchSynonym->first, NONE);
				}
				else {
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
			AttributeType getAttrName = KEYWORDS_WITH_TYPE.find(attrRef.at(1))->second;
			queryObject.insertSelectStmt(searchDeclareType->second, searchSynonym->first, getAttrName);
		}
		else if (isValidSynonym(elem)) {
			//check if select synonym exist in the declarationMap
			if (!isDeclarationSynonymExist(elem)) {
				return false;
			}

			//Populate the selectType of QueryObject
			auto searchSynonym = declarationMap.find(elem);
			auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
			queryObject.insertSelectStmt(searchDeclareType->second, searchSynonym->first, NONE);
		}
		else {
			return false;
		}
	}

	endOfSelectStatement++;

	//Check if there is any such that or pattern clause
	if (queryArr.size() == endOfSelectStatement) {
		// insert evaluator query api here
		//(*_evaluator).setQueryObject(queryObject);
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

			if (!parseClauseArg(queryObject, queryArr.at(i + 2), leftArg, rightArg)) {
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

			auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

			//Add patternType
			patternLength++;

			//Add all the left Param
			string leftArg = retrieveParamFromQuery(queryArr, patternLength, i, string(1, SYMBOL_COMMA));

			//Add all the right Param if is assignpt
			string rightArg;
			if (searchDeclareType->second == ASSIGN) {
				rightArg = retrieveParamFromQuery(queryArr, patternLength, i, string(1, SYMBOL_CLOSE_BRACKET));
			}
			else {
				rightArg = EMPTY_STRING;
				//keep track of the number of underscore
				int countUnderscore = 0;

				if (searchDeclareType->second == IF) {
					//iterate through the syntax

					while (queryArr.at(i + patternLength).at(0) != SYMBOL_CLOSE_BRACKET) {
						//even position must be underscore
						if (patternLength % 2) {
							if (queryArr.at(i + patternLength).at(0) == SYMBOL_UNDERSCORE) {
								countUnderscore++;
							}
							else {
								return false;
							}
						}
						else {
							//check if is comma
							if (queryArr.at(i + patternLength).at(0) != SYMBOL_COMMA) {
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

					while (queryArr.at(i + patternLength).at(0) != SYMBOL_CLOSE_BRACKET) {
						//even position must be underscore
						if (patternLength % 2) {
							if (queryArr.at(i + patternLength).at(0) == SYMBOL_UNDERSCORE) {
								countUnderscore++;
							}
							else {
								return false;
							}
						}
						else {
							//check if is comma
							if (queryArr.at(i + patternLength).at(0) != SYMBOL_COMMA) {
								return false;
							}
						}
						patternLength++;
					}

					//whilept should have 2 underscore (right param)
					if (countUnderscore != 1) {
						return false;
					}
				}
				patternLength++;
			}

			if (!parsePattern(queryObject, searchDeclareType->second, queryArr.at(i + 1), leftArg, rightArg)) {
				return false;
			}

			//Finish processing this pattern
			i += (patternLength - 1);

			prevAndClause = 2;
		}
		//check whether "with" word exists
		else if (queryArr.at(i).compare(WITH_WORD) == 0) {

			if ((i + 1) >= queryArr.size() ||
				(i + 2) >= queryArr.size() ||
				(i + 3) >= queryArr.size()) {
				return false;
			}

			string attrRef = Utils::sanitise(queryArr.at(i + 1));
			char equalSign = Utils::sanitise(queryArr.at(i + 2)).at(0);
			string ref = Utils::sanitise(queryArr.at(i + 3));

			if (!isValidAttrRef(attrRef) ||
				equalSign != SYMBOL_EQUALS ||
				!isValidRef(ref)) {
				return false;
			}

			if (!parseWithClause(queryObject, attrRef, ref)) {
				return false;
			}

			i += 3;

			prevAndClause = 3;
		}
		else if (queryArr.at(i).compare(AND_WORD) == 0) {

			// check whether "and" have continuation
			if ((i + 1) >= queryArr.size()) {
				return false;
			}

			//such that
			if (prevAndClause == 1) {

				//Not a valid such that clause
				if (KEYWORDS_CLAUSES.find(queryArr.at(i + 1)) == KEYWORDS_CLAUSES.end()) {
					return false;
				}

				int clauseLength = 2;

				//Add all the left Param
				string leftArg = retrieveParamFromQuery(queryArr, clauseLength, i, string(1, SYMBOL_COMMA));

				//Add all the right Param
				string rightArg = retrieveParamFromQuery(queryArr, clauseLength, i, string(1, SYMBOL_CLOSE_BRACKET));

				if (!parseClauseArg(queryObject, queryArr.at(i + 1), leftArg, rightArg)) {
					return false;
				}

				//Finish processing this clause
				i += (clauseLength - 1);

			}
			//pattern
			else if (prevAndClause == 2) {
				//Keep track of the length of pattern
				int patternLength = 1;

				//check whether pattern type exists and
				//the patternType should exist in the declarationMap
				if ((i + 1) >= queryArr.size() || !isDeclarationSynonymExist(queryArr.at(i + 1))) {
					return false;
				}
				string entityType = Utils::sanitise(queryArr.at(i + 1));
				auto searchSynonym = declarationMap.find(entityType);

				//check whether patternType is valid
				if (KEYWORDS_PATTERN_TYPE.find(searchSynonym->second) == KEYWORDS_PATTERN_TYPE.end()) {
					return false;
				}

				auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

				//Add patternType
				patternLength++;

				//Add all the left Param
				string leftArg = retrieveParamFromQuery(queryArr, patternLength, i, string(1, SYMBOL_COMMA));

				//Add all the right Param if is assignpt
				string rightArg;
				if (searchDeclareType->second == ASSIGN) {
					rightArg = retrieveParamFromQuery(queryArr, patternLength, i, string(1, SYMBOL_CLOSE_BRACKET));
				}
				else {
					rightArg = EMPTY_STRING;
					//keep track of the number of underscore
					int countUnderscore = 0;

					if (searchDeclareType->second == IF) {
						//iterate through the syntax

						while (queryArr.at(i + patternLength).at(0) != SYMBOL_CLOSE_BRACKET) {
							//even position must be underscore
							if (patternLength % 2) {
								if (queryArr.at(i + patternLength).at(0) == SYMBOL_UNDERSCORE) {
									countUnderscore++;
								}
								else {
									return false;
								}
							}
							else {
								//check if is comma
								if (queryArr.at(i + patternLength).at(0) != SYMBOL_COMMA) {
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

						while (queryArr.at(i + patternLength).at(0) != SYMBOL_CLOSE_BRACKET) {
							//even position must be underscore
							if (patternLength % 2) {
								if (queryArr.at(i + patternLength).at(0) == SYMBOL_UNDERSCORE) {
									countUnderscore++;
								}
								else {
									return false;
								}
							}
							else {
								//check if is comma
								if (queryArr.at(i + patternLength).at(0) != SYMBOL_COMMA) {
									return false;
								}
							}
							patternLength++;
						}

						//whilept should have 2 underscore (right param)
						if (countUnderscore != 1) {
							return false;
						}
					}
					patternLength++;
				}

				if (!parsePattern(queryObject, searchDeclareType->second, entityType, leftArg, rightArg)) {
					return false;
				}

				//Finish processing this pattern
				i += (patternLength - 1);
			}
			//with
			else if (prevAndClause == 3) {

			}
			else {
				return false;
			}
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

bool Preprocessor::isValidExpressSpec(string expressSpec) {
	if (expressSpec.length() == 0) {
		return false;
	}

	return regex_match(expressSpec, expressSpecRegex);
}

bool Preprocessor::isValidAttrRef(string attrRef) {
	if (attrRef.length() == 0) {
		return false;
	}

	return regex_match(attrRef, attrRefRegex);
}

bool Preprocessor::isValidAttrName(ParamType synonymType, string attrRef) {
	auto checkAttrRef = KEYWORDS_WITH_TYPE.find(attrRef);

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

bool Preprocessor::isValidAttrCond(ParamType attrRef, string ref) {

	//attrRef
	if (isValidAttrRef(ref)) {

		vector<string> refArr = Utils::split(ref, SYMBOL_FULL_STOP);

		if (!isDeclarationSynonymExist(refArr.at(0))) {
			return false;
		}

		//Change the synonym of attrRef to the declaration type with reference to the declarationMap
		auto searchSynonym = declarationMap.find(refArr.at(0));
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		if (!isValidAttrName(searchDeclareType->second, refArr.at(1))) {
			return false;
		}

		auto getAttrName = KEYWORDS_WITH_TYPE.find(refArr.at(1));

		if (attrRef == PROCNAME || attrRef == VARNAME) {
			if (getAttrName->second == PROCNAME || getAttrName->second == VARNAME) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (getAttrName->second == VALUE || getAttrName->second == STMT_NO) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	//IDENT or INTEGER
	else {
		if (Utils::isInteger(ref)) {
			if (attrRef == VALUE || attrRef == STMT_NO) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (attrRef == PROCNAME || attrRef == VARNAME) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}

bool Preprocessor::isDeclarationSynonymExist(string synonym) {
	//Check if there exist a synonym in the declarationMap
	auto checkSynonymExist = declarationMap.find(synonym);

	if (checkSynonymExist == declarationMap.end()) {
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
		leftArg = (Utils::split(leftArg, SYMBOL_DOUBLE_QUOTE)).at(1);
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
		rightArg = (Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE)).at(1);
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
	case NextT:
		if (!isValidStmtRef(leftArg) || !isValidStmtRef(rightArg)) {
			return false;
		}
		break;
	default:
		return false;
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

	if (leftArg.length() < 1 || !isValidVarRef(leftArg)) {
		return false;
	}

	//if (entityType == ASSIGN && (rightArg.length() < 1 || !isValidExpressSpec(rightArg))) {
	//	return false;
	//}


	//auto leftArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(leftArg));
	//ParamType insertLeftArgType = leftArgType->second;

	////Check if is synonym and whether the synonym exists in declarationMap
	//if (insertLeftArgType == SYNONYM) {
	//	if (!isDeclarationSynonymExist(leftArg)) {
	//		return false;
	//	}

	//	//Change the synonym to the declaration type with reference to the declarationMap
	//	auto searchSynonym = declarationMap.find(leftArg);
	//	auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

	//	// if is not a variable declaration for the right Param, return false
	//	if (searchDeclareType->second != VARIABLE) {
	//		return false;
	//	}

	//	insertLeftArgType = searchDeclareType->second;
	//}
	////Check if is Ident and store the content between the double quotes
	//else if (leftArgType->second == IDENT) {
	//	leftArg = Utils::sanitise((Utils::split(leftArg, SYMBOL_DOUBLE_QUOTE)).at(1));
	//}

	//if (entityType == ASSIGN) {
	//	auto rightArgType = NUMBER_MAPPING_REF_TYPE.find(retrieveArgType(rightArg));

	//	//Check if is factor expresson-spec and store the content between the double quotes
	//	if (rightArgType->second == EXPR) {
	//		rightArg = (Utils::split(rightArg, SYMBOL_DOUBLE_QUOTE)).at(1);
	//	}

	//	qo.insertPattern(entityType, entity, insertLeftArgType,
	//		leftArg, rightArgType->second, rightArg);
	//}
	//else {
	//	qo.insertPattern(entityType, entity, insertLeftArgType,
	//		leftArg, ALL, Utils::trim(SYMBOL_UNDERSCORE + rightArg));
	//}

	return true;
}

bool Preprocessor::parseWithClause(QueryObject &qo, string attrRef, string ref) {

	//vector<string> attrRefArr = Utils::split(attrRef, SYMBOL_FULL_STOP);

	//if (!isDeclarationSynonymExist(attrRefArr.at(0))) {
	//	return false;
	//}

	////Change the synonym of attrRef to the declaration type with reference to the declarationMap
	//auto searchSynonym = declarationMap.find(attrRefArr.at(0));
	//auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

	//if (!isValidAttrName(searchDeclareType->second, attrRefArr.at(1))) {
	//	return false;
	//}

	//auto getAttrName = KEYWORDS_WITH_TYPE.find(attrRefArr.at(1));

	//if (!isValidAttrCond(getAttrName->second, ref)) {
	//	return false;
	//}

	////attrRef
	//if (isValidAttrRef(ref)) {

	//	vector<string> refArr = Utils::split(ref, SYMBOL_FULL_STOP);
	//	auto getRef_AttrName = KEYWORDS_WITH_TYPE.find(refArr.at(1));

	//	qo.updateWithClause(getAttrName->second, attrRefArr.at(0),
	//		getRef_AttrName->second, ref);
	//}
	////INTEGER
	//else if (Utils::isInteger(ref)) {
	//	qo.updateWithClause(getAttrName->second, attrRefArr.at(0),
	//		INTEGER, ref);
	//}
	////IDENTITY
	//else {
	//	qo.updateWithClause(getAttrName->second, attrRefArr.at(0),
	//		IDENT, ref);
	//}

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
	return 0;
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

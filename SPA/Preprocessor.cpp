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
const char SYMBOL_NEGATION = '~';

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
const string OR_WORD = "or";
const string NOT_WORD = "not";
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
const unordered_map<char, OPERATORS> MAP_OPERATORS = { { '*', AND }, { '+', OR } };
const unordered_map<string, char> MAP_SYMBOL_OPERATORS = { { "and", '*' }, { "or", '+' } };

const regex synonymRegex("(^[a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)");
const regex identRegex("(^(\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$))");
const regex stmtRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|([0-9]+$)))");
const regex entRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$)|([0-9]+$))");
const regex varRefRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([_]$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$))");
const regex attrRefRegex("(^[a-zA-Z]([a-zA-Z]|[0-9]|[#])*[.](procName|varName|value|stmt#)$)");
const regex refRegex("(^(([a-zA-Z]([a-zA-Z]|[0-9]|[#])*$)|([0-9]+$)|\"([a-zA-Z]([a-zA-Z]|[0-9]|[#])*)\"$|[a-zA-Z]([a-zA-Z]|[0-9]|[#])*[.](procName|varName|value|stmt#)))");

Preprocessor::Preprocessor() {
	errorMessage = EMPTY_STRING;
	isErrorExist = false;
}

string Preprocessor::getErrorMessage() {
	return errorMessage;
}

std::vector<QueryContent> Preprocessor::getQueryContent() {
	return vqc;
}

bool Preprocessor::getIsErrorExist() {
	return isErrorExist;
}

void Preprocessor::insertDeclarationToMap(string synonym, string declaration) {
	declarationMap.insert({ synonym, declaration });
}

unordered_map<string, string> Preprocessor::getDeclarationMap() {
	return declarationMap;
}

void Preprocessor::preprocessQuery(string query) {

	//Clear the contents of vector<QueryContent>
	vqc.clear();

	//Clear the contents in declarationMap first before processing the query
	declarationMap.clear();

	//sanitise whitespace from a String
	string q = Utils::sanitise(query);

	//Split the string into different parts: declarations & query
	vector<string> declarations = Utils::split(q, SYMBOL_SEMICOLON);

	//The position of the query will always be the last element of the vector
	int queryIndex = declarations.size() - 1;

	//Reset Error
	isErrorExist = false;

	//if queryIndex is 0, means no declarations at all
	if (queryIndex <= 0) {

		if (!isValidQuery(q)) {

			isErrorExist = true;

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
			isErrorExist = true;
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
		isErrorExist = true;
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

	QueryContent queryContent;
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
				if (!isValidElem(queryArr, endOfSelectStatement, queryContent)) {
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
		queryContent.insertSelect(BOOLEAN, BOOLEAN_WORD, NONE);
	}
	//single elem
	else {
		if (!isValidElem(queryArr, endOfSelectStatement, queryContent)) {
			return false;
		}
	}

	endOfSelectStatement++;

	//Check if there is any such that, pattern or with clause
	if (queryArr.size() == endOfSelectStatement) {
		vqc.push_back(queryContent);
		return true;
	}

	//Process the NOT bracket by performing Demorgan's law

	int prevSelectedClause = 0;

	stack<char> suchThatOperator;
	stack<char> patternOperator;
	stack<char> withOperator;

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

			prevSelectedClause = 1;

			if (queryContent.getClauses().size() != 0) {
				while (!suchThatOperator.empty() && higherPrecedenceValidate(suchThatOperator.top(), SYMBOL_MULTIPLICATION)) {
					queryContent.insertOperator(CLAUSE, MAP_OPERATORS.find(suchThatOperator.top())->second);
					suchThatOperator.pop();
				}

				suchThatOperator.push(SYMBOL_MULTIPLICATION);
			}

			//Check whether the next element is a open bracket
			if (i + clauseLength < queryArr.size() && queryArr.at(i + clauseLength).at(0) == SYMBOL_OPEN_BRACKET) {
				if (!convertToPostFix(queryArr, prevSelectedClause, clauseLength, i, queryContent)) {
					return false;
				}
			}
			else if (i + clauseLength < queryArr.size() && queryArr.at(i + clauseLength).compare(NOT_WORD) == 0) {

				//Add "not"
				clauseLength++;

				if (i + clauseLength < queryArr.size() && queryArr.at(i + clauseLength).at(0) == SYMBOL_OPEN_BRACKET) {
					int startPtrNegation = queryContent.getClauses().size() - 1;
					if (!convertToPostFix(queryArr, prevSelectedClause, clauseLength, i, queryContent)) {
						return false;
					}
					int endPtrNegation = queryContent.getClauses().size() - 1;

					while (endPtrNegation != startPtrNegation) {
						queryContent.setNegation(CLAUSE, endPtrNegation);
						endPtrNegation--;
					}
				}
				else {
					if (!isValidClause(queryArr, clauseLength, i, queryContent, true)) {
						return false;
					}
				}

			}
			else {
				if (!isValidClause(queryArr, clauseLength, i, queryContent, false)) {
					return false;
				}
			}

			//Finish processing this clause
			i += (clauseLength - 1);
		}
		//check whether "pattern" word exists
		else if (queryArr.at(i).compare(PATTERN_WORD) == 0) {

			//Keep track of the length of pattern
			int patternLength = 1;

			prevSelectedClause = 2;

			if (queryContent.getPattern().size() != 0) {
				while (!patternOperator.empty() && higherPrecedenceValidate(patternOperator.top(), SYMBOL_MULTIPLICATION)) {
					queryContent.insertOperator(PATTERN, MAP_OPERATORS.find(patternOperator.top())->second);
					patternOperator.pop();
				}

				patternOperator.push(SYMBOL_MULTIPLICATION);
			}

			if (i + patternLength < queryArr.size() && queryArr.at(i + patternLength).at(0) == SYMBOL_OPEN_BRACKET) {
				if (!convertToPostFix(queryArr, prevSelectedClause, patternLength, i, queryContent)) {
					return false;
				}
			}
			else if (i + patternLength < queryArr.size() && queryArr.at(i + patternLength).compare(NOT_WORD) == 0) {

				//Add "not"
				patternLength++;

				if (i + patternLength < queryArr.size() && queryArr.at(i + patternLength).at(0) == SYMBOL_OPEN_BRACKET) {
					int startPtrNegation = queryContent.getClauses().size() - 1;
					if (!convertToPostFix(queryArr, prevSelectedClause, patternLength, i, queryContent)) {
						return false;
					}
					int endPtrNegation = queryContent.getClauses().size() - 1;

					while (endPtrNegation != startPtrNegation) {
						queryContent.setNegation(PATTERN, endPtrNegation);
						endPtrNegation--;
					}
				}
				else {
					if (!isValidPattern(queryArr, patternLength, i, queryContent, true)) {
						return false;
					}
				}

			}
			else {
				if (!isValidPattern(queryArr, patternLength, i, queryContent, false)) {
					return false;
				}
			}

			//Finish processing this pattern
			i += (patternLength - 1);
		}
		//check whether "with" word exists
		else if (queryArr.at(i).compare(WITH_WORD) == 0) {

			//Keep track of the length of with clause
			int withLength = 1;
			
			prevSelectedClause = 3;

			if (queryContent.getWithClauses().size() != 0) {
				while (!withOperator.empty() && higherPrecedenceValidate(withOperator.top(), SYMBOL_MULTIPLICATION)) {
					queryContent.insertOperator(WITH_CLAUSE, MAP_OPERATORS.find(withOperator.top())->second);
					withOperator.pop();
				}

				withOperator.push(SYMBOL_MULTIPLICATION);
			}

			if (i + withLength < queryArr.size() && queryArr.at(i + withLength).at(0) == SYMBOL_OPEN_BRACKET) {
				if (!convertToPostFix(queryArr, prevSelectedClause, withLength, i, queryContent)) {
					return false;
				}
				withLength--;
			}
			else if (i + withLength < queryArr.size() && queryArr.at(i + withLength).compare(NOT_WORD) == 0) {

				//Add "not"
				withLength++;

				if (i + withLength < queryArr.size() && queryArr.at(i + withLength).at(0) == SYMBOL_OPEN_BRACKET) {
					int startPtrNegation = queryContent.getClauses().size() - 1;
					if (!convertToPostFix(queryArr, prevSelectedClause, withLength, i, queryContent)) {
						return false;
					}
					withLength--;
					int endPtrNegation = queryContent.getClauses().size() - 1;

					while (endPtrNegation != startPtrNegation) {
						queryContent.setNegation(WITH_CLAUSE, endPtrNegation);
						endPtrNegation--;
					}
				}
				else {
					if (!isValidWithClause(queryArr, withLength, i, queryContent, true)) {
						return false;
					}
				}

			}
			else {
				if (!isValidWithClause(queryArr, withLength, i, queryContent, false)) {
					return false;
				}
			}

			i += withLength;		
		}
		else if (queryArr.at(i).compare(AND_WORD) == 0) {

			// check whether "and" have continuation
			if ((i + 1) >= queryArr.size()) {
				return false;
			}

			//such that
			if (prevSelectedClause == 1) {

				int clauseLength = 1;

				while (!suchThatOperator.empty() && higherPrecedenceValidate(suchThatOperator.top(), SYMBOL_MULTIPLICATION)) {
					queryContent.insertOperator(CLAUSE, MAP_OPERATORS.find(suchThatOperator.top())->second);
					suchThatOperator.pop();
				}

				suchThatOperator.push(SYMBOL_MULTIPLICATION);

				//Check whether the next element is a open bracket
				if (queryArr.at(i + clauseLength).at(0) == SYMBOL_OPEN_BRACKET) {
					if (!convertToPostFix(queryArr, prevSelectedClause, clauseLength, i, queryContent)) {
						return false;
					}
				}
				else if (i + clauseLength < queryArr.size() && queryArr.at(i + clauseLength).compare(NOT_WORD) == 0) {

					//Add "not"
					clauseLength++;

					if (i + clauseLength < queryArr.size() && queryArr.at(i + clauseLength).at(0) == SYMBOL_OPEN_BRACKET) {
						int startPtrNegation = queryContent.getClauses().size() - 1;
						if (!convertToPostFix(queryArr, prevSelectedClause, clauseLength, i, queryContent)) {
							return false;
						}
						int endPtrNegation = queryContent.getClauses().size() - 1;

						while (endPtrNegation != startPtrNegation) {
							queryContent.setNegation(CLAUSE, endPtrNegation);
							endPtrNegation--;
						}
					}
					else {
						if (!isValidClause(queryArr, clauseLength, i, queryContent, true)) {
							return false;
						}
					}

				}
				else {
					if (!isValidClause(queryArr, clauseLength, i, queryContent, false)) {
						return false;
					}
				}

				//Finish processing this clause
				i += (clauseLength - 1);

			}
			//pattern
			else if (prevSelectedClause == 2) {
				//Keep track of the length of pattern
				int patternLength = 1;

				while (!patternOperator.empty() && higherPrecedenceValidate(patternOperator.top(), SYMBOL_MULTIPLICATION)) {
					queryContent.insertOperator(PATTERN, MAP_OPERATORS.find(patternOperator.top())->second);
					patternOperator.pop();
				}

				patternOperator.push(SYMBOL_MULTIPLICATION);

				if (queryArr.at(i + patternLength).at(0) == SYMBOL_OPEN_BRACKET) {
					if (!convertToPostFix(queryArr, prevSelectedClause, patternLength, i, queryContent)) {
						return false;
					}
				}
				else if (i + patternLength < queryArr.size() && queryArr.at(i + patternLength).compare(NOT_WORD) == 0) {

					//Add "not"
					patternLength++;

					if (i + patternLength < queryArr.size() && queryArr.at(i + patternLength).at(0) == SYMBOL_OPEN_BRACKET) {
						int startPtrNegation = queryContent.getClauses().size() - 1;
						if (!convertToPostFix(queryArr, prevSelectedClause, patternLength, i, queryContent)) {
							return false;
						}
						int endPtrNegation = queryContent.getClauses().size() - 1;

						while (endPtrNegation != startPtrNegation) {
							queryContent.setNegation(PATTERN, endPtrNegation);
							endPtrNegation--;
						}
					}
					else {
						if (!isValidPattern(queryArr, patternLength, i, queryContent, true)) {
							return false;
						}
					}

				}
				else {
					if (!isValidPattern(queryArr, patternLength, i, queryContent, false)) {
						return false;
					}
				}

				//Finish processing this pattern
				i += (patternLength - 1);
			}
			//with
			else if (prevSelectedClause == 3) {

				//Keep track of the length of with clause
				int withLength = 1;

				while (!withOperator.empty() && higherPrecedenceValidate(withOperator.top(), SYMBOL_MULTIPLICATION)) {
					queryContent.insertOperator(WITH_CLAUSE, MAP_OPERATORS.find(withOperator.top())->second);
					withOperator.pop();
				}

				withOperator.push(SYMBOL_MULTIPLICATION);

				if (queryArr.at(i + withLength).at(0) == SYMBOL_OPEN_BRACKET) {
					if (!convertToPostFix(queryArr, prevSelectedClause, withLength, i, queryContent)) {
						return false;
					}
					withLength--;
				}
				else if (i + withLength < queryArr.size() && queryArr.at(i + withLength).compare(NOT_WORD) == 0) {

					//Add "not"
					withLength++;

					if (i + withLength < queryArr.size() && queryArr.at(i + withLength).at(0) == SYMBOL_OPEN_BRACKET) {
						int startPtrNegation = queryContent.getClauses().size() - 1;
						if (!convertToPostFix(queryArr, prevSelectedClause, withLength, i, queryContent)) {
							return false;
						}
						withLength--;
						int endPtrNegation = queryContent.getClauses().size() - 1;

						while (endPtrNegation != startPtrNegation) {
							queryContent.setNegation(WITH_CLAUSE, endPtrNegation);
							endPtrNegation--;
						}
					}
					else {
						if (!isValidWithClause(queryArr, withLength, i, queryContent, true)) {
							return false;
						}
					}

				}
				else {
					if (!isValidWithClause(queryArr, withLength, i, queryContent, false)) {
						return false;
					}
				}

				i += withLength;

			}
			else {
				return false;
			}
		}
		else if (queryArr.at(i).compare(OR_WORD) == 0) {

			// check whether "and" have continuation
			if ((i + 1) >= queryArr.size()) {
				return false;
			}

			//such that
			if (prevSelectedClause == 1) {

				int clauseLength = 1;

				while (!suchThatOperator.empty() && higherPrecedenceValidate(suchThatOperator.top(), SYMBOL_PLUS)) {
					queryContent.insertOperator(CLAUSE, MAP_OPERATORS.find(suchThatOperator.top())->second);
					suchThatOperator.pop();
				}

				suchThatOperator.push(SYMBOL_PLUS);

				//Check whether the next element is a open bracket
				if (queryArr.at(i + clauseLength).at(0) == SYMBOL_OPEN_BRACKET) {
					if (!convertToPostFix(queryArr, prevSelectedClause, clauseLength, i, queryContent)) {
						return false;
					}
				}
				else if (i + clauseLength < queryArr.size() && queryArr.at(i + clauseLength).compare(NOT_WORD) == 0) {

					//Add "not"
					clauseLength++;

					if (i + clauseLength < queryArr.size() && queryArr.at(i + clauseLength).at(0) == SYMBOL_OPEN_BRACKET) {
						int startPtrNegation = queryContent.getClauses().size() - 1;
						if (!convertToPostFix(queryArr, prevSelectedClause, clauseLength, i, queryContent)) {
							return false;
						}
						int endPtrNegation = queryContent.getClauses().size() - 1;

						while (endPtrNegation != startPtrNegation) {
							queryContent.setNegation(CLAUSE, endPtrNegation);
							endPtrNegation--;
						}
					}
					else {
						if (!isValidClause(queryArr, clauseLength, i, queryContent, true)) {
							return false;
						}
					}

				}
				else {
					if (!isValidClause(queryArr, clauseLength, i, queryContent, false)) {
						return false;
					}
				}

				//Finish processing this clause
				i += (clauseLength - 1);

			}
			//pattern
			else if (prevSelectedClause == 2) {
				//Keep track of the length of pattern
				int patternLength = 1;

				while (!patternOperator.empty() && higherPrecedenceValidate(patternOperator.top(), SYMBOL_PLUS)) {
					queryContent.insertOperator(PATTERN, MAP_OPERATORS.find(patternOperator.top())->second);
					patternOperator.pop();
				}

				patternOperator.push(SYMBOL_PLUS);

				if (queryArr.at(i + patternLength).at(0) == SYMBOL_OPEN_BRACKET) {
					if (!convertToPostFix(queryArr, prevSelectedClause, patternLength, i, queryContent)) {
						return false;
					}
				}
				else if (i + patternLength < queryArr.size() && queryArr.at(i + patternLength).compare(NOT_WORD) == 0) {

					//Add "not"
					patternLength++;

					if (i + patternLength < queryArr.size() && queryArr.at(i + patternLength).at(0) == SYMBOL_OPEN_BRACKET) {
						int startPtrNegation = queryContent.getClauses().size() - 1;
						if (!convertToPostFix(queryArr, prevSelectedClause, patternLength, i, queryContent)) {
							return false;
						}
						int endPtrNegation = queryContent.getClauses().size() - 1;

						while (endPtrNegation != startPtrNegation) {
							queryContent.setNegation(PATTERN, endPtrNegation);
							endPtrNegation--;
						}
					}
					else {
						if (!isValidPattern(queryArr, patternLength, i, queryContent, true)) {
							return false;
						}
					}

				}
				else {
					if (!isValidPattern(queryArr, patternLength, i, queryContent, false)) {
						return false;
					}
				}

				//Finish processing this pattern
				i += (patternLength - 1);
			}
			//with
			else if (prevSelectedClause == 3) {

				//Keep track of the length of with clause
				int withLength = 1;

				while (!withOperator.empty() && higherPrecedenceValidate(withOperator.top(), SYMBOL_PLUS)) {
					queryContent.insertOperator(WITH_CLAUSE, MAP_OPERATORS.find(withOperator.top())->second);
					withOperator.pop();
				}

				withOperator.push(SYMBOL_PLUS);

				if (queryArr.at(i + withLength).at(0) == SYMBOL_OPEN_BRACKET) {
					if (!convertToPostFix(queryArr, prevSelectedClause, withLength, i, queryContent)) {
						return false;
					}
					withLength--;
				}
				else if (i + withLength < queryArr.size() && queryArr.at(i + withLength).compare(NOT_WORD) == 0) {

					//Add "not"
					withLength++;

					if (i + withLength < queryArr.size() && queryArr.at(i + withLength).at(0) == SYMBOL_OPEN_BRACKET) {
						int startPtrNegation = queryContent.getClauses().size() - 1;
						if (!convertToPostFix(queryArr, prevSelectedClause, withLength, i, queryContent)) {
							return false;
						}
						withLength--;
						int endPtrNegation = queryContent.getClauses().size() - 1;

						while (endPtrNegation != startPtrNegation) {
							queryContent.setNegation(WITH_CLAUSE, endPtrNegation);
							endPtrNegation--;
						}
					}
					else {
						if (!isValidWithClause(queryArr, withLength, i, queryContent, true)) {
							return false;
						}
					}

				}
				else {
					if (!isValidWithClause(queryArr, withLength, i, queryContent, false)) {
						return false;
					}
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

	while (!suchThatOperator.empty()) {
		queryContent.insertOperator(CLAUSE, MAP_OPERATORS.find(suchThatOperator.top())->second);
		suchThatOperator.pop();
	}

	while (!patternOperator.empty()) {
		queryContent.insertOperator(PATTERN, MAP_OPERATORS.find(patternOperator.top())->second);
		patternOperator.pop();
	}

	while (!withOperator.empty()) {
		queryContent.insertOperator(WITH_CLAUSE, MAP_OPERATORS.find(withOperator.top())->second);
		withOperator.pop();
	}

	vqc.push_back(queryContent);
	return true;
};

bool Preprocessor::isValidSubQuery(vector<string> queryArr, int pos, int &queryLength, QueryContent &qc) {

	QueryContent subQueryContent;

	//Check for Select word
	if (pos + queryLength >= queryArr.size() || queryArr.at(pos + queryLength).compare(SELECT_WORD) != 0) {
		return false;
	}

	//Add Select word
	queryLength++;

	//Check for Synonym or Synonym.attribute
	string elem = Utils::sanitise(queryArr.at(pos + queryLength));

	if (isValidAttrRef(elem)) {
		vector<string> attrRef = Utils::split(elem, SYMBOL_FULL_STOP);

		if (!isDeclarationSynonymExist(attrRef.at(0))) {
			return false;
		}

		auto searchSynonym = declarationMap.find(attrRef.at(0));
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);

		if (!isValidAttrName(searchDeclareType->second, attrRef.at(1))) {
			return false;
		}
	}
	
}

bool Preprocessor::convertToPostFix(vector<string> queryArr, int conditionType, int &queryLength, int pos, QueryContent &qc) {
	
	stack<char> operators;
	stack<int> startPtrNegation;
	bool isOperandClause = false;
	bool isMathOperatorRepeated = false;
	bool isOperandRepeated = false;
	bool isOperatorAtTheStart = false;
	bool isOperatorAtTheEnd = false;
	bool isOperandBesideStarBracket = false;
	bool isCloseBracketBesideOperand = false;
	bool negateOperand = false;
	bool negateBracket = false;
	
	operators.push(SYMBOL_OPEN_BRACKET);
	queryLength++;

	while (!operators.empty() && pos + queryLength < queryArr.size()) {
		if (queryArr.at(pos + queryLength).compare(AND_WORD) == 0 ||
			queryArr.at(pos + queryLength).compare(OR_WORD) == 0) {

			if (isMathOperatorRepeated || isOperatorAtTheStart || isOperatorAtTheEnd) {
				return false;
			}

			char tempOp = MAP_SYMBOL_OPERATORS.find(queryArr.at(pos + queryLength))->second;
			while (!operators.empty() && higherPrecedenceValidate(operators.top(), tempOp)) {
				switch (conditionType) {
				case 1:
					qc.insertOperator(CLAUSE, MAP_OPERATORS.find(operators.top())->second);
					break;
				case 2:
					qc.insertOperator(PATTERN, MAP_OPERATORS.find(operators.top())->second);
					break;
				case 3:
					qc.insertOperator(WITH_CLAUSE, MAP_OPERATORS.find(operators.top())->second);
					break;
				}
				operators.pop();
			}

			operators.push(MAP_SYMBOL_OPERATORS.find(queryArr.at(pos + queryLength))->second);
			isMathOperatorRepeated = true;
			isOperandRepeated = false;
			isOperatorAtTheStart = false;
			isOperatorAtTheEnd = true;
			isOperandBesideStarBracket = false;
			isCloseBracketBesideOperand = false;
			negateOperand = false;
		}
		else if (queryArr.at(pos + queryLength).compare(NOT_WORD) == 0) {
			//Check for open bracket
			if (pos + queryLength + 1 < queryArr.size() && queryArr.at(pos + queryLength + 1).at(0) == SYMBOL_OPEN_BRACKET) {				
				negateBracket = true;
				switch (conditionType) {
				case 1:
					startPtrNegation.push(qc.getClauses().size() - 1);
					break;
				case 2:
					startPtrNegation.push(qc.getPattern().size() - 1);
					break;
				case 3:
					startPtrNegation.push(qc.getWithClauses().size() - 1);
					break;
				}
			}
			else {
				negateOperand = true;
			}
		}
		else if (KEYWORDS_CLAUSES.find(queryArr.at(pos + queryLength)) != KEYWORDS_CLAUSES.end() ||
				(isDeclarationSynonymExist(queryArr.at(pos + queryLength)) && 
					KEYWORDS_PATTERN_TYPE.find(declarationMap.find(queryArr.at(pos + queryLength))->second) != KEYWORDS_PATTERN_TYPE.end()) ||
				((pos + queryLength + 1) < queryArr.size() && (pos + queryLength + 2) < queryArr.size() &&
					isValidRef(Utils::sanitise(queryArr.at(pos + queryLength))) && 
					Utils::sanitise(queryArr.at(pos + queryLength + 1)).at(0) == SYMBOL_EQUALS &&
					isValidRef(Utils::sanitise(queryArr.at(pos + queryLength + 2))))) {

			if (isOperandRepeated || isCloseBracketBesideOperand) {
				return false;
			}

			switch (conditionType) {
			case 1:
				if (!isValidClause(queryArr, queryLength, pos, qc, negateOperand)) {
					return false;
				}
				queryLength--;
				break;
			case 2:
				if (!isValidPattern(queryArr, queryLength, pos, qc, negateOperand)) {
					return false;
				}
				queryLength--;
				break;
			case 3:
				if (!isValidWithClause(queryArr, queryLength, pos, qc, negateOperand)) {
					return false;
				}
				break;
			}

			isMathOperatorRepeated = false;
			isOperandRepeated = true;
			isOperatorAtTheStart = false;
			isOperatorAtTheEnd = false;
			isOperandBesideStarBracket = true;
			isCloseBracketBesideOperand = false;
			negateOperand = false;
		}
		//Checking for open bracket
		else if (queryArr.at(pos + queryLength).at(0) == SYMBOL_OPEN_BRACKET) {

			if (isOperandBesideStarBracket) {
				return false;
			}

			operators.push(SYMBOL_OPEN_BRACKET);

			isMathOperatorRepeated = false;
			isOperandRepeated = false;
			isOperatorAtTheEnd = false;
			isOperatorAtTheStart = true;
			isOperandBesideStarBracket = false;
			isCloseBracketBesideOperand = false;
			negateOperand = false;
		}
		//Checking for closing bracket
		else if (queryArr.at(pos + queryLength).at(0) == SYMBOL_CLOSE_BRACKET) {
			while (!operators.empty() && operators.top() != SYMBOL_OPEN_BRACKET) {
				switch (conditionType) {
				case 1:
					qc.insertOperator(CLAUSE, MAP_OPERATORS.find(operators.top())->second);
					break;
				case 2:
					qc.insertOperator(PATTERN, MAP_OPERATORS.find(operators.top())->second);
					break;
				case 3:
					qc.insertOperator(WITH_CLAUSE, MAP_OPERATORS.find(operators.top())->second);
					break;
				}
				operators.pop();
			}

			if (operators.empty() || isOperatorAtTheEnd) {
				return false;
			}
			else {
				operators.pop();
			}

			if (negateBracket) {
				negateBracket = false;

				int endPtr, startPtr = startPtrNegation.top();
				CLAUSE_NODE_TYPE nodeType;
				startPtrNegation.pop();

				switch (conditionType) {
				case 1: {
					endPtr = qc.getClauses().size() - 1;
					nodeType = CLAUSE;
				}
				break;
				case 2: {
					endPtr = qc.getPattern().size() - 1;
					nodeType = PATTERN;
				}
				break;
				case 3: {
					endPtr = qc.getWithClauses().size() - 1;
					nodeType = WITH_CLAUSE;
				}
				break;
				}

				while (endPtr != startPtr) {
					qc.setNegation(nodeType, endPtr);
					endPtr--;
				}
			}

			isMathOperatorRepeated = false;
			isOperandRepeated = false;
			isOperatorAtTheStart = false;
			isOperatorAtTheEnd = false;
			isOperandBesideStarBracket = false;
			isCloseBracketBesideOperand = true;
			negateOperand = false;
			negateBracket = false;
		}
		else {
			return false;
		}
		queryLength++;
	}
	return true;
}

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

bool Preprocessor::isValidElem(vector<string> queryArr, int endOfSelectStatement, QueryContent &qc) {
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
		qc.insertSelect(searchDeclareType->second, searchSynonym->first, getAttrName);
	}
	else if (isValidSynonym(elem)) {
		//check if select synonym exist in the declarationMap
		if (!isDeclarationSynonymExist(elem)) {
			return false;
		}

		//Populate the selectType of QueryObject
		auto searchSynonym = declarationMap.find(elem);
		auto searchDeclareType = KEYWORDS_DECLARATIONS.find(searchSynonym->second);
		qc.insertSelect(searchDeclareType->second, searchSynonym->first, NONE);
	}
	else {
		return false;
	}

	return true;
}

bool Preprocessor::isValidClause(vector<string> queryArr, int &clauseLength, int pos, QueryContent &qc, bool invert) {
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

	//Check for subquery LHS, first check for open bracket
	if ((pos + clauseLength) >= queryArr.size() && queryArr.at(pos + clauseLength).at(0) == SYMBOL_OPEN_BRACKET) {

	}


	//Add all the left Param
	string leftArg = retrieveParamFromQuery(queryArr, clauseLength, pos, string(1, SYMBOL_COMMA));

	//Add all the right Param
	string rightArg = retrieveParamFromQuery(queryArr, clauseLength, pos, string(1, SYMBOL_CLOSE_BRACKET));

	if (!parseClauseArg(qc, clauseValue, leftArg, rightArg, invert)) {
		return false;
	}
	return true;
}

bool Preprocessor::isValidPattern(vector<string> queryArr, int &patternLength, int pos, QueryContent &qc, bool invert) {
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

	if (!parsePattern(qc, searchDeclareType->second, searchSynonym->first, leftArg, rightArg, invert)) {
		return false;
	}
	return true;
}

bool Preprocessor::isValidWithClause(vector<string> queryArr, int &withLength, int pos, QueryContent &qo, bool invert) {
	

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

	if (!parseWithClause(qo, ref1, ref2, invert)) {
		return false;
	}
	return true;
}

bool Preprocessor::parseClauseArg(QueryContent &qc, string relType, string arg1, string arg2, bool invert) {

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
	bool LHSInvalidHigher = false;

	//Check if both synonym are the same value
	if (leftArgType == SYNONYM && rightArgType == SYNONYM) {
		sameSynonymValue = leftArg.compare(rightArg) == 0;
	} else if (leftArgType == INTEGER && rightArgType == INTEGER) {

		//Check if both integer are the same value
		sameIntegerValue = leftArg.compare(rightArg) == 0;

		//Check if LHS is higher than RHS
		LHSInvalidHigher = stoi(leftArg) > stoi(rightArg);
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
	case Parent: case ParentT: case Follows: case FollowsT: 
		if (!isValidStmtRef(leftArg) || !isValidStmtRef(rightArg) || sameSynonymValue || sameIntegerValue || LHSInvalidHigher) {
			return false;
		}
		break;
	case Next:
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

	qc.insertClause(searchRelType->second, leftArgType, leftArg,
		rightArgType, rightArg, invert);

	return true;
}

bool Preprocessor::parsePattern(QueryContent &qc, ParamType entityType, string entity, string arg1, string arg2, bool invert)
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

	qc.insertPattern(entityType, entity, leftArgType,
		leftArg, rightArgType, rightArg, invert);

	return true;
}

bool Preprocessor::parseWithClause(QueryContent &qc, string leftRef, string rightRef, bool invert) {
	
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
		qc.insertWithClause(leftArgType, leftArg, leftAttrType,
			rightArgType, rightArg, rightAttrType, invert);
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

unordered_map<int, Param *> Preprocessor::getSubQueryMapping() {
	return subQueryMapping;
}

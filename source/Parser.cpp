#pragma once
#include "../SPA/InvalidExpressionException.h"
#include "../AutoTester/source/TestWrapper.h"
#include "../SPA/Constants.h"
using namespace std;

// constructor
Parser::Parser() {
}

// tokenize content
void Parser::tokenize(string content)
{
	content = Utils::sanitise(content);
	tokens = Utils::explode(content, ParserConstants::DELIM_STRING, ParserConstants::DELIMITERS);
	iter = tokens.begin();
}

// reads and returns next token
string Parser::getToken() {
	{
		if (iter < tokens.end()) {
			nextToken = *(iter++);
		}
		else {
			nextToken.clear();
		}
		return nextToken;
	}
}

string Parser::getWord()
{
	string result = nextToken;
	nextToken = getToken();
	return result;
}

queue<string> Parser::getRPN(queue<string> expr)
{
	originalExpression = expr;
	//using Shunting-yard algorithm
	while (!originalExpression.empty()) {
		word = Utils::getWordAndPop(originalExpression);
		//parse word if it follows the mathematical rule
		if (Utils::isValidFactor(word)) {
			//factor either appears at the start of the expression, or it follows an open bracket or operator
			if (Utils::isValidConstant(word)) {
				int constant = stoi(word);
				pkb.insertToTable(CONST_TABLE, constant, { { currentStmNum } });
			}
			else if (Utils::isValidName(word)) {
				string var_name = word;
				int var_id = pkb.insertToNameTable(VAR_TABLE, var_name);

				// get parents and insert parents
				vector<int> parents = pkb.getParentStar(currentStmNum)[0];
				for (int i = 0; i < static_cast<int>(parents.size()); i++) {
					pkb.insertToTable(STATEMENT_TABLE, parents[i], { {},{ var_id },{},{} });
					pkb.insertToTable(USES_TABLE, var_id, { { parents[i] },{} });
				}

				// insert uses
				pkb.insertToTable(STATEMENT_TABLE, currentStmNum, { {},{ var_id },{},{} });
				pkb.insertToTable(PROC_INFO_TABLE, currentProcId, { {},{ var_id },{} });
				pkb.insertToTable(USES_TABLE, var_id, { { currentStmNum },{ currentProcId } });

			}
			else {
				throw InvalidExpressionException("Invalid Expression!");
			}
			parseFactor();
		}
		else if (Utils::isOpenBracket(word)) {
			//open bracket either appears at the start of the expression, or it follows an operator
			parseOpenBracket();
		}
		else if (Utils::isCloseBracket(word)) {
			//close bracket follows a valid factor
			parseCloseBracket();
		}
		else if (Utils::isValidOperator(word)) {
			//operator follows a valid factor or close bracket
			parseOperator();
		}
		/*
		else {
		throw InvalidExpressionException("Invalid Expression!");
		}*/
		previousWord = word;
	}

	//ensure that the last element in the expression is a close bracket or a factor

	if (!(Utils::isCloseBracket(previousWord) || Utils::isValidFactor(previousWord))) {
		throw InvalidExpressionException("Invalid Expression!");
	}

	//pop the remaining operators into the expression, as according to the Shunting-yard algorithm

	while (!operationStack.empty()) {
		if (!Utils::isValidOperator(operationStack.top())) {
			throw InvalidExpressionException("Invalid Expression!");
		}
		expressionQueue.push(operationStack.top());
		operationStack.pop();
	}
	return expressionQueue;
}

void Parser::parseFactor()
{
	expressionQueue.push(word);
}

void Parser::parseOperator()
{
	//while there is an operator token, o2, at the top of the operator stack and the current operator o1 has precedence less than that of o2,
	while (!operationStack.empty() && Utils::isValidOperator(operationStack.top()) && UtilsConstants::OPERATOR_PRIORITIES.at(word) <= UtilsConstants::OPERATOR_PRIORITIES.at(operationStack.top())) {
		//then pop o2 off the operator stack, onto the output queue;
		expressionQueue.push(operationStack.top());
		operationStack.pop();
	}
	//push o1 onto the operator stack.
	operationStack.push(word);
}

void Parser::parseOpenBracket()
{
	operationStack.push(word);
}

void Parser::parseCloseBracket()
{
	while (!operationStack.empty()) {
		if (Utils::isOpenBracket(operationStack.top())) {
			operationStack.pop();
			return;
		}
		else {
			expressionQueue.push(operationStack.top());
			operationStack.pop();
		}
	}
	throw InvalidExpressionException("Invalid Expression!");
}

queue<string> Parser::getExpression()
{
	queue<string> originalExpression;
	string word;
	while ( nextToken != ";") {
		word = getWord();
		if (Utils::isValidFactor(word) || Utils::isValidOperator(word) || word == "(" || word == ")") {
			originalExpression.push(word);
		}
		else {
			throw InvalidExpressionException("Invalid Expression!");
		}
	}
	if (originalExpression.empty()) {
		throw InvalidExpressionException("Invalid Expression!");
	}
	queue<string> rpn;

	try {
		rpn = getRPN(originalExpression);
	}
	catch (exception) {
		throw InvalidExpressionException("Invalid Expression!");
	}

	return rpn;
}

// check if token matches nextToken
bool Parser::match(string token, bool isVar = false) {
	if (isVar == false) {
		// expected vs real token
		if (token == nextToken) {
			nextToken = getToken();
			return true;
		}
		else {
			throw InvalidSyntaxException;
			return false;
		}
	}
	else {
		// check if name of variable is valid
		if (!Utils::isValidName(token)) {
			throw InvalidNameException;
		}
		nextToken = getToken();
		return true;
	}
}




void Parser::expression() {
	getExpression();
}

bool Parser::ifStatement() {
	int currentIfNum = currentStmNum;
	int curStmListId = stmListIdStack.top();
	// insert StmtListID to StmtTable1 
	pkb.insertToTable(STATEMENT_TABLE, currentStmNum, { { curStmListId, nextStmListId },{},{},{ IF_TYPE } });
	// insert StmtID to StmtListTable2
	pkb.insertToTable(STATEMENT_LIST_TABLE, nextStmListId, { { currentStmNum },{},{ IF_TYPE } });

	match("if");

	string var_name = nextToken;

	match(var_name, true);

	// insert variable to varTable
	int var_id = pkb.insertToNameTable(VAR_TABLE, var_name);

	// get parents and insert parents
	vector<int> parents = pkb.getParentStar(currentStmNum)[0];
	for (int i = 0; i < static_cast<int>(parents.size()); i++) {
		pkb.insertToTable(STATEMENT_TABLE, parents[i], { {},{ var_id },{},{} });
		pkb.insertToTable(USES_TABLE, var_id, { { parents[i] },{ } });
	}

	// insert uses to tables 1,3,4
	pkb.insertToTable(STATEMENT_TABLE, currentStmNum, { {},{ var_id },{},{} });
	pkb.insertToTable(PROC_INFO_TABLE, currentProcId, { {},{ var_id },{} });
	pkb.insertToTable(USES_TABLE, var_id, { { currentStmNum },{ currentProcId } });
	match("then");
	match("{");
	statementList();
	match("}");
	// else
	// update ElseStatementListID for IfStatement
	pkb.insertToTable(STATEMENT_TABLE, currentIfNum, { { nextStmListId },{},{},{} });
	// insert StmtID to StmtListTable2 - new StmtList
	pkb.insertToTable(STATEMENT_LIST_TABLE, nextStmListId, { { currentIfNum },{},{ IF_TYPE } });
	match("else");
	match("{");
	statementList();
	match("}");
	return true;
}

bool Parser::whileStatement() {
	int curStmListId = stmListIdStack.top();
	// insert StmtListID to StmtTable1 
	pkb.insertToTable(STATEMENT_TABLE, currentStmNum, { { curStmListId, nextStmListId },{},{},{ WHILE_TYPE } });
	// insert StmtID to StmtListTable2
	pkb.insertToTable(STATEMENT_LIST_TABLE, nextStmListId, { { currentStmNum },{},{ WHILE_TYPE } });

	match("while");
	string var_name = nextToken;

	match(var_name, true);

	// insert variable to var table
	int var_id = pkb.insertToNameTable(VAR_TABLE, var_name);

	// get parents and insert parents
	vector<int> parents = pkb.getParentStar(currentStmNum)[0];
	for (int i = 0; i < static_cast<int>(parents.size()); i++) {
		pkb.insertToTable(STATEMENT_TABLE, parents[i], { {},{ var_id },{},{} });
		pkb.insertToTable(USES_TABLE, var_id, { { parents[i] },{} });
	}

	// insert uses to table 1,3,4
	pkb.insertToTable(STATEMENT_TABLE, currentStmNum, { {},{ var_id },{},{} });
	pkb.insertToTable(PROC_INFO_TABLE, currentProcId, { {},{ var_id },{} });
	pkb.insertToTable(USES_TABLE, var_id, { { currentStmNum },{ currentProcId } });
	match("{");
	statementList();
	match("}");
	return true;
}

bool Parser::assignmentStatement() {
	int curStmListId = stmListIdStack.top();
	// insert StmtListID to StmtTable1 
	pkb.insertToTable(STATEMENT_TABLE, currentStmNum, { { curStmListId },{},{},{ ASSIGNMENT_TYPE } });
	string var_name = nextToken;

	match(var_name, true);

	// insert variable
	int var_id = pkb.insertToNameTable(VAR_TABLE, var_name);

	// get parents and insert parents
	vector<int> parents = pkb.getParentStar(currentStmNum)[0];
	for (int i = 0; i < static_cast<int>(parents.size()); i++) {
		pkb.insertToTable(STATEMENT_TABLE, parents[i], { {},{},{ var_id },{} });
		pkb.insertToTable(MODIFIES_TABLE, var_id, { { parents[i] },{} });
	}

	// insert modifies to table 1,3,5
	pkb.insertToTable(STATEMENT_TABLE, currentStmNum, { {},{},{ var_id },{} });
	pkb.insertToTable(PROC_INFO_TABLE, currentProcId, { {},{},{ var_id } });
	pkb.insertToTable(MODIFIES_TABLE, var_id, { { currentStmNum },{ currentProcId } });
	
	match("=");
	expression();
	match(";");
	return true;
}

bool Parser::statement() {
	string var_name;
	int var_id;
	// increate Statement number
	currentStmNum++;
	// need to top stmListIdStack to get curStmListId
	int curStmListId = stmListIdStack.top();

	// add cur stm to cur stm list 
	bool a = pkb.insertToTable(STATEMENT_LIST_TABLE, curStmListId, { {},{ currentStmNum },{} });

	if (nextToken == "if") {
		ifStatement();
	}
	else if (nextToken == "while") {
		whileStatement();
	}
	else {
		assignmentStatement();
	}
	return true;
}


bool Parser::statementList() {
	bool first = true;
	// only push to StatementListStack 1 time
	if (first) {
		stmListIdStack.push(nextStmListId);
		nextStmListId++;
		first = false;
	}

	statement();

	while (nextToken != "}") {
		statement();
	}

	stmListIdStack.pop();

	return true;
}

bool Parser::procedure() {
	match("procedure");
	string procName = nextToken;

	match(procName, true);

	// insert proc id to PROC_INFO_TABLE_3
	currentProcId = pkb.insertToNameTable(PROC_TABLE, procName);

	match("{");
	// insert proc id to STATEMENT_LIST_TABLE_2
	pkb.insertToTable(STATEMENT_LIST_TABLE, nextStmListId, { { PROCEDURE_PARENT_ID },{},{ PROCEDURE_PARENT_ID } });
	// insert statement list id to PROC_INFO_TABLE_3
	pkb.insertToTable(PROC_INFO_TABLE, currentProcId, { { nextStmListId },{},{} });
	statementList();
	match("}");
	return true;
}


bool Parser::program() {
	procedure();
	if (nextToken == "procedure") {
		procedure();
	}
	else if (nextToken == "") {
		// program is finished
	}
	else {
		// wrong syntax - expect "procedure" but token is some other word
		throw InvalidSyntaxException;
	}
	return true;
}


PKB Parser::Parse(string fileName, PKB passedPKB, bool isStringInput, string stringInput)
{
	pkb = passedPKB;
	if (isStringInput == true) {
		stringstream stringInputStream(stringInput);
		simpleStringStream << stringInputStream.rdbuf();
	}
	else {
		// input is a file
		ifstream file(fileName);
		if (file)
		{
			simpleStringStream << file.rdbuf();
			file.close();
		}
		else {
			cout << "file not found" << endl;
		}
	}

	try {
		tokenize(simpleStringStream.str());
		nextToken = getToken();
		program();
	}
	catch (exception& e) {
		cout << "Exception caught" << endl;
		cout << e.what() << endl;
		exit(1);
	} 
/*
	catch (InvalidExpressionException& e) {
		cout << "InvalidExpressionException caught" << endl;
		cout << e.what() << endl;
		exit(2);
	}
*/
	cout << "success!";

	return pkb;
}
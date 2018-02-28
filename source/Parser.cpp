#pragma once
//#include "../SPA/Parser.h"
#include "../SPA/InvalidExpressionException.h"
//#include "../SPA/Utils.h"
//#include "PKB.h"
#include "../AutoTester/source/TestWrapper.h"


Parser::Parser() {
	//cout << "Object is being created" << endl;
}

void Parser::tokenize(string content)
{
	content = Utils::sanitise(content);
	tokens = Utils::explode(content, ParserConstants::DELIM_STRING, ParserConstants::DELIMITERS);
	iter = tokens.begin();
}

string Parser::getToken() {
	{
		if (iter < tokens.end()) {
			nextToken = *(iter++);
		}
		else {
			nextToken.clear();
		}
		//std::cout << "Token: " << nextToken;
		return nextToken;
	}
}

queue<string> Parser::getRPN(queue<string> expr)
{
	//cout << "-------- BEGIN RPN -----------\n";
	originalExpression = expr;
	//using Shunting-yard algorithm
	while (!originalExpression.empty()) {
		word = Utils::getWordAndPop(originalExpression);
		//parse word if it follows the mathematical rule
		if (Utils::isValidFactor(word)) {
			//factor either appears at the start of the expression, or it follows an open bracket or operator
			//cout << "var: " << word;
			if (Utils::isValidConstant(word)) {
				cout << "word" << word;
				int constant = stoi(word);
				pkb.insertToTable(ParserConstants::CONST_TABLE_7, constant, { { currentStmNum } });
			}
			else if (Utils::isValidName(word)) {
				string var_name = word;
				int var_id = pkb.insertToNameTable(ParserConstants::VAR_TABLE_9, var_name);

				// get parents and insert parents
				std::vector<int> parents = pkb.getParentStar(currentStmNum);
				for (int i = 0; i < static_cast<int>(parents.size()); i++) {
					pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, parents[i], { {},{ var_id },{},{} });
					pkb.insertToTable(ParserConstants::USES_TABLE_4, var_id, { { parents[i] },{} });
				}

				// insert uses
				pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { {},{ var_id },{},{} });
				pkb.insertToTable(ParserConstants::PROC_INFO_TABLE_3, currentProcId, { {},{ var_id },{} });
				pkb.insertToTable(ParserConstants::USES_TABLE_4, var_id, { { currentStmNum },{ currentProcId } });

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
	//cout << "--------END RPN------\n";
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
		//cout << "---- CURRENT WORD: " << word << "-------\n";
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

bool Parser::match(string token, bool isVar = false) {
	cout << "[ " << token << " == " << nextToken << " ]" << endl;
	if (isVar == false) {
		// expected vs real
		if (token == nextToken) {
			nextToken = getToken();
			return true;
		}
		else {
			cout << "Diff: " << token << " != " << nextToken << "\n";
			throw InvalidSyntaxException;
			return false;
		}
	}
	else {
		nextToken = getToken();
		return true;
	}

}

string Parser::getWord()
{
	string result = nextToken;
	nextToken = getToken();
	//cout << "RESULT: " << result;
	return result;
}


void Parser::expression() {
	getExpression();
}

bool Parser::ifStatement() {
	int currentIfNum = currentStmNum;
	int curStmListId = stmListIdStack.top();
	// insert StmtListID to StmtTable1 
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { { curStmListId, nextStmListId },{},{},{ ParserConstants::IF_TYPE } });
	// insert StmtID to StmtListTable2
	pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, nextStmListId, { { currentStmNum },{},{ ParserConstants::IF_TYPE } });

	match("if");
	// insert variable to varTable
	string var_name = nextToken;
	if (!Utils::isValidName(var_name)) {
		throw InvalidNameException;
	}
	int var_id = pkb.insertToNameTable(ParserConstants::VAR_TABLE_9, var_name);

	// get parents and insert parents
	std::vector<int> parents = pkb.getParentStar(currentStmNum);
	for (int i = 0; i < static_cast<int>(parents.size()); i++) {
		pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, parents[i], { {},{ var_id },{},{} });
		pkb.insertToTable(ParserConstants::USES_TABLE_4, var_id, { { parents[i] },{ } });
	}

	// insert uses to table 1,3,4
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { {},{ var_id },{},{} });
	pkb.insertToTable(ParserConstants::PROC_INFO_TABLE_3, currentProcId, { {},{ var_id },{} });
	pkb.insertToTable(ParserConstants::USES_TABLE_4, var_id, { { currentStmNum },{ currentProcId } });
	match("", true);
	match("then");
	match("{");
	statementList();
	match("}");
	// else
	// insert StmtID to StmtListTable2 - new StmtList
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentIfNum, { { nextStmListId },{},{},{} });
	pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, nextStmListId, { { currentIfNum },{},{ ParserConstants::IF_TYPE } });
	match("else");
	match("{");
	statementList();
	match("}");
	return true;
}

bool Parser::whileStatement() {
	int curStmListId = stmListIdStack.top();
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { { curStmListId, nextStmListId },{},{},{ ParserConstants::WHILE_TYPE } });
	pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, nextStmListId, { { currentStmNum },{},{ ParserConstants::WHILE_TYPE } });

	match("while");
	// insert variable
	string var_name = nextToken;
	if (!Utils::isValidName(var_name)) {
		throw InvalidNameException;
	}
	int var_id = pkb.insertToNameTable(ParserConstants::VAR_TABLE_9, var_name);

	// get parents and insert parents
	std::vector<int> parents = pkb.getParentStar(currentStmNum);
	for (int i = 0; i < static_cast<int>(parents.size()); i++) {
		pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, parents[i], { {},{ var_id },{},{} });
		pkb.insertToTable(ParserConstants::USES_TABLE_4, var_id, { { parents[i] },{} });
	}

	// insert uses to table 1,3,4
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { {},{ var_id },{},{} });
	pkb.insertToTable(ParserConstants::PROC_INFO_TABLE_3, currentProcId, { {},{ var_id },{} });
	pkb.insertToTable(ParserConstants::USES_TABLE_4, var_id, { { currentStmNum },{ currentProcId } });
	match("", true);
	match("{");
	statementList();
	match("}");
	return true;
}

bool Parser::assignStatement() {
	int curStmListId = stmListIdStack.top();
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { { curStmListId },{},{},{ ParserConstants::ASSIGNMENT_TYPE } });
	// insert variable
	string var_name = nextToken;
	if (!Utils::isValidName(var_name)) {
		throw InvalidNameException;
	}
	int var_id = pkb.insertToNameTable(ParserConstants::VAR_TABLE_9, var_name);

	// get parents and insert parents
	std::vector<int> parents = pkb.getParentStar(currentStmNum);
	for (int i = 0; i < static_cast<int>(parents.size()); i++) {
		pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, parents[i], { {},{},{ var_id },{} });
		pkb.insertToTable(ParserConstants::MODIFIES_TABLE_5, var_id, { { parents[i] },{} });
	}

	// insert modifies to table 1,3,5
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { {},{},{ var_id },{} });
	pkb.insertToTable(ParserConstants::PROC_INFO_TABLE_3, currentProcId, { {},{},{ var_id } });
	pkb.insertToTable(ParserConstants::MODIFIES_TABLE_5, var_id, { { currentStmNum },{ currentProcId } });
	match("", true);
	match("=");
	expression();
	//cout << "\nEND OF EXPRESSION";
	//match("", true);
	match(";");
	return true;
}

bool Parser::statement() {
	string var_name;
	int var_id;
	// increate Statement number
	currentStmNum++;
	// need to top
	int curStmListId = stmListIdStack.top();

	// *** add cur stm to cur stm list ***
	bool a = pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, curStmListId, { {},{ currentStmNum },{} });

	if (nextToken == "if") {
		ifStatement();
	}
	else if (nextToken == "while") {
		whileStatement();
	}
	else {
		assignStatement();
	}
	return true;
}


bool Parser::statementList() {
	bool first = true;
	if (first) {
		//cout << "NEW List: " << nextStmListId << ". ";
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
	if (Utils::isValidName(procName)) {
		currentProcId = pkb.insertToNameTable(ParserConstants::PROC_TABLE_8, procName);
	}
	else {
		throw InvalidNameException;
	}
	match("", true);
	match("{");
	pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, nextStmListId, { { ParserConstants::PROCEDURE_PARENT_ID },{},{ ParserConstants::PROCEDURE_PARENT_ID } });
	pkb.insertToTable(ParserConstants::PROC_INFO_TABLE_3, currentProcId, { { nextStmListId },{},{} });
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
		// done
	}
	else {
		// wrong syntax
		cout << "expect 'procedure'" << "but is: " << nextToken << endl;
		throw InvalidSyntaxException;
	}
	return true;
}


PKB Parser::Parse(string fileName, PKB passedPKB, bool isString, string stringInput)
{
	pkb = passedPKB;
	if (isString == true) {
		stringstream stringInputStream(stringInput);
		simpleStringStream << stringInputStream.rdbuf();
	}
	else {
		std::ifstream file(fileName);
		if (file)
		{
			//cout << file.rdbuf();
			simpleStringStream << file.rdbuf();
			file.close();
		}
		else {
			cout << "file not found";
		}
	}

	try {
		tokenize(simpleStringStream.str());
		nextToken = getToken();
		program();
	}
	catch (exception& e) {
		std::cout << "MyException caught" << std::endl;
		std::cout << e.what() << std::endl;
		exit(1);
	} catch (InvalidExpressionException& e) {
		std::cout << "InvalidExpressionException caught" << std::endl;
		std::cout << e.what() << std::endl;
		exit(2);
	}
	
	std::cout << "success!";

	return pkb;
}

void printTable(unordered_map<int, std::vector<std::vector<int>>> table) {
	for (const auto& element : table) {
		std::cout << "Key:[" << element.first;

		// Iterate and print values of vector
		cout << "], ";
		std::vector<std::vector<int>> VinV = element.second;
		for (int i = 0; i< VinV.size(); i++) {
			for (int p = 0; p < VinV[i].size(); p++) {
				cout << VinV[i][p] << " ";
			}
			cout << ";";
		}
		cout << "]\n";
	}
}

void printNameTable(unordered_map<int, std::string> table) {
	for (const auto& element : table) {
		std::cout << "[ " << element.first << " , " << element.second << " ]\n";
	}
}

/*
int main() {

Parser parser;

PKB pkb;
string testString = "procedure a { x = x + 1; if x then { if x then { x = x + 1; } else { x = x + 1; } while x { x = x + 1; } } else { x = x + 1; } while x { while x { x = x + 1; } if x then { x = x + 1; } else { x = x + 1;} x = x + 1; } }";
testString = "procedure a {a = b; while c { b = a; d = 7; } if a then { while e { c = 4; } } else { d = 1; } e = 1; } procedure b{ a = b; while a { while b { a = 1; }} }";
pkb = parser.Parse("hell.txt", pkb, true, testString);

std::vector<int> data = pkb.getAllStatementsWithType(2);
for (int i = 0; i < data.size(); i++) {
	cout << i << " " << data[i] << endl;
}

cout << "*** TABLE 1 - StmtTable ***\n";
unordered_map<int, std::vector<std::vector<int>>> table1 = pkb.tables[0];
printTable(table1);

cout << "*** TABLE 2 - StmtListInfoTable ***\n";
unordered_map<int, std::vector<std::vector<int>>> table2 = pkb.tables[1];
printTable(table2);

cout << "*** TABLE 3 - ProcInfoTable ***\n";
unordered_map<int, std::vector<std::vector<int>>> table3 = pkb.tables[2];
printTable(table3);

cout << "*** TABLE 4 - Uses ***\n";
unordered_map<int, std::vector<std::vector<int>>> table4 = pkb.tables[3];
printTable(table4);

cout << "*** TABLE 5 - Modifies ***\n";
unordered_map<int, std::vector<std::vector<int>>> table5 = pkb.tables[4];
printTable(table5);

cout << "*** TABLE 7 - Constants ***\n";
unordered_map<int, std::vector<std::vector<int>>> table7 = pkb.tables[6];
printTable(table7);

cout << "*** TABLE 8 - ProcTable ***\n";
unordered_map<int, std::string> table8 = pkb.nameTables[0];
printNameTable(table8);

cout << "*** TABLE 9 - VarTable ***\n";
unordered_map<int, std::string> table9 = pkb.nameTables[1];
printNameTable(table9);

return 0;
}
*/
#pragma once
#include "../SPA/Parser.h"
#include "../SPA/Utils.h"
#include "PKB.h"



Parser::Parser() {
	//cout << "Object is being created" << endl;
}

void Parser::tokenize(string content)
{
	content = Utils::sanitise(content);
	tokens = Utils::explode(content, ParserConstants::DELIM_STRING, ParserConstants::DELIMITERS);
	iter = tokens.begin();
}

string Parser::getToken(void) {
	{
		if (iter < tokens.end()) {
			nextToken = *(iter++);
		}
		else {
			nextToken.clear();
		}
		std::cout << nextToken;
		return nextToken;
	}
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
			throw MyException();
			return false;
		}
	}
	else {
		nextToken = getToken();
		return true;
	}

}

void Parser::expression() {

}

void Parser::ifStatement() {
	int currentIfNum = currentStmNum;
	int curStmListId = stmListIdStack.top();
	// insert StmtListID to StmtTable1 
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { { curStmListId },{},{},{ ParserConstants::IF_TYPE } });
	// insert StmtID to StmtListTable2
	pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, nextStmListId, { { currentStmNum },{},{ ParserConstants::IF_TYPE } });

	match("if");
	// insert variable to varTable
	string var_name = nextToken;
	int var_id = pkb.insertToNameTable(ParserConstants::VAR_TABLE_9, var_name);
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
	pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, nextStmListId, { { currentIfNum },{},{ ParserConstants::IF_TYPE } });
	match("else");
	match("{");
	statementList();
	match("}");
}

void Parser::whileStatement() {
	int curStmListId = stmListIdStack.top();
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { { curStmListId },{},{},{ ParserConstants::WHILE_TYPE } });
	pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, nextStmListId, { { currentStmNum },{},{ ParserConstants::WHILE_TYPE } });

	match("while");
	// insert variable
	string var_name = nextToken;
	int var_id = pkb.insertToNameTable(ParserConstants::VAR_TABLE_9, var_name);
	// insert uses to table 1,3,4
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { {},{ var_id },{},{} });
	pkb.insertToTable(ParserConstants::PROC_INFO_TABLE_3, currentProcId, { {},{ var_id },{} });
	pkb.insertToTable(ParserConstants::USES_TABLE_4, var_id, { { currentStmNum },{ currentProcId } });
	match("", true);
	match("{");
	statementList();
	match("}");
}

void Parser::assignStatement() {
	int curStmListId = stmListIdStack.top();
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { { curStmListId },{},{},{ ParserConstants::ASSIGNMENT_TYPE } });
	// insert variable
	string var_name = nextToken;
	int var_id = pkb.insertToNameTable(ParserConstants::VAR_TABLE_9, var_name);
	// insert modifies to table 1,3,5
	pkb.insertToTable(ParserConstants::STATEMENT_TABLE_1, currentStmNum, { {},{},{ var_id },{} });
	pkb.insertToTable(ParserConstants::PROC_INFO_TABLE_3, currentProcId, { {},{},{ var_id } });
	pkb.insertToTable(ParserConstants::MODIFIES_TABLE_5, var_id, { { currentStmNum },{ currentProcId } });
	match("", true);
	match("=");
	//expression();
	match("", true);
	match(";");
}

void Parser::statement() {
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

}


void Parser::statementList() {
	bool first = true;
	if (first) {
		cout << "NEW List: " << nextStmListId << ". ";
		stmListIdStack.push(nextStmListId);
		nextStmListId++;
		first = false;
	}

	statement();

	while (nextToken != "}") {
		statement();
	}
	stmListIdStack.pop();
}

void Parser::procedure() {
	match("procedure");
	string procName = nextToken;
	currentProcId = pkb.insertToNameTable(ParserConstants::PROC_TABLE_8, procName);
	match("", true);
	match("{");
	pkb.insertToTable(ParserConstants::STATEMENT_LIST_TABLE_2, nextStmListId, { { ParserConstants::PROCEDURE_PARENT_ID },{},{ ParserConstants::PROCEDURE_PARENT_ID } });
	pkb.insertToTable(ParserConstants::PROC_INFO_TABLE_3, currentProcId, { { nextStmListId },{},{} });
	statementList();
	match("}");
}


void Parser::program() {
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
		throw MyException();
	}
}



PKB Parser::Parse(string fileName, PKB passedPKB, bool isString, string stringInput)
{
	//int i = 1, currentStmNo = 0 , StmListIndex = 0, currentParent = 0, currentIf = 0, nestingLevel = 0;

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
	catch (MyException& e) {
		std::cout << "MyException caught" << std::endl;
		std::cout << e.what() << std::endl;
		//return EXIT_FAILURE;
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
	pkb = parser.Parse("subset_if_while_diff_nospace.txt", pkb);

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

	cout << "*** TABLE 8 - ProcTable ***\n";
	unordered_map<int, std::string> table8 = pkb.nameTables[0];
	printNameTable(table8);

	cout << "*** TABLE 9 - VarTable ***\n";
	unordered_map<int, std::string> table9 = pkb.nameTables[1];
	printNameTable(table9);

	return 0;
}
*/
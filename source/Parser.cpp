#pragma once
#include "Parser.h"
#include "Utils.h"

int Parse() {
	//
	return 0;
}

Parser::Parser() {
	test = "van";
	//cout << "Object is being created" << endl;
}

string Parser::getTest() {
	return test;
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

void Parser::statement() {
	// *** add cur stm to cur stm list ***
	// need to top
	int curStmListId = stmListIdStack.top();
	cout << "curStmListId: " << curStmListId << ". ";
	cout << "curNum: " << currentStmNum << ". ";

	// Iterate and print values of vector
	cout << "vectorBefore: ";
	for (int m : stmIdMap[curStmListId]) {
		std::cout << m << " ";
	}
	cout << "]\n";

	stmIdMap[curStmListId].push_back(currentStmNum);

	cout << "vectorAfter: ";
	for (int m : stmIdMap[curStmListId]) {
		std::cout << m << " ";
	}
	cout << "]\n";


	if (nextToken == "if") {
		int currentIfNum = currentStmNum;
		stmIdMap.insert({ nextStmListId,{ currentStmNum } });
		currentStmNum++;

		match("if");
		match("", true);
		match("then");
		match("{");
		statementList();
		match("}");
		// else
		stmIdMap.insert({ nextStmListId,{ currentIfNum } });
		match("else");
		match("{");
		statementList();
		match("}");
	}
	else if (nextToken == "while") {
		stmIdMap.insert({ nextStmListId,{ currentStmNum } });
		currentStmNum++;

		match("while");
		match("", true);
		match("{");
		statementList();
		match("}");
	}
	else {
		match("", true);
		match("=");
		//expression();
		match("", true);
		match(";");
		currentStmNum++;

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

	/*
	if (nextToken == "}") {
	stmListIdStack.pop();
	}
	else {
	statementList();
	}
	*/
	while (nextToken != "}") {
		statement();
	}
	stmListIdStack.pop();
}

void Parser::procedure() {
	match("procedure");
	match("", true);
	match("{");
	stmIdMap.insert({ nextStmListId,{ -1 } });
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




// return 0: success, 1: wrong syntax, 2: file not found
int Parser::parse(string fileName, bool isString = false, string stringInput = "")
{
	//int i = 1, currentStmNo = 0 , StmListIndex = 0, currentParent = 0, currentIf = 0, nestingLevel = 0;


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
			return 2;
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
		return EXIT_FAILURE;
	}
	std::cout << "success!";
	for (const auto& n : stmIdMap) {
		std::cout << "Key:[" << n.first << "] Value:[";
		// Iterate and print values of vector
		for (int m : n.second) {
			std::cout << m << " ";
		}
		cout << "]\n";
	}
	return 0;
}

/*
int main() {

Parser parser;

int c = parser.parse("subset_if_while_diff.txt");
std::cout << endl << "result: " << c << endl;

return 0;
}
*/
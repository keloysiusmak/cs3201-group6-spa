#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "PKB.h"
#include <fstream>
#include <sstream>
#include <stack>
#include <deque>
#include <unordered_map>
using namespace std;


class Parser {
public:   PKB Parse(string simpleSource, PKB, bool isString = false, string stringInput = "");
		  Parser();
		  void tokenize(string content);

private:	 PKB pkb;
			 string nextToken;
			 stringstream simpleStringStream;
			 string getToken();
			 vector<string> tokens;
			 vector<string>::iterator iter;
			 int currentStmNum = 0;
			 stack<int> stmListIdStack;
			 int nextStmListId = 1;
			 int currentProcId = 0;
			 unordered_map<int, vector<int>> stmIdMap;


			 bool match(string, bool);
			 void expression();
			 void statement();
			 void statementList();
			 void procedure();
			 void program();
			 string test;
			 struct MyException : public exception {
				 const char * what() const throw () {
					 return "Syntax is wrong!";
				 }
			 };
};

namespace ParserConstants {
	const string DELIM_STRING = " ;={}+*-() ";
	const vector<char> DELIMITERS{ ';', '=', '{', '}', '+', '*', '-', '(', ')' };
	const int STATEMENT_TABLE_1 = 1;
	const int STATEMENT_LIST_TABLE_2 = 2;
	const int PROC_INFO_TABLE_3 = 3;
	const int USES_TABLE_4 = 4;
	const int MODIFIES_TABLE_5 = 5;
	const int CONST_TABLE_7 = 7;
	const int PROC_TABLE_8 = 8;
	const int VAR_TABLE_9 = 9;

	const int ASSIGNMENT_TYPE = 1;
	const int WHILE_TYPE = 2;
	const int IF_TYPE = 3;

	const int PROCEDURE_PARENT_ID = 0;
}
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
		  string getTest();
		  void tokenize(string);

private:	 PKB pkb;
			 string nextToken;
			 stringstream simpleStringStream;
			 string getToken();
			 vector<string> tokens;
			 vector<string>::iterator iter;
			 int currentStmNum = 1;
			 stack<int> stmListIdStack;
			 int nextStmListId = 1;
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
	const int CONTAINER_TABLE = 2;
	const int PROCEDURE_PARENT_ID = 0;
}

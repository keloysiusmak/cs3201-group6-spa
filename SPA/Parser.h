#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <deque>
#include <unordered_map>
#include "PKB.h"
#include "../SPA/Utils.h"
using namespace std;


class Parser {
public:   PKB Parse(string simpleSource, PKB, bool isString = false, string stringInput = "");
		  Parser();
		  void tokenize(string content);
		  PKB pkb;
		  string nextToken;
		  stringstream simpleStringStream;
		  string getToken();
		  queue<string> getRPN(queue<string>);
		  queue<string> originalExpression;
		  string word;
		  string previousWord;
		  void parseFactor();
		  void parseOperator();
		  void parseOpenBracket();
		  void parseCloseBracket();
		  queue<string> getExpression();
		  stack<string> operationStack;
		  queue<string> expressionQueue;
		  vector<string> tokens;
		  vector<string>::iterator iter;

		  int currentStmNum = 0;
		  stack<int> stmListIdStack;
		  int nextStmListId = 1;
		  int currentProcId = 0;

		  bool match(string, bool);
		  string getWord();
		  void expression();
		  bool ifStatement();
		  bool whileStatement();
		  bool assignmentStatement();
		  bool statement();
		  bool statementList();
		  bool procedure();
		  bool program();
		  string test;
		  struct MySyntaxException : public exception {
			  const char * what() const throw () {
				  return "Syntax is invalid!";
			  }
		  } InvalidSyntaxException;
		  struct MyNameException : public exception {
			  const char * what() const throw () {
				  return "Name is invalid!";
			  }
		  } InvalidNameException;
};

namespace ParserConstants {
	const string DELIM_STRING = " ;={}+*-() ";
	const vector<char> DELIMITERS{ ';', '=', '{', '}', '+', '*', '-', '(', ')' };
}
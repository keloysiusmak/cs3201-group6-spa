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
#include "Utils.h"
using namespace std;


class Parser {
public:   
		PKB Parse(string simpleSource, PKB, bool isString = false, string stringInput = "");
		Parser();		  
	/*  Parser attributes  */
		PKB pkb;
		// tokens
		stringstream simpleStringStream;
		vector<string> tokens;
		vector<string>::iterator iter;
		string nextToken;
		// IDs
		int currentStmNum = 0;
		stack<int> stmListIdStack;
		int nextStmListId = 1;
		int currentProcId = 0;
		// expression
		queue<string> originalExpression;
		stack<string> operationStack;
		queue<string> expressionQueue;
		string word;
		string previousWord;

	/*  Operations  */
		// tokenize
		void tokenize(string content);	
		string getToken();
		// expression parsing
		void parseFactor();
		void parseOperator();
		void parseOpenBracket();
		void parseCloseBracket();		
		string getWord();
		queue<string> getRPN(queue<string>);
		queue<string> getExpression();
		void expression();
		string getExpressionQueueString();
		// parsing of SIMPLE code structure
		bool match(string, bool);
		bool ifStatement();
		bool whileStatement();
		bool callStatement();
		bool assignmentStatement();
		bool statement();	
		bool statementList();
		bool procedure();
		bool program();
		// exceptions
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
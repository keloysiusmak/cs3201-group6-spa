#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include "../SPA/PatternObject.h"
#include "../SPA/Parser.h"
#include "../SPA/Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ParserTest
{
Parser parser;

string getQueueString(queue<string> expression) {
	std::stringstream ss;
	while (expression.size() != 0) {
		ss << expression.front();
		expression.pop();
	}
	return ss.str();
}

void initializeParserStatementList() {
	parser.nextToken = parser.getToken();
	parser.pkb.insertToTable(STATEMENT_LIST_TABLE, parser.nextStmListId, { { PROCEDURE_PARENT_ID },{},{ parser.currentProcId } });
	parser.stmListIdStack.push(parser.nextStmListId);
	parser.nextStmListId++;
}

void initializeParserProcedure() {
	parser.nextToken = parser.getToken();
	parser.match("{", false);
	parser.pkb.insertToTable(STATEMENT_LIST_TABLE, parser.nextStmListId, { { PROCEDURE_PARENT_ID },{},{ parser.currentProcId } });
}

void initializeParser() {
	parser.nextToken = parser.getToken();
}

TEST_CLASS(ParserTest)
{
public:

	TEST_METHOD(ParserTokenize) {
		parser = Parser();
		parser.tokenize("procedure Main { \
						if x then{ \
						c = d*(2-e);} \
						else {call Second;} }");
		Assert::AreEqual(string("procedure"), *(parser.iter++));
		Assert::AreEqual(string("Main"), *(parser.iter++));
		Assert::AreEqual(string("{"), *(parser.iter++));
		Assert::AreEqual(string("if"), *(parser.iter++));
		Assert::AreEqual(string("x"), *(parser.iter++));
		Assert::AreEqual(string("then"), *(parser.iter++));
		Assert::AreEqual(string("{"), *(parser.iter++));
		Assert::AreEqual(string("c"), *(parser.iter++));
		Assert::AreEqual(string("="), *(parser.iter++));
		Assert::AreEqual(string("d"), *(parser.iter++));
		Assert::AreEqual(string("*"), *(parser.iter++));
		Assert::AreEqual(string("("), *(parser.iter++));
		Assert::AreEqual(string("2"), *(parser.iter++));
		Assert::AreEqual(string("-"), *(parser.iter++));
		Assert::AreEqual(string("e"), *(parser.iter++));
		Assert::AreEqual(string(")"), *(parser.iter++));
		Assert::AreEqual(string(";"), *(parser.iter++));
		Assert::AreEqual(string("}"), *(parser.iter++));
		Assert::AreEqual(string("else"), *(parser.iter++));
		Assert::AreEqual(string("{"), *(parser.iter++));
		Assert::AreEqual(string("call"), *(parser.iter++));
		Assert::AreEqual(string("Second"), *(parser.iter++));
		Assert::AreEqual(string(";"), *(parser.iter++));
		Assert::AreEqual(string("}"), *(parser.iter++));
		Assert::AreEqual(string("}"), *(parser.iter++));
	}

	TEST_METHOD(ParserGetToken) {
		parser = Parser();
		parser.tokenize("procedure Main { b=c; }");
		Assert::AreEqual(string("procedure"), parser.getToken());
		Assert::AreEqual(string("Main"), parser.getToken());
		Assert::AreEqual(string("{"), parser.getToken());
		Assert::AreEqual(string("b"), parser.getToken());
		Assert::AreEqual(string("="), parser.getToken());
		Assert::AreEqual(string("c"), parser.getToken());
		Assert::AreEqual(string(";"), parser.getToken());
		Assert::AreEqual(string("}"), parser.getToken());
	}



	TEST_METHOD(ParserParseFactor)
	{
		parser = Parser();
		parser.word = "abc";
		parser.parseFactor();
		string expectedString = "abc|";
		Assert::AreEqual(expectedString, getQueueString(parser.expressionQueue));
	};

	TEST_METHOD(ParserParseOperator)
	{
		parser = Parser();
		parser.word = "+";
		Assert::IsTrue(parser.operationStack.empty());
		parser.parseOperator();
		string expectedString = "+";
		Assert::AreEqual(expectedString, parser.operationStack.top());
	};

	TEST_METHOD(ParserParseOperator2)
	{
		parser = Parser();
		Assert::IsTrue(parser.operationStack.empty());
		parser.word = "+";
		parser.parseOperator();
		string expectedString = "+";
		Assert::AreEqual(expectedString, parser.operationStack.top());

		parser.word = "*";
		parser.parseOperator();
		expectedString = "*";
		Assert::AreEqual(expectedString, parser.operationStack.top());
		std::stack<std::string>::size_type s = 2;
		Assert::AreEqual(s, parser.operationStack.size());
	};

	TEST_METHOD(ParserParseOperator3)
	{
		parser = Parser();
		Assert::IsTrue(parser.operationStack.empty());
		parser.word = "*";
		parser.parseOperator();
		string expectedString = "*";
		Assert::AreEqual(expectedString, parser.operationStack.top());

		parser.word = "+";
		parser.parseOperator();
		expectedString = "+";
		Assert::AreEqual(expectedString, parser.operationStack.top());
		std::stack<std::string>::size_type s2 = 1;
		Assert::AreEqual(s2, parser.operationStack.size());

	};

	TEST_METHOD(ParserParseOperator4)
	{
		parser = Parser();
		Assert::IsTrue(parser.operationStack.empty());
		parser.word = "+";
		parser.parseOperator();
		string expectedString = "+";
		Assert::AreEqual(expectedString, parser.operationStack.top());

		parser.word = "*";
		parser.parseOperator();
		expectedString = "*";
		Assert::AreEqual(expectedString, parser.operationStack.top());
		std::stack<std::string>::size_type s = 2;
		Assert::AreEqual(s, parser.operationStack.size());

		parser.word = "*";
		parser.parseOperator();
		expectedString = "*";
		Assert::AreEqual(expectedString, parser.operationStack.top());
		std::stack<std::string>::size_type s2 = 2;
		Assert::AreEqual(s2, parser.operationStack.size());
	};

	TEST_METHOD(ParserParseOpenBracket)
	{
		parser = Parser();
		parser.word = "(";
		Assert::IsTrue(parser.operationStack.empty());
		parser.parseOpenBracket();
		string expectedString = "(";
		Assert::AreEqual(expectedString, parser.operationStack.top());
	};

	TEST_METHOD(ParserParseCloseBracket)
	{
		parser = Parser();
		Assert::IsTrue(parser.operationStack.empty());
		parser.operationStack.push("-");
		parser.operationStack.push("(");
		parser.operationStack.push("*");
		parser.operationStack.push("+");

		parser.word = ")";
		parser.parseCloseBracket();
		string expectedString = "-";
		Assert::AreEqual(expectedString, parser.operationStack.top());
		std::stack<std::string>::size_type s2 = 1;
		Assert::AreEqual(s2, parser.operationStack.size());
	};

	TEST_METHOD(ParserGetWord)
	{
		parser = Parser();
		parser.tokenize("procedure Main { b=c; }");
		initializeParserStatementList();
		Assert::AreEqual(string("procedure"), parser.getWord());
		Assert::AreEqual(string("Main"), parser.getWord());
		Assert::AreEqual(string("{"), parser.getWord());
		Assert::AreEqual(string("b"), parser.getWord());
		Assert::AreEqual(string("="), parser.getWord());
		Assert::AreEqual(string("c"), parser.getWord());
		Assert::AreEqual(string(";"), parser.getWord());
		Assert::AreEqual(string("}"), parser.getWord());
	}

	TEST_METHOD(ParserGetRPN)
	{
		parser = Parser();
		queue<string> s1;
		s1.push("a");
		s1.push("+");
		s1.push("b");
		s1 = parser.getRPN(s1);
		string expStr = "a|b|+|";
		Assert::AreEqual(expStr, getQueueString(s1));
	};

	TEST_METHOD(ParserGetRPN2)
	{
		parser = Parser();
		std::deque<string> deq{ "a", "+", "b", "-", "c" };
		std::queue<string> s1(deq);
		s1 = parser.getRPN(s1);
		string expStr = "a|b|+|c|-|";
		Assert::AreEqual(expStr, getQueueString(s1));
	};

	TEST_METHOD(ParserGetRPN3)
	{
		parser = Parser();
		std::deque<string> deq{ "a", "+", "b", "-", "c", "*", "d" };
		std::queue<string> s1(deq);
		s1 = parser.getRPN(s1);
		string expStr = "a|b|+|c|d|*|-|";
		Assert::AreEqual(expStr, getQueueString(s1));
	};

	TEST_METHOD(ParserGetExpression)
	{
		parser = Parser();
		parser.tokenize("b + a;");
		parser.nextToken = parser.getToken();
		queue<string> expression = parser.getExpression();
		string expectedString = "b|a|+|";
		Assert::AreEqual(expectedString, getQueueString(expression));
	};

	TEST_METHOD(ParserGetExpression2)
	{
		parser = Parser();
		parser.tokenize("b + a * c;");
		parser.nextToken = parser.getToken();
		queue<string> expression = parser.getExpression();
		string expectedString = "b|a|c|*|+|";
		Assert::AreEqual(expectedString, getQueueString(expression));
	};

	TEST_METHOD(ParserGetExpression3)
	{
		parser = Parser();
		parser.tokenize("b - d + a * c;");
		parser.nextToken = parser.getToken();
		queue<string> expression = parser.getExpression();
		string expectedString = "b|d|-|a|c|*|+|";
		Assert::AreEqual(expectedString, getQueueString(expression));
	};

	TEST_METHOD(ParserGetExpression4)
	{
		parser = Parser();
		parser.tokenize("b - (d + a) * c;");
		parser.nextToken = parser.getToken();
		queue<string> expression = parser.getExpression();
		string expectedString = "b|d|a|+|c|*|-|";
		Assert::AreEqual(expectedString, getQueueString(expression));
	};


	TEST_METHOD(ParserGetExpression5)
	{
		parser = Parser();
		parser.tokenize("b - (d + a) * c;");
		parser.nextToken = parser.getToken();
		queue<string> expression = parser.getExpression();
		string expectedString = "b|d|a|+|c|*|-|";
		Assert::AreEqual(expectedString, getQueueString(expression));
	};

	TEST_METHOD(ParserGetExpression6)
	{
		parser = Parser();
		parser.tokenize("x - y + (5 * z) * 10;");
		parser.nextToken = parser.getToken();
		queue<string> expression = parser.getExpression();
		string expectedString = "x|y|-|5|z|*|10|*|+|";
		Assert::AreEqual(expectedString, getQueueString(expression));
	};

	TEST_METHOD(ParserGetExpression7)
	{
		parser = Parser();
		parser.tokenize("x - y + (5 * z) * 10 + (a * 3) + (max - 4);");
		parser.nextToken = parser.getToken();
		queue<string> expression = parser.getExpression();
		string expectedString = "x|y|-|5|z|*|10|*|+|a|3|*|+|max|4|-|+|";
		Assert::AreEqual(expectedString, getQueueString(expression));
	};




	TEST_METHOD(ParserMatch)
	{
		parser = Parser();
		parser.tokenize("a b c");
		parser.nextToken = parser.getToken();
		Assert::IsTrue(parser.match("a", false));
		Assert::IsTrue(parser.match("abc", true));
	};

	TEST_METHOD(ParserIfStatement)
	{
		parser = Parser();
		parser.tokenize("if x then { x = x + 1; } else { x = x + 1; }");
		initializeParserStatementList();
		Assert::IsTrue(parser.ifStatement());

	};

	TEST_METHOD(ParserWhileStatement)
	{
		parser = Parser();
		parser.tokenize("while y { x = x + 1; b = a;} ");
		initializeParserStatementList();
		Assert::IsTrue(parser.whileStatement());
	};

	TEST_METHOD(ParserCallStatement)
	{
		parser = Parser();
		parser.tokenize("call abc;");
		initializeParserStatementList();
		Assert::IsTrue(parser.callStatement());
	};

	TEST_METHOD(ParserAssignmentStatement)
	{
		parser = Parser();
		parser.tokenize("b = a;");
		initializeParserStatementList();
		Assert::IsTrue(parser.assignmentStatement());
	};

	TEST_METHOD(ParserStatement)
	{
		parser = Parser();
		parser.tokenize("b = a;");
		initializeParserStatementList();
		Assert::IsTrue(parser.statement());
	};

	TEST_METHOD(ParserStatement2)
	{
		parser = Parser();
		parser.tokenize("call a;");
		initializeParserStatementList();
		Assert::IsTrue(parser.statement());
	};

	TEST_METHOD(ParserStatement3)
	{
		parser = Parser();
		parser.tokenize("while y { x = x + 1; b = a;} ");
		initializeParserStatementList();
		Assert::IsTrue(parser.statement());
	};

	TEST_METHOD(ParserStatement4)
	{
		parser = Parser();
		parser.tokenize("if x then { x = x + 1; } else { x = x + 1; }");
		initializeParserStatementList();
		Assert::IsTrue(parser.statement());
	};

	TEST_METHOD(ParserStatementList)
	{
		parser = Parser();
		parser.tokenize("{a = 2*(b - 1); if x then { x = x + 1; } else { x = x + 1; } \
						call d; while y {x = 1;} }");
		initializeParserProcedure();
		Assert::IsTrue(parser.statementList());
	};

	TEST_METHOD(ParserStatementProcedure)
	{
		parser = Parser();
		parser.tokenize("procedure Main { a = 2*(b + 1); if x then { x = x + 1; } else { x = x + 1; } \
						call d; while y {x = 1;} }");
		initializeParser();
		Assert::IsTrue(parser.procedure());
	};

	TEST_METHOD(ParserStatementProgram)
	{
		parser = Parser();
		parser.tokenize("procedure Main { a = 2*(b - 1); if x then { x = x + 1; } else { x = x + 1; } \
						call d; while y {x = 1;} }");
		initializeParser();
		Assert::IsTrue(parser.program());
	};

};
}
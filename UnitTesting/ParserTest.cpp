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
	PKB pkb;
	Parser parser;

	string getQueueString(queue<string> expression) {
		std::stringstream ss;
		while (expression.size() != 0) {
			ss << expression.front();
			expression.pop();
		}
		return ss.str();
	}

	TEST_CLASS(ParserTest)
	{
	public:
		TEST_CLASS_INITIALIZE(setup)
		{
			parser = Parser();
		}
		TEST_METHOD(ParserGetToken) {
			parser.tokenize("a b c");
			string a = "a";
			Assert::AreEqual(a, parser.getToken());
		}


		TEST_METHOD(ParserMatch)
		{
			parser.tokenize("a b c");
			parser.nextToken = parser.getToken();
			Assert::IsTrue(parser.match("a", false));
			Assert::IsTrue(parser.match("abc", true));
		};

		TEST_METHOD(ParserIfStatement)
		{
			parser.tokenize("if x then { x = x + 1; } else { x = x + 1; }");
			parser.nextToken = parser.getToken();
			parser.stmListIdStack.push(1);
			Assert::IsTrue(parser.ifStatement());

		};

		TEST_METHOD(ParserWhileStatement)
		{
			parser.tokenize("while y { x = x + 1; b = a;} ");
			parser.nextToken = parser.getToken();
			parser.stmListIdStack.push(1);
			Assert::IsTrue(parser.whileStatement());
		};

		TEST_METHOD(ParserAssignmentStatement)
		{
			parser.tokenize("b = a;");
			parser.nextToken = parser.getToken();
			parser.stmListIdStack.push(1);
			Assert::IsTrue(parser.assignmentStatement());
		};

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


		TEST_METHOD(ParserGetRPN1)
		{
			parser = Parser();
			queue<string> s1;
			s1.push("a");
			s1.push("+");
			s1.push("b");
			s1 = parser.getRPN(s1);
			string expStr = "a|b|+|";
			Assert::AreEqual(expStr,  getQueueString(s1));
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

	};
}
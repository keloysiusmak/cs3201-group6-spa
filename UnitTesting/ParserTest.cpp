#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB.h"
#include "../source/PatternObject.h"
#include "../SPA/Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting
{
	TEST_CLASS(ParserTest)
	{
	public:

		TEST_METHOD(ParserMatchTest)
		{
			Parser parser;
			parser.tokenize("a b c");
			string nextToken = parser.getToken();
			Assert::IsTrue(parser.match("a", false));
			Assert::IsTrue(parser.match("abc", true));
		};
	/*
		TEST_METHOD(ParserIfStatementPositiveTest)
		{
			Parser parser;
			parser.tokenize("if x then { x = x + 1; } else { x = x + 1; }");
			string nextToken = parser.getToken();
			Assert::IsTrue(parser.ifStatement());

		}

		TEST_METHOD(ParserIfStatementNegativeTest)
		{
			Parser parser;
			parser.tokenize("if then { x = x + 1; } else { x = x + 1; }");
			string nextToken = parser.getToken();
			Assert::IsFalse(parser.ifStatement());
		}
		*/


	};
}
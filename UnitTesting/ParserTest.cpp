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

		TEST_METHOD(ParserGetToken) {
			Parser parser;
			parser.tokenize("a b c");
			string a = "a";
			Assert::AreEqual(a, parser.getToken());
		}


		TEST_METHOD(ParserMatch)
		{
			Parser parser;
			parser.tokenize("a b c");
			string nextToken = parser.getToken();
			Assert::IsTrue(parser.match("a", false));
			Assert::IsTrue(parser.match("abc", true));
		};
	
		TEST_METHOD(ParserIfStatement)
		{
			Parser parser;
			parser.tokenize("if x then { x = x + 1; } else { x = x + 1; }");
			string nextToken = parser.getToken();
			parser.stmListIdStack.push(1);
			Assert::IsTrue(parser.ifStatement());

		}

		TEST_METHOD(ParserWhileStatement)
		{
			Parser parser;
			parser.tokenize("while y { x = x + 1; b = a;} ");
			string nextToken = parser.getToken();
			parser.stmListIdStack.push(1);
			Assert::IsTrue(parser.whileStatement());
		}



	};
}
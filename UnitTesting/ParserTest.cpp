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
			string nextToken = parser.getToken();
			Assert::IsTrue(parser.match("a", false));
			Assert::IsTrue(parser.match("abc", true));
		};

		TEST_METHOD(ParserIfStatement)
		{
			parser.tokenize("if x then { x = x + 1; } else { x = x + 1; }");
			string nextToken = parser.getToken();
			parser.stmListIdStack.push(1);
			Assert::IsTrue(parser.ifStatement());

		};

		TEST_METHOD(ParserWhileStatement)
		{
			parser.tokenize("while y { x = x + 1; b = a;} ");
			string nextToken = parser.getToken();
			parser.stmListIdStack.push(1);
			Assert::IsTrue(parser.whileStatement());
		};
	};
}
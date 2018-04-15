#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include "../SPA/PatternObject.h"
#include "../SPA/Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UtilsTest
{
	TEST_CLASS(UtilsTest) 
	{
	public:
		TEST_METHOD(UtilsSanitise) {
			string testString = "This \
								is \
								a sanitise test \
									case";
			string result = Utils::sanitise(testString);
			string match = "This is a sanitise test case";
			Assert::AreEqual(match, result);
		}

		TEST_METHOD(UtilsExplode) {
			string testString = "procedure    Test { a = b; c = d+2; }";
			string sanitised = Utils::sanitise(testString);
			vector<string> tokens = Utils::explode(sanitised, ParserConstants::DELIM_STRING, ParserConstants::DELIMITERS);
			Assert::AreEqual(tokens[0], string("procedure"));
			Assert::AreEqual(tokens[1], string("Test"));
			Assert::AreEqual(tokens[2], string("{"));
			Assert::AreEqual(tokens[3], string("a"));
			Assert::AreEqual(tokens[4], string("="));
			Assert::AreEqual(tokens[5], string("b"));
			Assert::AreEqual(tokens[6], string(";"));
			Assert::AreEqual(tokens[7], string("c"));
			Assert::AreEqual(tokens[8], string("="));
			Assert::AreEqual(tokens[9], string("d"));
			Assert::AreEqual(tokens[10], string("+"));
			Assert::AreEqual(tokens[11], string("2"));
			Assert::AreEqual(tokens[12], string(";"));
			Assert::AreEqual(tokens[13], string("}"));
		}

		TEST_METHOD(UtilsInValidName) {
			Assert::AreEqual(true, Utils::isValidName("procedure"));
			Assert::AreEqual(true, Utils::isValidName("aY789"));
			Assert::AreEqual(true, Utils::isValidName("a987688999"));
			Assert::AreEqual(true, Utils::isValidName("A0987"));
			Assert::AreEqual(false, Utils::isValidName("123"));
			Assert::AreEqual(false, Utils::isValidName("/888"));
			Assert::AreEqual(false, Utils::isValidName("a??"));
			Assert::AreEqual(false, Utils::isValidName("Mo++"));
		}

		TEST_METHOD(UtilsTrim) {
			Assert::AreEqual(string("11"), Utils::trim(string("   11  ")));
			Assert::AreEqual(string("1a b"), Utils::trim(string("   1a b  ")));
			Assert::AreEqual(string("b%b"), Utils::trim(string("   b%b  ")));
		}

		TEST_METHOD(UtilsGetWordAndPop) {
			std::deque<string> deq{ "a", "+", "b", "-", "c" };
			std::queue<string> expression(deq);
			Assert::AreEqual(string("a"), Utils::getWordAndPop(expression));
			Assert::AreEqual(string("+"), Utils::getWordAndPop(expression));
			Assert::AreEqual(string("b"), Utils::getWordAndPop(expression));
			Assert::AreEqual(string("-"), Utils::getWordAndPop(expression));
			Assert::AreEqual(string("c"), Utils::getWordAndPop(expression));
		}

		TEST_METHOD(UtilsIsValidIntger) {
			Assert::AreEqual(true, Utils::isInteger(string("11")));
			Assert::AreEqual(true, Utils::isInteger(string("112")));
			Assert::AreEqual(true, Utils::isInteger(string("121")));
			Assert::AreEqual(false, Utils::isInteger(string("1a1")));
			Assert::AreEqual(false, Utils::isInteger(string("1+1")));
		}

		TEST_METHOD(UtilsIsValidFactor) {
			Assert::AreEqual(true, Utils::isValidFactor(string("aa")));
			Assert::AreEqual(true, Utils::isValidFactor(string("11")));
			Assert::AreEqual(false, Utils::isValidFactor(string("1a1")));
			Assert::AreEqual(false, Utils::isValidFactor(string("a-b")));
		}

		TEST_METHOD(UtilsIsValidOperator) {
			Assert::AreEqual(true, Utils::isValidOperator(string("+")));
			Assert::AreEqual(true, Utils::isValidOperator(string("-")));
			Assert::AreEqual(true, Utils::isValidOperator(string("*")));
			Assert::AreEqual(false, Utils::isValidOperator(string("/")));
			Assert::AreEqual(false, Utils::isValidOperator(string("^")));
			Assert::AreEqual(false, Utils::isValidOperator(string("$$$")));
		}

		TEST_METHOD(UtilsIsValidConstant) {
			Assert::AreEqual(true, Utils::isValidConstant(string("12")));
			Assert::AreEqual(true, Utils::isValidConstant(string("12999")));
			Assert::AreEqual(true, Utils::isValidConstant(string("0")));
			Assert::AreEqual(false, Utils::isValidConstant(string("-2")));
			Assert::AreEqual(false, Utils::isValidConstant(string("a1")));
			Assert::AreEqual(false, Utils::isValidConstant(string("**")));
		}

		TEST_METHOD(UtilsIsOpenBracket) {
			Assert::AreEqual(true, Utils::isOpenBracket(string("(")));
			Assert::AreEqual(false, Utils::isOpenBracket(string("( ")));
			Assert::AreEqual(false, Utils::isOpenBracket(string(")")));
			Assert::AreEqual(false, Utils::isOpenBracket(string("{")));
			Assert::AreEqual(false, Utils::isOpenBracket(string("<")));
		}

		TEST_METHOD(UtilsIsCloseBracket) {
			Assert::AreEqual(true, Utils::isCloseBracket(string(")")));
			Assert::AreEqual(false, Utils::isCloseBracket(string(") ")));
			Assert::AreEqual(false, Utils::isCloseBracket(string("(")));
			Assert::AreEqual(false, Utils::isCloseBracket(string("}")));
			Assert::AreEqual(false, Utils::isCloseBracket(string(">")));
		}
	};
}
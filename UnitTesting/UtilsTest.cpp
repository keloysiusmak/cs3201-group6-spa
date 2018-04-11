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
			string testString = "procedure Test { a = b; c = d+2; }";
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
	};
}
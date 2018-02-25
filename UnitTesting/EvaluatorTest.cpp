#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Evaluator.h"
#include "./PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {

	Evaluator evaluator;
	PKB pkbStub;
	QueryObject queryObjectStub;

	TEST_CLASS(EvaluatorTest) {

		TEST_METHOD(ValidQueryObjectSetterTest) {
			evaluator.setQueryObject(queryObjectStub);
			Assert::AreEqual(true, evaluator.isValidQuery());
		}

		TEST_METHOD(InvalidQueryObjectSetterTest) {
			evaluator.setInvalidQuery("Invalid Query");
			Assert::AreEqual(false, evaluator.isValidQuery());
		}
	};
}
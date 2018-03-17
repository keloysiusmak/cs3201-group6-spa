#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryEvaluator.h"
#include "./PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace EvaluatorTest {

	QueryEvaluator evaluator;
	PKB pkbStub;
	QueryObject queryObjectStub;

	TEST_CLASS(EvaluatorTest) {

		TEST_METHOD(EvaluatorValidQueryObjectSetter) {
			evaluator.setQueryObject(queryObjectStub);
			Assert::AreEqual(true, evaluator.isValidQuery());
		};

		TEST_METHOD(EvaluatorInvalidQueryObjectSetter) {
			evaluator.setInvalidQuery({});
			Assert::AreEqual(false, evaluator.isValidQuery());
		};
	};
}
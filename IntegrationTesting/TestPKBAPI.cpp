#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/PKB.h";
#include "../SPA/Evaluator.h";
#include "../SPA/QueryObject.h";
#include "../SPA/Pattern.h";
#include "../SPA/Declaration.h";
#include "../SPA/Clause.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PKBAPIIntegrationTesting
{
	PKB pkb;
	Evaluator evaluator;
	list<string> _variableID;

	TEST_CLASS(PKBAPIIntegration)
	{
	public:
		

		TEST_CLASS_INITIALIZE(setup)
		{
			evaluator.setPKB(pkb);
		}

		TEST_METHOD(PKBAPIEvaluate)
		{
			evaluator.evaluateQuery();
		}

	};
}
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

namespace PKBPQLIntegrationTesting
{
	PKB pkb;
	Evaluator evaluator;
	list<string> _variableID;

	TEST_CLASS(PKBPQLIntegration)
	{
	public:
		

		TEST_CLASS_INITIALIZE(setup)
		{
			_variableID.push_back(std::to_string(pkb.insertToNameTable(9, "x")));
			evaluator.setPKB(pkb);
		}

		TEST_METHOD(PKBPQLFollows)
		{
		}

	};
}
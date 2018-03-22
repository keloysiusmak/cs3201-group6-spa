#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/Constants.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace DesignExtractorTest
{

	PKB pkb;
	DesignExtractor de;

	TEST_CLASS(PKBTest)
	{
	public:
		TEST_CLASS_INITIALIZE(setup)
		{
			pkb = PKB();
			pkb.insertToTable(STATEMENT_TABLE, 1, { {1},{},{}, {1} });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{},{},{1} });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 1,2 },{},{},{2} });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 2,3,4 },{},{},{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{},{},{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 4,5 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 5 },{},{},{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 9, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{1,2},{3},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{4,5},{6},{ 1 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { {0}, {1,2,3,9}, {1} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { {3},{ 4,8 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { {4},{ 5 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 4 },{ 6 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 6 },{ 7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 0 },{ 10 },{2} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 0 },{ 11 },{3} });
			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{ },{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 6 },{1,2},{3} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 7 },{4,5},{6} });
			pkb.insertToTable(CALLS_TABLE, 1, { {2,3}, {} });
			pkb.insertToTable(CALLS_TABLE, 2, { { 3 },{5} });
			pkb.insertToTable(CALLS_TABLE, 3, { { },{ 7 } });
			pkb.insertToTable(USES_TABLE, 1, { {10},{2} });
			pkb.insertToTable(USES_TABLE, 2, { { 10 },{ 2 } });
			pkb.insertToTable(USES_TABLE, 4, { { 11 },{ 3 } });
			pkb.insertToTable(USES_TABLE, 5, { { 11 },{ 3 } });
			pkb.insertToTable(MODIFIES_TABLE, 3, { { 10 },{ 2 } });
			pkb.insertToTable(MODIFIES_TABLE, 6, { { 11 },{ 3 } });

			pkb.insertToNameTable(PROC_TABLE, {"a"});
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });
			de = DesignExtractor();
		}
		TEST_METHOD(DesignExtractorExtract)
		{
			Assert::AreEqual(true, de.extract(pkb));
		}
		TEST_METHOD(DesignExtractorExtractNext)
		{
			de.extract(pkb);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_TABLE, 1)[0][0]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_TABLE, 2)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_TABLE, 3)[0][0]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_TABLE, 3)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_TABLE, 4)[0][0]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_TABLE, 4)[0][1]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_TABLE, 5)[0][0]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_TABLE, 6)[0][0]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_TABLE, 6)[0][1]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_TABLE, 7)[0][0]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_TABLE, 8)[0][0]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_INVERSE_TABLE, 2)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_INVERSE_TABLE, 3)[0][0]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_INVERSE_TABLE, 3)[0][1]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_INVERSE_TABLE, 4)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_INVERSE_TABLE, 5)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_INVERSE_TABLE, 6)[0][0]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_INVERSE_TABLE, 6)[0][1]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_INVERSE_TABLE, 7)[0][0]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_INVERSE_TABLE, 8)[0][0]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_INVERSE_TABLE, 8)[0][1]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_INVERSE_TABLE, 9)[0][0]);
		}
		TEST_METHOD(DesignExtractorExtractCallStatements)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> result;
			Assert::AreEqual(true, (result == pkb.getFromTable(CALL_STATEMENT_TABLE, 1)));
			Assert::AreEqual(2, pkb.getFromTable(CALL_STATEMENT_TABLE, 5)[0][0]);
			Assert::AreEqual(3, pkb.getFromTable(CALL_STATEMENT_TABLE, 7)[0][0]);
			result.clear();
			Assert::AreEqual(true, (result == pkb.getFromTable(CALL_STATEMENT_TABLE, 4)));
		}
		TEST_METHOD(DesignExtractorExtractCallsInverse)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> result;
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_INVERSE_TABLE, 1)));
			Assert::AreEqual(1, pkb.getFromTable(CALLS_INVERSE_TABLE, 2)[0][0]);
			Assert::AreEqual(1, pkb.getFromTable(CALLS_INVERSE_TABLE, 3)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(CALLS_INVERSE_TABLE, 3)[0][1]);
			result.clear();
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_INVERSE_TABLE, 4)));
		}
		TEST_METHOD(DesignExtractorExtractCallsStar)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> result;
			result = { { 2,3} };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_TABLE, 1)));
			result = { { 3 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_TABLE, 2)));
			result = { { 1,2 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_INVERSE_TABLE, 3)));
			result.clear();
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_INVERSE_TABLE, 4)));
			result = { { 1 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_INVERSE_TABLE, 2)));
			result.clear();
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_INVERSE_TABLE, 1)));
		}
		TEST_METHOD(DesignExtractorExtractUsesModifies)
		{
			de.extract(pkb);
			std::vector<int> result;
			result = {1,2 };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 5)[1]));
			result = { 3 };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 5)[2]));
			result = { 4,5 };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 7)[1]));
			result = { 6 };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 7)[2]));
			result = { 1,2,4,5 };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 4)[1]));
			result = { 3,6 };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 4)[2]));
			result = { 1,2,4,5 };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 3)[1]));
			result = { 3,6 };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 3)[2]));
			result = { 1,2,4,5 };
			Assert::AreEqual(true, (result == pkb.getFromTable(PROC_INFO_TABLE, 1)[1]));
			result = { 3,6 };
			Assert::AreEqual(true, (result == pkb.getFromTable(PROC_INFO_TABLE, 1)[2]));
			result = { 1,2 };
			Assert::AreEqual(true, (result == pkb.getFromTable(PROC_INFO_TABLE, 2)[1]));
			result = { 3 };
			Assert::AreEqual(true, (result == pkb.getFromTable(PROC_INFO_TABLE, 2)[2]));
			result = { 4,5 };
			Assert::AreEqual(true, (result == pkb.getFromTable(PROC_INFO_TABLE, 3)[1]));
			result = { 6 };
			Assert::AreEqual(true, (result == pkb.getFromTable(PROC_INFO_TABLE, 3)[2]));
			std::vector < std::vector<int>> newResult;
			newResult = { {3,4,5,10}, {1,2} };
			Assert::AreEqual(true, (newResult == pkb.getFromTable(USES_TABLE, 1)));
			newResult = { { 3,4,5,10 },{ 1,2 } };
			Assert::AreEqual(true, (newResult == pkb.getFromTable(USES_TABLE, 2)));
			newResult = { { 3,4,6,7,11 },{ 1,3 } };
			Assert::AreEqual(true, (newResult == pkb.getFromTable(USES_TABLE, 4)));
			newResult = { { 3,4,6,7,11 },{ 1,3 } };
			Assert::AreEqual(true, (newResult == pkb.getFromTable(USES_TABLE, 5)));
			newResult = { { 3,4,5,10 },{ 1,2 } };
			Assert::AreEqual(true, (newResult == pkb.getFromTable(MODIFIES_TABLE, 3)));
			newResult = { { 3,4,6,7,11 },{ 1,3 } };
			Assert::AreEqual(true, (newResult == pkb.getFromTable(MODIFIES_TABLE, 6)));
		}
	};
}
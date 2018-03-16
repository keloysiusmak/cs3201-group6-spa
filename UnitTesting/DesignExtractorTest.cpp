#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/Constants.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting
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
			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { {0}, {1,2,3,9}, {} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { {3},{ 4,8 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { {4},{ 5 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 4 },{ 6 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 6 },{ 7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 0 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 0 },{ 11 },{} });
			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{ },{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 6 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 7 },{},{} });
			pkb.insertToTable(CALLS_TABLE, 1, { {2,3}, {} });
			pkb.insertToTable(CALLS_TABLE, 2, { { 3 },{5} });
			pkb.insertToTable(CALLS_TABLE, 3, { { },{ 7 } });

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
		TEST_METHOD(DesignExtractorExtractNextStar)
		{
			de.extract(pkb);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_STAR_TABLE, 1)[0][0]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 1)[0][1]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 1)[0][2]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 1)[0][3]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 1)[0][4]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 1)[0][5]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_TABLE, 1)[0][6]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_STAR_TABLE, 1)[0][7]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][4]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][5]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][6]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][4]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][5]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][6]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][4]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][5]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][6]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][4]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][5]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][6]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][4]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][5]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][6]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][4]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][5]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][6]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 8)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 8)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 8)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 8)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 8)[0][4]);
			Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_TABLE, 8)[0][5]);
			Assert::AreEqual(9, pkb.getFromTable(NEXT_STAR_TABLE, 8)[0][6]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 2)[0][0]);
			for (int i = 3; i <= 9; i++) {
				Assert::AreEqual(1, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, i)[0][0]);
				Assert::AreEqual(2, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, i)[0][1]);
				Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, i)[0][2]);
				Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, i)[0][3]);
				Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, i)[0][4]);
				Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, i)[0][5]);
				Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, i)[0][6]);
				Assert::AreEqual(8, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, i)[0][7]);
			}
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
		TEST_METHOD(DesignExtractorExtractCalls)
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
			std::vector<int> a = pkb.getFromTable(STATEMENT_TABLE, 5)[1];
			for (int i = 0; i < a.size(); i++) {
				Logger::WriteMessage(to_string(i).c_str());
			}
			result = { { 1,2,4,5 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 5)[1]));
			result = { { 3,6 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(STATEMENT_TABLE, 5)[2]));
		}
	};
}
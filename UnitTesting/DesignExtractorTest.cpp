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
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 4,5 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 5 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { {0}, {1,2,3}, {} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { {3},{ 4 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { {4},{ 5 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 4 },{ 6 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 6 },{ 7 },{} });
			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{ },{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 4, { { 1 },{},{} });
			pkb.insertToTable(CALLS_TABLE, 1, { {2}, {4} });
			pkb.insertToTable(CALLS_TABLE, 2, { { 3, 4 },{5} });

			pkb.insertToNameTable(PROC_TABLE, {"a"});
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });
			pkb.insertToNameTable(PROC_TABLE, { "d" });
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
			Assert::AreEqual(5, pkb.getFromTable(NEXT_TABLE, 4)[0][0]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_TABLE, 4)[0][1]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_TABLE, 6)[0][0]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_TABLE, 6)[0][1]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_TABLE, 7)[0][0]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_INVERSE_TABLE, 2)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_INVERSE_TABLE, 3)[0][0]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_INVERSE_TABLE, 4)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_INVERSE_TABLE, 5)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_INVERSE_TABLE, 6)[0][0]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_INVERSE_TABLE, 6)[0][1]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_INVERSE_TABLE, 7)[0][0]);
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
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 2)[0][4]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 3)[0][4]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 4)[0][4]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 5)[0][4]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 6)[0][4]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][0]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][1]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][2]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][3]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_TABLE, 7)[0][4]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 2)[0][0]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 3)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 3)[0][1]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 4)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 4)[0][1]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 4)[0][2]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 4)[0][3]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 4)[0][4]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 4)[0][5]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 4)[0][6]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 5)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 5)[0][1]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 5)[0][2]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 5)[0][3]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 5)[0][4]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 5)[0][5]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 5)[0][6]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][1]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][2]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][3]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][4]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][5]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][6]);
			Assert::AreEqual(1, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][1]);
			Assert::AreEqual(3, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][2]);
			Assert::AreEqual(4, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][3]);
			Assert::AreEqual(5, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][4]);
			Assert::AreEqual(6, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][5]);
			Assert::AreEqual(7, pkb.getFromTable(NEXT_STAR_INVERSE_TABLE, 6)[0][6]);
		}
		TEST_METHOD(DesignExtractorExtractCallsInverse)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> result;
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_INVERSE_TABLE, 1)));
			Assert::AreEqual(1, pkb.getFromTable(CALLS_INVERSE_TABLE, 2)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(CALLS_INVERSE_TABLE, 3)[0][0]);
			Assert::AreEqual(2, pkb.getFromTable(CALLS_INVERSE_TABLE, 4)[0][0]);
		}
		TEST_METHOD(DesignExtractorExtractCalls)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> result;
			result = { { 2,3,4} };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_TABLE, 1)));
			result = { { 3, 4 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_TABLE, 2)));
			result = { { 1,2 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_INVERSE_TABLE, 3)));
			result = { { 1,2 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_INVERSE_TABLE, 4)));
			result = { { 1 } };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_INVERSE_TABLE, 2)));
			result.clear();
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_INVERSE_TABLE, 1)));
		}
	};
}
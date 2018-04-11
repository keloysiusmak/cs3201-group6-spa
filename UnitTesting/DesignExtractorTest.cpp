#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/DesignExtractor.h"
#include "../SPA/Constants.h";
#include "../SPA/Utils.h";

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
			pkb.insertToTable(STATEMENT_TABLE, 1, { {1},{},{6}, {1} });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{},{6},{1} });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 1,2 },{},{},{2} });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 2,3,4 },{},{},{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{},{},{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 4,5 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 5 },{},{},{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 2 },{},{6},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 9, { { 1 },{},{6},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{1,2},{3},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{4,5},{6},{ 1 } });

			pkb.insertToNameTable(PATTERN_TABLE, { "a", "1" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a", "1" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "d" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });

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
			pkb.insertToTable(CALLS_TABLE, 2, { { },{5} });
			pkb.insertToTable(CALLS_TABLE, 3, { { },{ 7 } });
			pkb.insertToTable(CONST_TABLE, 1, { { 1 } });
			pkb.insertToTable(CONST_TABLE, 2, { { 1 } });
			pkb.insertToTable(CONST_TABLE, 3, { { 1 } });
			pkb.insertToTable(CONST_TABLE, 4, { { 1 } });
			pkb.insertToTable(CONST_TABLE, 5, { { 1 } });
			pkb.insertToTable(CONST_TABLE, 6, { { 1 } });
			pkb.insertToTable(CONST_TABLE, 7, { { 10 } });
			pkb.insertToTable(CONST_TABLE, 8, { { 10 } });
			pkb.insertToTable(CONST_TABLE, 9, { { 10 } });
			pkb.insertToTable(CONST_TABLE, 10, { { 10 } });
			pkb.insertToTable(CONST_TABLE, 11, { { 10 } });
			pkb.insertToTable(CONST_TABLE, 12, { { 10 } });
			pkb.insertToTable(USES_TABLE, 1, { {10},{2} });
			pkb.insertToTable(USES_TABLE, 2, { { 10 },{ 2 } });
			pkb.insertToTable(USES_TABLE, 4, { { 11 },{ 3 } });
			pkb.insertToTable(USES_TABLE, 5, { { 11 },{ 3 } });
			pkb.insertToTable(MODIFIES_TABLE, 3, { { 10 },{ 2 } });
			pkb.insertToTable(MODIFIES_TABLE, 6, { { 1,2,3,8,9,11 },{ 1,3 } });

			pkb.insertToNameTable(PROC_TABLE, {"a"});
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			pkb.insertToNameTable(VAR_TABLE, { "f" });
			pkb.insertToNameTable(VAR_TABLE, { "e" });
			pkb.insertToNameTable(VAR_TABLE, { "d" });
			pkb.insertToNameTable(VAR_TABLE, { "c" });
			pkb.insertToNameTable(VAR_TABLE, { "b" });
			pkb.insertToNameTable(VAR_TABLE, { "a" });
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
			result.clear();
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_INVERSE_TABLE, 4)));
		}
		TEST_METHOD(DesignExtractorExtractCallsStar)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> result;
			result = { { 2,3} };
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_TABLE, 1)));
			result.clear();
			Assert::AreEqual(true, (result == pkb.getFromTable(CALLS_STAR_TABLE, 2)));
			result = { { 1 } };
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
			newResult = { { 1,2,3,4,6,7,8,9,11 },{ 1,3 } };
			Assert::AreEqual(true, (newResult == pkb.getFromTable(MODIFIES_TABLE, 6)));
		}
		TEST_METHOD(DesignExtractorPrecomputeConstants)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> data = { { 1,2,3,4,5,6 } };
			Assert::AreEqual(true, (pkb.getFromTable(CONST_STMT_TABLE, 1) == data));
			data = { { 7,8,9,10,11,12 } };
			Assert::AreEqual(true, (pkb.getFromTable(CONST_STMT_TABLE, 10) == data));
			data = { { 1,2,3,4,5,6 } };
			Assert::AreEqual(true, (pkb.getFromTable(CONST_PROC_TABLE, 1) == data));
			data = { { 7,8,9,10,11,12 } };
			Assert::AreEqual(true, (pkb.getFromTable(CONST_PROC_TABLE, 2) == data));
		}
		TEST_METHOD(DesignExtractorPrecomputeStatementLists)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> data = { { 1, 4,5,6,7,10,11 } };
			Assert::AreEqual(true, (pkb.getFromTable(STATEMENT_LIST_INFO_TABLE, 1) == data));
		}
		TEST_METHOD(DesignExtractorPrecomputeWithProcNameVarName)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> data = { { 1,6 },{ 2,5 },{ 3,4 } };
			Assert::AreEqual(true, (pkb.getFromTable(PROC_NAME_VAR_NAME_TABLE, 0) == data));
		}
		TEST_METHOD(DesignExtractorPrecomputeWithProcNameCallProcName)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> data = { { 2,5 },{ 3,7 } };
			Assert::AreEqual(true, (pkb.getFromTable(PROC_NAME_CALL_NAME_TABLE, 0) == data));
		}
		TEST_METHOD(DesignExtractorPrecomputeWithVarNameCallProcName)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> data = { { 5,5 },{ 4,7 } };
			Assert::AreEqual(true, (pkb.getFromTable(VAR_NAME_CALL_NAME_TABLE, 0) == data));
		}
		TEST_METHOD(DesignExtractorPrecomputeWithStmtNoConstValue)
		{
			de.extract(pkb);
			std::vector<std::vector<int>> data = { { 1,1 },{ 2,2 },{ 3,3 },{ 4,4 },{ 5,5 },{ 6,6 },{ 7, 7 },{ 8,8 },{ 9,9 },{ 10,10 },{ 11, 11 } };
			std::vector<std::vector<int>> result = pkb.getFromTable(STMT_NO_CONST_VALUE_TABLE, 0);
			std::sort(result.begin(), result.end());
			Assert::AreEqual(true, (result == data));
		}
		TEST_METHOD(DesignExtractorPrecomputePatternTwoSyn)
		{
			de.extract(pkb);
			std::vector<int> assignC = { 1,2,8,9,10,11 };
			std::vector<int> assignV = { 6,6,6,6,3,6  };
			for (int i = 0; i < assignC.size(); i++) {
				std::vector<std::vector<int>> result = pkb.getFromTable(PATTERN_ASSIGN_VARIABLE_TABLE, assignC[i]);
				Assert::AreEqual(result[0][0], assignV[i]);
			}
			std::vector<int> whileC = { 3,6 };
			std::vector<int> whileV = { 6,6 };
			for (int i = 0; i < whileC.size(); i++) {
				std::vector<std::vector<int>> result = pkb.getFromTable(PATTERN_WHILE_VARIABLE_TABLE, whileC[i]);
				Assert::AreEqual(result[0][0], whileV[i]);
			}
			std::vector<int> ifC = { 4 };
			std::vector<int> ifV = { 6 };
			for (int i = 0; i < ifC.size(); i++) {
				std::vector<std::vector<int>> result = pkb.getFromTable(PATTERN_IF_VARIABLE_TABLE, ifC[i]);
				Assert::AreEqual(result[0][0], ifV[i]);
			}
		}
		TEST_METHOD(DesignExtractorCountFollows)
		{
			de.extract(pkb);
			Assert::AreEqual(4, pkb.getFromResultTable(RelationFollows, 0, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationFollows, 1, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationFollows, 2, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationFollows, 4, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationFollows, 8, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationFollows, 0, 1));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationFollows, 0, 2));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationFollows, 0, 8));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationFollows, 0, 9));
		}
		TEST_METHOD(DesignExtractorCountUsesProcedure)
		{
			de.extract(pkb);
			Assert::AreEqual(8, pkb.getFromResultTable(RelationUsesProcedure, 0, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationUsesProcedure, 1, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationUsesProcedure, 2, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationUsesProcedure, 3, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationUsesProcedure, 0, 1));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationUsesProcedure, 0, 2));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationUsesProcedure, 0, 4));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationUsesProcedure, 0, 5));
		}
		TEST_METHOD(DesignExtractorCountUsesStatement)
		{
			de.extract(pkb);
			Assert::AreEqual(18, pkb.getFromResultTable(RelationUsesStatement, 0, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationUsesStatement, 1, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationUsesStatement, 3, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationUsesStatement, 4, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationUsesStatement, 6, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationUsesStatement, 0, 1));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationUsesStatement, 0, 2));
			Assert::AreEqual(5, pkb.getFromResultTable(RelationUsesStatement, 0, 4));
			Assert::AreEqual(5, pkb.getFromResultTable(RelationUsesStatement, 0, 5));
		}
		TEST_METHOD(DesignExtractorCountModifiesProcedure)
		{
			de.extract(pkb);
			Assert::AreEqual(4, pkb.getFromResultTable(RelationModifiesProcedure, 0, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationModifiesProcedure, 1, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationModifiesProcedure, 2, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationModifiesProcedure, 3, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationModifiesProcedure, 0, 3));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationModifiesProcedure, 0, 6));
		}
		TEST_METHOD(DesignExtractorCountModifiesStatement)
		{
			de.extract(pkb);
			Assert::AreEqual(13, pkb.getFromResultTable(RelationModifiesStatement, 0, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationModifiesStatement, 1, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationModifiesStatement, 3, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationModifiesStatement, 4, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationModifiesStatement, 6, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationModifiesStatement, 0, 3));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationModifiesStatement, 0, 6));
		}
		TEST_METHOD(DesignExtractorCountParent)
		{
			de.extract(pkb);
			Assert::AreEqual(5, pkb.getFromResultTable(RelationParent, 0, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationParent, 3, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationParent, 4, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationParent, 5, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationParent, 10, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationParent, 0, 3));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationParent, 0, 4));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationParent, 0, 5));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationParent, 0, 7));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationParent, 0, 11));
		}
		TEST_METHOD(DesignExtractorCountParentStar)
		{
			de.extract(pkb);
			vector<vector<int>> a = pkb.getAllParentStar();
			Assert::AreEqual(9, pkb.getFromResultTable(RelationParentStar, 0, 0));
			Assert::AreEqual(5, pkb.getFromResultTable(RelationParentStar, 3, 0));
			Assert::AreEqual(3, pkb.getFromResultTable(RelationParentStar, 4, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationParentStar, 5, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationParentStar, 10, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationParentStar, 0, 3));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationParentStar, 0, 4));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationParentStar, 0, 5));
			Assert::AreEqual(3, pkb.getFromResultTable(RelationParentStar, 0, 7));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationParentStar, 0, 11));
		}
		TEST_METHOD(DesignExtractorCountCalls)
		{
			de.extract(pkb);
			Assert::AreEqual(2, pkb.getFromResultTable(RelationCalls, 0, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationCalls, 1, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationCalls, 2, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationCalls, 3, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationCalls, 0, 1));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationCalls, 0, 2));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationCalls, 0, 3));
		}
		TEST_METHOD(DesignExtractorCountCallsStar)
		{
			de.extract(pkb);
			Assert::AreEqual(2, pkb.getFromResultTable(RelationCallsStar, 0, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationCallsStar, 1, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationCallsStar, 2, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationCallsStar, 3, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationCallsStar, 0, 1));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationCallsStar, 0, 2));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationCallsStar, 0, 3));
		}
		TEST_METHOD(DesignExtractorCountNext)
		{
			de.extract(pkb);
			Assert::AreEqual(11, pkb.getFromResultTable(RelationNext, 0, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationNext, 1, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationNext, 3, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationNext, 4, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationNext, 6, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationNext, 9, 0));
			Assert::AreEqual(0, pkb.getFromResultTable(RelationNext, 0, 1));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationNext, 0, 3));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationNext, 0, 4));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationNext, 0, 8));
		}
		TEST_METHOD(DesignExtractorCountNextStar)
		{
			de.extract(pkb);
			Assert::AreEqual(83, pkb.getFromResultTable(RelationNextStar, 0, 0));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationNextStar, 1, 0));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationNextStar, 3, 0));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationNextStar, 4, 0));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationNextStar, 6, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationNextStar, 10, 0));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationNextStar, 0, 1));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationNextStar, 0, 3));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationNextStar, 0, 4));
			Assert::AreEqual(9, pkb.getFromResultTable(RelationNextStar, 0, 8));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationNextStar, 0, 10));
		}
		TEST_METHOD(DesignExtractorCountAffects)
		{
			de.extract(pkb);
			Assert::AreEqual(18, pkb.getFromResultTable(RelationAffects, 0, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffects, 1, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffects, 3, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffects, 4, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffects, 6, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationAffects, 10, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffects, 0, 1));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffects, 0, 3));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffects, 0, 4));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffects, 0, 8));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationAffects, 0, 10));
		}
		TEST_METHOD(DesignExtractorCountAffectsStar)
		{
			de.extract(pkb);
			Assert::AreEqual(18, pkb.getFromResultTable(RelationAffectsStar, 0, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffectsStar, 1, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffectsStar, 3, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffectsStar, 4, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffectsStar, 6, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationAffectsStar, 10, 0));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffectsStar, 0, 1));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffectsStar, 0, 3));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffectsStar, 0, 4));
			Assert::AreEqual(4, pkb.getFromResultTable(RelationAffectsStar, 0, 8));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationAffectsStar, 0, 10));
		}
		TEST_METHOD(DesignExtractorCountWithProcNameVarName)
		{
			de.extract(pkb);
			Assert::AreEqual(3, pkb.getFromResultTable(RelationWithName, PROCEDURE, VARIABLE));
		}
		TEST_METHOD(DesignExtractorCountWithProcNameCallProcName)
		{
			de.extract(pkb);
			Assert::AreEqual(2, pkb.getFromResultTable(RelationWithName, PROCEDURE, CALL));
		}
		TEST_METHOD(DesignExtractorCountWithVarNameCallProcName)
		{
			de.extract(pkb);
			Assert::AreEqual(2, pkb.getFromResultTable(RelationWithName, VARIABLE, CALL));
		}
		TEST_METHOD(DesignExtractorCountWithStmtNoConstValue)
		{
			de.extract(pkb);
			Assert::AreEqual(11, pkb.getFromResultTable(RelationWithConst, 0, 0));
			Assert::AreEqual(6, pkb.getFromResultTable(RelationWithConst, ASSIGNMENT_TYPE, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationWithConst, WHILE_TYPE, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationWithConst, IF_TYPE, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationWithConst, CALL_TYPE, 0));
		}
		TEST_METHOD(DesignExtractorCountPattern)
		{
			de.extract(pkb);
			Assert::AreEqual(6, pkb.getFromResultTable(RelationPattern, ASSIGNMENT_TYPE, 0));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationPattern, WHILE_TYPE, 0));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationPattern, IF_TYPE, 0));
			Assert::AreEqual(6, pkb.getFromResultTable(RelationPattern, ASSIGNMENT_TYPE, VARIABLE));
			Assert::AreEqual(2, pkb.getFromResultTable(RelationPattern, WHILE_TYPE, VARIABLE));
			Assert::AreEqual(1, pkb.getFromResultTable(RelationPattern, IF_TYPE, VARIABLE));
		}
	};
}
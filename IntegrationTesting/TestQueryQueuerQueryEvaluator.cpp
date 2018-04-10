#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Preprocessor.h"
#include "../SPA/QueryQueuer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace QueryQueuerQueryEvaluatorIntegrationTesting
{
	QueryQueuer qq;
	QueryEvaluator evaluator;
	PKB pkb;

	TEST_CLASS(QueryQueuerQueryEvaluatorIntegration) {

public:

		TEST_CLASS_INITIALIZE(setup) {
			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{ 1,2,3,4,5 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 2 },{ 1,2,3 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 4,5 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 2 },{  },{  },{ 4 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 1 },{ 2,3,4 },{ 1 } });

			pkb.insertToTable(USES_TABLE, 1, { { 1,2 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 2, { { 2 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 3, { { 2 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 4, { { 3 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 5, { { 3 },{ 1 } });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { {1}, {1,2,3,4,5}, {1} });

			pkb.insertToTable(MODIFIES_TABLE, 1, { { 3 },{ 1 } });
			pkb.insertToTable(MODIFIES_TABLE, 4, { { 2 },{ 1 } });

			pkb.insertToTable(CALLS_TABLE, 1, { { 2 },{  } });
			pkb.insertToTable(CALLS_TABLE, 2, { {  },{4} });
			pkb.insertToTable(CALLS_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 1, { { 2 } });
			pkb.insertToTable(CALLS_STAR_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(CALL_STATEMENT_TABLE, 4, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 1 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 1, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });

			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a", "a|b|+|c|+|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a", "d|e|+|" });
			pkb.insertToNameTable(VAR_TABLE, { "a" });
			pkb.insertToNameTable(VAR_TABLE, { "b" });
			pkb.insertToNameTable(VAR_TABLE, { "c" });
			pkb.insertToNameTable(VAR_TABLE, { "d" });
			pkb.insertToNameTable(VAR_TABLE, { "e" });
			pkb.insertToNameTable(PROC_TABLE, { "First" });
			pkb.insertToNameTable(PROC_TABLE, { "Second" });
			evaluator.setPKB(pkb);
			qq.setEvaluator(evaluator);
		}

		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateSingleQueryClause)
		{
			std::vector<QueryContent> vqc;
			QueryContent qc1;
			qc1.insertSelect(ASSIGN, "a", NONE);
			qc1.insertClause(Uses, ASSIGN, "a", VAR_IDENT, "c", false);

			vqc.push_back(qc1);
			std::vector<int> mapping;
			unordered_map<int, std::vector<int>> subQueryMapping;
			qq.setSubQueryMapping(subQueryMapping);
			qq.setQueryContent(vqc);
			list<string> result = qq.evaluateQueries();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc2;
			qc2.insertSelect(ASSIGN, "a", NONE);
			qc2.insertClause(Modifies, ASSIGN, "a", VAR_IDENT, "a", false);
			vqc.clear();
			vqc.push_back(qc2);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("3");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc3;
			qc3.insertSelect(STMT, "s", NONE);
			qc3.insertClause(Follows, STMT, "s", INTEGER, "3", false);
			vqc.clear();
			vqc.push_back(qc3);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("2");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc4;
			qc4.insertSelect(STMT, "s", NONE);
			qc4.insertClause(FollowsT, STMT, "s", INTEGER, "3", false);
			vqc.clear();
			vqc.push_back(qc4);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("2");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc5;
			qc5.insertSelect(STMT, "s", NONE);
			qc5.insertClause(Parent, STMT, "s", INTEGER, "3", false);
			vqc.clear();
			vqc.push_back(qc5);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("1");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc6;
			qc6.insertSelect(STMT, "s", NONE);
			qc6.insertClause(ParentT, STMT, "s", INTEGER, "2", false);
			vqc.clear();
			vqc.push_back(qc6);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("1");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc7;
			qc7.insertSelect(PROCEDURE, "p", NONE);
			qc7.insertClause(Calls, PROCEDURE, "p", PROC_IDENT, "Second", false);
			vqc.clear();
			vqc.push_back(qc7);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("First");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc8;
			qc8.insertSelect(PROCEDURE, "p", NONE);
			qc8.insertClause(CallsT, PROC_IDENT, "First", PROCEDURE, "p", false);
			vqc.clear();
			vqc.push_back(qc8);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("Second");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc9;
			qc9.insertSelect(PROCEDURE, "p1", NONE);
			qc9.insertSelect(PROCEDURE, "p2", NONE);
			qc9.insertClause(CallsT, PROCEDURE, "p1", PROCEDURE, "p2", false);
			vqc.clear();
			vqc.push_back(qc9);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("First Second");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc10;
			qc10.insertSelect(STMT, "s", NONE);
			qc10.insertClause(Next, STMT, "s", INTEGER, "3", false);
			vqc.clear();
			vqc.push_back(qc10);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("2");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc11;
			qc11.insertSelect(STMT, "s", NONE);
			qc11.insertClause(NextT, STMT, "s", INTEGER, "3", false);
			vqc.clear();
			vqc.push_back(qc11);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("3");
			expected.push_back("4");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc12;
			qc12.insertSelect(STMT, "s1", NONE);
			qc12.insertSelect(STMT, "s2", NONE);
			qc12.insertClause(Next, STMT, "s1", STMT, "s2", false);
			vqc.clear();
			vqc.push_back(qc12);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("1 2");
			expected.push_back("2 3");
			expected.push_back("3 4");
			expected.push_back("4 1");
			Assert::AreEqual(true, (result == expected));


			QueryContent qc13;
			qc13.insertSelect(ASSIGN, "a", NONE);
			qc13.insertClause(Affects, ASSIGN, "a", INTEGER, "2", false);
			vqc.clear();
			vqc.push_back(qc13);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("3");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc14;
			qc14.insertSelect(ASSIGN, "a", NONE);
			qc14.insertClause(AffectsT, ASSIGN, "a", INTEGER, "2", false);
			vqc.clear();
			vqc.push_back(qc14);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("2");
			expected.push_back("3");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc15;
			qc15.insertSelect(ASSIGN, "a", NONE);
			qc15.insertClause(AffectsT, ASSIGN, "a", INTEGER, "2", false);
			vqc.clear();
			vqc.push_back(qc15);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("2");
			expected.push_back("3");
			Assert::AreEqual(true, (result == expected));

			QueryContent qc16;
			qc16.insertSelect(ASSIGN, "a1", NONE);
			qc16.insertSelect(ASSIGN, "a2", NONE);
			qc16.insertClause(Affects, ASSIGN, "a1", ASSIGN, "a2", false);
			vqc.clear();
			vqc.push_back(qc16);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected.clear();
			expected.push_back("2 3");
			expected.push_back("3 2");
			Assert::AreEqual(true, (result == expected));
		}

		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateSingleQueryPattern)
		{
			std::vector<QueryContent> vqc;
			QueryContent qc1;
			qc1.insertSelect(ASSIGN, "a", NONE);
			qc1.insertPattern(ASSIGN, "a", VAR_IDENT, "a", EXPR, "a|b|+|", false);

			vqc.push_back(qc1);
			std::vector<int> mapping;
			unordered_map<int, std::vector<int>> subQueryMapping;
			subQueryMapping.insert({ 1, mapping });
			qq.setSubQueryMapping(subQueryMapping);
			qq.setQueryContent(vqc);
			list<string> result = qq.evaluateQueries();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (result == expected));
		}

		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateSingleQueryWithClause)
		{
			std::vector<QueryContent> vqc;
			QueryContent qc1;
			qc1.insertSelect(INTEGER, "s", NONE);
			qc1.insertWithClause(STMT, "s", STMT_NO, INTEGER, "3", NONE, false);

			vqc.push_back(qc1);
			std::vector<int> mapping;
			unordered_map<int, std::vector<int>> subQueryMapping;
			subQueryMapping.insert({ 1, mapping });
			qq.setSubQueryMapping(subQueryMapping);
			qq.setQueryContent(vqc);
			list<string> result = qq.evaluateQueries();
			list<string> expected;
			expected.push_back("3");
			Assert::AreEqual(true, (result == expected));
		}

		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateSingleClauseSubQuery)
		{
			std::vector<QueryContent> vqc;
			QueryContent qc1;
			qc1.insertSelect(INTEGER, "s", NONE);
			qc1.insertClause(Uses, STMT, "s", VAR_IDENT, "", false);
			qc1.setChildren(1);
			QueryContent qc2;
			qc2.insertSelect(VARIABLE, "v", NONE);
			qc2.insertClause(Uses, INTEGER, "2", VARIABLE, "v", true);

			vqc.push_back(qc1);
			vqc.push_back(qc2);
			std::vector<int> mapping = { REPLACE_CLAUSE, 0, RIGHT_PARAM };
			unordered_map<int, std::vector<int>> subQueryMapping;
			subQueryMapping.insert({ 1, mapping });
			qq.setSubQueryMapping(subQueryMapping);
			qq.setQueryContent(vqc);
			list<string> result = qq.evaluateQueries();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			Assert::AreEqual(true, (result == expected));
		}

		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateDoubleClauseSubQuery)
		{
			std::vector<QueryContent> vqc;
			QueryContent qc1;
			qc1.insertSelect(INTEGER, "s", NONE);
			qc1.insertClause(Uses, STMT, "s", VAR_IDENT, "", false);
			qc1.insertClause(Uses, STMT, "s", VAR_IDENT, "", false);
			qc1.insertOperator(CLAUSE, OR);
			qc1.setChildren(1);
			qc1.setChildren(2);
			QueryContent qc2;
			qc2.insertSelect(VARIABLE, "v", NONE);
			qc2.insertClause(Uses, INTEGER, "2", VARIABLE, "v", true);
			QueryContent qc3;
			qc3.insertSelect(VARIABLE, "v", NONE);
			qc3.insertClause(Uses, INTEGER, "3", VARIABLE, "v", true);

			vqc.push_back(qc1);
			vqc.push_back(qc2);
			vqc.push_back(qc3);
			std::vector<int> mapping = { REPLACE_CLAUSE, 0, RIGHT_PARAM };
			unordered_map<int, std::vector<int>> subQueryMapping;
			subQueryMapping.insert({ 1, mapping });
			mapping = { REPLACE_CLAUSE, 1, RIGHT_PARAM };
			subQueryMapping.insert({ 2, mapping });
			qq.setSubQueryMapping(subQueryMapping);
			qq.setQueryContent(vqc);
			list<string> result = qq.evaluateQueries();
			list<string> expected;
			expected.push_back("1");
			expected.push_back("2");
			expected.push_back("3");
			Assert::AreEqual(true, (result == expected));
		}



		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateSingleNestedSingleClauseSubQuery)
		{
			std::vector<QueryContent> vqc;
			QueryContent qc1;
			qc1.insertSelect(VARIABLE, "v", NONE);
			qc1.insertClause(Uses, INTEGER, "", VARIABLE, "v", false);
			qc1.setChildren(1);
			QueryContent qc2;
			qc2.insertSelect(STMT, "s", NONE);
			qc2.insertClause(Uses, STMT, "s", VAR_IDENT, "", true);
			qc2.setChildren(2);
			QueryContent qc3;
			qc3.insertSelect(VARIABLE, "v", NONE);
			qc3.insertClause(Uses, PROCEDURE, "p", VARIABLE, "v", true);

			vqc.push_back(qc1);
			vqc.push_back(qc2);
			vqc.push_back(qc3);
			unordered_map<int, std::vector<int>> subQueryMapping;
			std::vector<int> mapping = { REPLACE_CLAUSE, 0, LEFT_PARAM };
			subQueryMapping.insert({ 1, mapping });
			mapping = { REPLACE_CLAUSE, 0, RIGHT_PARAM };
			subQueryMapping.insert({ 2, mapping });
			qq.setSubQueryMapping(subQueryMapping);
			qq.setQueryContent(vqc);
			list<string> result = qq.evaluateQueries();
			list<string> expected;
			expected.push_back("a");
			expected.push_back("b");
			expected.push_back("c");
			expected.push_back("d");
			expected.push_back("e");
			Assert::AreEqual(true, (result == expected));
		}
		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateSingleClauseDoubleParamSubQuery)
		{
			std::vector<QueryContent> vqc;
			QueryContent qc1;
			qc1.insertSelect(VARIABLE, "v", NONE);
			qc1.insertClause(Follows, INTEGER, "1", INTEGER, "1", false);

			vqc.push_back(qc1);
			qq.setQueryContent(vqc);
			list<string> result = qq.evaluateQueries();
			list<string> expected;
			Assert::AreEqual(true, (result == expected));


			qc1.setChildren(1);
			qc1.setChildren(2);

			QueryContent qc2;
			qc2.insertSelect(STMT, "s", NONE);
			qc2.insertClause(Uses, STMT, "s", VAR_IDENT, "a", true);
			QueryContent qc3;
			qc3.insertSelect(STMT, "s", NONE);
			qc3.insertClause(Uses, STMT, "s", VAR_IDENT, "d", true);

			vqc.push_back(qc2);
			vqc.push_back(qc3);
			unordered_map<int, std::vector<int>> subQueryMapping;
			std::vector<int> mapping = { REPLACE_CLAUSE, 0, LEFT_PARAM };
			subQueryMapping.insert({ 1, mapping });
			mapping = { REPLACE_CLAUSE, 0, RIGHT_PARAM };
			subQueryMapping.insert({ 2, mapping });
			qq.setSubQueryMapping(subQueryMapping);
			qq.setQueryContent(vqc);
			result = qq.evaluateQueries();
			expected;
			expected.push_back("a");
			expected.push_back("b");
			expected.push_back("c");
			expected.push_back("d");
			expected.push_back("e");
			Assert::AreEqual(true, (result == expected));
		}
	};
}
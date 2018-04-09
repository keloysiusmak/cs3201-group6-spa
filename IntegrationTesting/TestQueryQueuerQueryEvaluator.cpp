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
			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{ 1,2,3 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 4,5 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2 },{ 1 } });

			pkb.insertToTable(USES_TABLE, 1, { { 1 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 2, { { 1 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 3, { { 1 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 4, { { 2 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 5, { { 2 },{ 1 } });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { {1}, {1,2,3,4,5}, {1} });

			pkb.insertToTable(MODIFIES_TABLE, 1, { { 1,2 },{ 1 } });

			pkb.insertToNameTable(PATTERN_TABLE, { "a", "a|b|+|c|+|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a", "d|e|+|" });
			pkb.insertToNameTable(VAR_TABLE, { "a" });
			pkb.insertToNameTable(VAR_TABLE, { "b" });
			pkb.insertToNameTable(VAR_TABLE, { "c" });
			pkb.insertToNameTable(VAR_TABLE, { "d" });
			pkb.insertToNameTable(VAR_TABLE, { "e" });
			pkb.insertToNameTable(PROC_TABLE, { "First" });
			evaluator.setPKB(pkb);
			qq.setEvaluator(evaluator);
		}

		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateSingleQuery)
		{
			std::vector<QueryContent> vqc;
			QueryContent qc1;
			qc1.insertSelect(INTEGER, "s", NONE);
			qc1.insertClause(Uses, STMT, "s", VAR_IDENT, "", false);
			qc1.setChildren(1);
			QueryContent qc2;
			qc2.insertSelect(VARIABLE, "v", NONE);
			qc2.insertClause(Uses, INTEGER, "1", VARIABLE, "v", true);

			vqc.push_back(qc1);
			vqc.push_back(qc2);
			std::vector<int> mapping = {REPLACE_CLAUSE, 0, RIGHT_PARAM};
			unordered_map<int, std::vector<int>> subQueryMapping;
			subQueryMapping.insert({1, mapping });
			qq.setSubQueryMapping(subQueryMapping);
			qq.setQueryContent(vqc);
			list<string> result = qq.evaluateQueries();
			list<string> expected;
			expected.push_back("1");
			Assert::AreEqual(true, (result == expected));
		}

		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateDoubleQuery)
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
			qc2.insertClause(Uses, INTEGER, "1", VARIABLE, "v", true);
			QueryContent qc3;
			qc3.insertSelect(VARIABLE, "v", NONE);
			qc3.insertClause(Uses, INTEGER, "2", VARIABLE, "v", true);

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
			Assert::AreEqual(true, (result == expected));
		}



		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateSingleNestedQuery)
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
		TEST_METHOD(QueryQueuerQueryEvaluatorEvaluateDoubleParamQuery)
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
			qc3.insertClause(Uses, STMT, "s", VAR_IDENT, "c", true);

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
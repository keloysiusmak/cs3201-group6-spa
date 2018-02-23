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

namespace PKBEvaluatorIntegrationTesting
{
	PKB pkb;
	Evaluator evaluator;
	list<string> _variableID;

	TEST_CLASS(PKBAPIIntegration)
	{
	public:
		/*
		procedure a{
		1.	a = b;
		2.	while c {
		3.		b = a;
		4.		d = 7; }
		5.	if a then {
		6.		while e {
		7.			c = 4; }}
		else {
		8.		d = 1; }
		9.	e = 1; }

		procedure b{
		10.	a = b;
		11.	while a {
		12.		while b {
		13.			a = 1; }}
		*/

		TEST_CLASS_INITIALIZE(setup)
		{
			unordered_map<int, std::vector<std::vector<int>>> table0;
			table0.insert({ 1, { {1}, {2}, {1}, {1} } });
			table0.insert({ 2,{ { 1,2 },{ 3,1 },{ 2,4 },{ 2 } } });
			table0.insert({ 3,{ { 2 },{ 1 },{2},{ 1 } } });
			table0.insert({ 4,{ { 2 },{  },{ 4 },{ 1 } } });
			table0.insert({ 5,{ { 1,3,5 },{ 1, 5 },{ 3,4 },{ 3 } } });
			table0.insert({ 6,{ { 3,4 },{ 5 },{ 3 },{ 2 } } });
			table0.insert({ 7,{ { 4 },{  },{ 3 },{ 1 } } });
			table0.insert({ 8,{ { 5 }, {},{ 4 },{ 1 } } });
			table0.insert({ 9,{ { 1 },{},{ 5 },{ 1 } } });
			table0.insert({ 10,{ { 6 },{1},{ 2},{ 1 } } });
			table0.insert({ 11,{ { 6,7 },{ 1,2 },{ 1 },{ 2 } } });
			table0.insert({ 12,{ { 7,8 },{ 2 },{ 1 },{ 2 } } });
			table0.insert({ 13,{ { 8 },{ },{1 },{ 1 } } });

			unordered_map<int, std::vector<std::vector<int>>> table1;
			table1.insert({ 1, {{0},{1,2,5,9}, {1}} });
			table1.insert({ 2,{ { 2 },{ 3,4 },{ 2 } } });
			table1.insert({ 3,{ { 5 },{ 6 },{ 3 } } });
			table1.insert({ 4,{ { 6 },{ 7 },{ 2 } } });
			table1.insert({ 5,{ { 5 },{ 8 },{ 3 } } });
			table1.insert({ 6,{ { 0 },{ 10,11 },{ 2 } } });
			table1.insert({ 7,{ { 11 },{ 12 },{ 2 } } });
			table1.insert({ 8,{ { 12 },{ 13 },{ 2 } } });

			unordered_map<int, std::vector<std::vector<int>>> table2;
			table2.insert({ 1, {{1},{1,2,3,5},{1,2,3,4,5}} });
			table2.insert({ 2,{ { 6 },{ 1,2 },{ 1 } } });

			unordered_map<int, std::vector<std::vector<int>>> table3;
			table3.insert({ 1, {{2,3,5,12}, {1,2}} });
			table3.insert({ 2,{ {1,10,11,12},{ 1,2 } } });
			table3.insert({ 3,{ { 2,5,6,7 },{ 1 } } });
			table3.insert({ 5,{ {5,6,9},{1} } });

			unordered_map<int, std::vector<std::vector<int>>> table4;
			table4.insert({ 1,{ {1,10,11,12,13},{1,2} } });
			table4.insert({ 2,{ { 2,3 },{ 1 } } });
			table4.insert({ 3,{ { 5,6,7 },{ 1 } } });
			table4.insert({ 4,{ { 5,8 },{ 1 } } });
			table4.insert({ 5,{ { 9 },{ 1 } } });

			unordered_map<int, std::vector<std::vector<int>>> table6;
			table6.insert({ 1, {{8, 9, 13}} });
			table6.insert({ 4,{ { 7 } } });
			table6.insert({ 7,{ { 4 } } });

			unordered_map<int, string> table7;
			table7.insert({1, "a"});
			table7.insert({ 2, "b" });

			unordered_map<int, string> table8;
			table8.insert({ 1, "a" });
			table8.insert({ 2, "b" });
			table8.insert({ 3, "c" });
			table8.insert({ 4, "d" });
			table8.insert({ 5, "e" });

			pkb.tables[0] = table0;
			pkb.tables[1] = table1;
			pkb.tables[2] = table2;
			pkb.tables[3] = table3;
			pkb.tables[4] = table4;
			pkb.tables[6] = table6;
			pkb.nameTables[0] = table7;
			pkb.nameTables[1] = table8;

			evaluator.setPKB(pkb);
		}

		TEST_METHOD(PKBEvaluatorFollows)
		{
			QueryObject q;
			
			q.insertSelectStmt(VARIABLE, "s");
			q.insertClause(Parent, INTEGER, "1", STMT, "s");

			evaluator.setQueryObject(q);

			list<string> result = evaluator.evaluateQuery();
			list<string> expected;
			expected.push_back("2");
			Assert::AreEqual(true, (expected == result));

		}

	};
}
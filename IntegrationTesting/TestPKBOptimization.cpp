#include "stdafx.h"
#include "CppUnitTest.h"

#include "../SPA/PKB.h";
#include "../SPA/QueryOptimization.h";
#include "../SPA/QueryEvaluator.h";
#include "../SPA/Clause.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PKBOptimizationIntegrationTesting {
	PKB pkb;
	QueryOptimization optimization;
	QueryEvaluator evaluator;

	TEST_CLASS(PKBOptimizationIntegration) {
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

		TEST_CLASS_INITIALIZE(setup) {
			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1,2 },{ 3,1 },{ 2,4 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 2 },{},{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 1,3,5 },{ 1, 5 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3,4 },{ 5 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 4 },{},{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 5 },{},{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 9, { { 1 },{},{ 5 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 6,7 },{ 1,2 },{ 1 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 7,8 },{ 2 },{ 1 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{},{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2,5,9 },{ 1 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3,4 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 5 },{ 6 },{ 3 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 5 },{ 8 },{ 3 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 0 },{ 10,11 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 11 },{ 12 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 13 },{ 2 } });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{ 1,2,3,5 },{ 1,2,3,4,5 } });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 6 },{ 1,2 },{ 1 } });

			pkb.insertToTable(USES_TABLE, 1, { { 2,3,5,11 },{ 1,2 } });
			pkb.insertToTable(USES_TABLE, 2, { { 1,10,11, 12 },{ 1,2 } });
			pkb.insertToTable(USES_TABLE, 3, { { 2 },{ 1 } });
			pkb.insertToTable(USES_TABLE, 5, { { 5,6 },{ 1 } });

			pkb.insertToTable(MODIFIES_TABLE, 1, { { 1,10,11,12,13 },{ 1,2 } });
			pkb.insertToTable(MODIFIES_TABLE, 2, { { 2,3 },{ 1 } });
			pkb.insertToTable(MODIFIES_TABLE, 3, { { 5,6,7 },{ 1 } });
			pkb.insertToTable(MODIFIES_TABLE, 4, { { 5,8 },{ 1 } });
			pkb.insertToTable(MODIFIES_TABLE, 5, { { 9 },{ 1 } });

			pkb.insertToTable(CONST_TABLE, 1, { { 8, 9, 13 } });
			pkb.insertToTable(CONST_TABLE, 4, { { 7 } });
			pkb.insertToTable(CONST_TABLE, 7, { { 4 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3,5 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6,8 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7,9 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 9 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 11 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 11,13 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 12 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5,7 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 9, { { 6, 8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 10,12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 11,13 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });

			pkb.insertToNameTable(PATTERN_TABLE, { "a", "b|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "b", "a|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "d", "7|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "e" });
			pkb.insertToNameTable(PATTERN_TABLE, { "c", "4|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "d", "1|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "e", "1|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a", "b|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a" });
			pkb.insertToNameTable(PATTERN_TABLE, { "b" });
			pkb.insertToNameTable(PATTERN_TABLE, { "a", "1|" });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });

			pkb.insertToNameTable(VAR_TABLE, { "a" });
			pkb.insertToNameTable(VAR_TABLE, { "b" });
			pkb.insertToNameTable(VAR_TABLE, { "c" });
			pkb.insertToNameTable(VAR_TABLE, { "d" });
			pkb.insertToNameTable(VAR_TABLE, { "e" });

		}

		Param createParam(ParamType type, string value) {
			Param param;
			param.type = type;
			param.value = value;
			param.attribute = NONE;
			return param;
		};
	};
}
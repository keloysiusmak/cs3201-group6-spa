#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/PKB.h"
#include "../SPA/Pattern.h"
#include "../SPA/Constants.h";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PKBTest
{ 
	TEST_CLASS(PKBTest)
	{
	public:
		TEST_METHOD(PKBGetFromTable)
		{
			PKB pkb;

			std::vector<std::vector<int>> null;
			std::vector<std::vector<int>> data = { { 1 },{},{},{} };
			pkb.insertToTable(STATEMENT_TABLE, 1, data);

			Assert::AreEqual(true, (data == pkb.getFromTable(STATEMENT_TABLE, 1)));
			Assert::AreEqual(true, (null == pkb.getFromTable(STATEMENT_TABLE, 2)));
			Assert::AreEqual(true, (null == pkb.getFromTable(99999, 2)));
		}
		TEST_METHOD(PKBInsertToTable)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			
			/* Null Test */
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));
			
			int tableValuesCount;

			for (int i = 1; i < PATTERN_TABLE; i++) {
				switch (i) {


				case STATEMENT_TABLE:
					tableValuesCount = 4;
					break;
				case STATEMENT_LIST_TABLE:
					tableValuesCount = 3;
					break;
				case PROC_INFO_TABLE:
					tableValuesCount = 3;
					break;
				case USES_TABLE:
					tableValuesCount = 2;
					break;
				case MODIFIES_TABLE:
					tableValuesCount = 2;
					break;
				case CONST_TABLE:
					tableValuesCount = 1;
					break;
				case CALLS_TABLE:
					tableValuesCount = 2;
					break;
				case CALLS_STAR_TABLE:
					tableValuesCount = 1;
					break;
				case CALLS_INVERSE_TABLE:
					tableValuesCount = 1;
					break;
				case CALLS_STAR_INVERSE_TABLE:
					tableValuesCount = 1;
					break;
				case CALL_STATEMENT_TABLE:
					tableValuesCount = 1;
					break;
				case NEXT_TABLE:
					tableValuesCount = 1;
					break;
				case NEXT_INVERSE_TABLE:
					tableValuesCount = 1;
					break;
				case PROC_NAME_VAR_NAME_TABLE:
					tableValuesCount = 1;
					break;
				case PROC_NAME_CALL_NAME_TABLE:
					tableValuesCount = 1;
					break;
				case VAR_NAME_CALL_NAME_TABLE:
					tableValuesCount = 1;
					break;
				case STMT_NO_CONST_VALUE_TABLE:
					tableValuesCount = 1;
					break;
				case PATTERN_ASSIGN_VARIABLE_TABLE:
					tableValuesCount = 1;
					break;
				case PATTERN_WHILE_VARIABLE_TABLE:
					tableValuesCount = 1;
					break;
				case PATTERN_IF_VARIABLE_TABLE:
					tableValuesCount = 1;
					break;
				}
				data.clear();
				for (int j = 0; j < tableValuesCount; j++) {
					data.push_back({ 1 });
				}
				pkb.insertToTable(i, 1, data);
				Assert::AreEqual(tableValuesCount, static_cast<int>(pkb.getFromTable(i, 1).size()));
				if (i <= NEXT_INVERSE_TABLE) {
					Assert::AreEqual(true, (pkb.getFromTable(i, 1) == data));
				} else if (i <= NEXT_INVERSE_TABLE) {
					data = { { 1, 1 } };
					Assert::AreEqual(true, (pkb.getFromTable(i, 1) == data));
				}
			}
		}

		TEST_METHOD(PKBInsertToTableInvalid)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));

			int tableValuesCount;

			for (int i = 1; i < PROC_TABLE; i++) {
				switch (i) {

				case STATEMENT_TABLE:
					tableValuesCount = 4;
					break;
				case STATEMENT_LIST_TABLE:
					tableValuesCount = 3;
					break;
				case PROC_INFO_TABLE:
					tableValuesCount = 3;
					break;
				case USES_TABLE:
					tableValuesCount = 2;
					break;
				case MODIFIES_TABLE:
					tableValuesCount = 2;
					break;
				case CONST_TABLE:
					tableValuesCount = 1;
					break;
				case CALLS_TABLE:
					tableValuesCount = 2;
					break;
				case CALLS_STAR_TABLE:
					tableValuesCount = 1;
					break;
				case CALLS_INVERSE_TABLE:
					tableValuesCount = 1;
					break;
				case CALLS_STAR_INVERSE_TABLE:
					tableValuesCount = 1;
					break;
				case CALL_STATEMENT_TABLE:
					tableValuesCount = 1;
					break;
				case NEXT_TABLE:
					tableValuesCount = 1;
					break;
				case NEXT_INVERSE_TABLE:
					tableValuesCount = 1;
					break;
				case PROC_NAME_VAR_NAME_TABLE:
					tableValuesCount = 1;
					break;
				case PROC_NAME_CALL_NAME_TABLE:
					tableValuesCount = 1;
					break;
				case VAR_NAME_CALL_NAME_TABLE:
					tableValuesCount = 1;
					break;
				case STMT_NO_CONST_VALUE_TABLE:
					tableValuesCount = 1;
					break;
				case PATTERN_ASSIGN_VARIABLE_TABLE:
					tableValuesCount = 1;
					break;
				case PATTERN_WHILE_VARIABLE_TABLE:
					tableValuesCount = 1;
					break;
				case PATTERN_IF_VARIABLE_TABLE:
					tableValuesCount = 1;
					break;
				}
				data.clear();
				for (int j = 0; j <= tableValuesCount; j++) {
					data.push_back({ 1 });
				}
				pkb.insertToTable(i, 1, data);
				Assert::AreEqual(0, static_cast<int>(pkb.getFromTable(i, 1).size()));
				Assert::AreEqual(false, (pkb.getFromTable(i, 1) == data));
			}
		}

		TEST_METHOD(PKBInsertToTableMultiple)
		{
			PKB pkb;

			std::vector<std::vector<int>> data = { {1}, {}, {}, {1} };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));

			data = { {},{1},{},{} };
			std::vector<std::vector<int>> dataFinal = { {1}, {1}, {}, {1} };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == dataFinal));

			data = { {},{ 2 },{},{} };
			dataFinal = { { 1 },{ 1, 2 },{},{ 1 } };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == dataFinal));
		}

		TEST_METHOD(PKBInsertToTableInsertMultipleSameValue)
		{
			PKB pkb;

			std::vector<std::vector<int>> data = { { 1 },{},{},{ 1 } };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));

			data = { {},{ 1 },{},{} };
			pkb.insertToTable(1, 1, data);

			data = { {},{ 1 },{},{} };
			std::vector<std::vector<int>> dataFinal = { { 1 },{ 1 },{},{ 1 } };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == dataFinal));
		}

		TEST_METHOD(PKBInsertToTableMultipleInvalid)
		{
			PKB pkb;

			std::vector<std::vector<int>> data = { {1}, {}, {}, {1} };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == data));
			data = { {},{1},{},{} };
			std::vector<std::vector<int>> dataFinal = { {1}, {1}, {}, {1} };
			pkb.insertToTable(1, 1, data);
			Assert::AreEqual(true, (pkb.getFromTable(1, 1) == dataFinal));
		}

		TEST_METHOD(PKBInsertToNameTable)
		{
			PKB pkb;

			string data = "x";
			std::vector<string> null;

			for (int i = PATTERN_TABLE; i < PATTERN_TABLE + 3; i++) {
				/* Null Test */
				null.clear();
				Assert::AreEqual(true, (null == pkb.getFromNameTable(i, 1)));
				if (i > PATTERN_TABLE) {
					null = { "x" };
					Assert::AreEqual(true, (null == pkb.getFromNameTable(i, pkb.insertToNameTable(i, { data }))));
				}
				else {
					null = {"x", "x"};
					Assert::AreEqual(true, (null == pkb.getFromNameTable(i, pkb.insertToNameTable(i, { data, data }))));
				}
			}
		}

		TEST_METHOD(PKBInsertToResultTable)
		{
			PKB pkb;

			string data = "x";
			std::vector<string> null;

			for (int i = 1; i < RELATIONS_SIZE; i++) {
				/* Null Test */
				null.clear();
				Assert::AreEqual(0, pkb.getFromResultTable(static_cast<Relations>(i), 0, 7));
				pkb.insertToResultTable(static_cast<Relations>(i), 0, 7, 17);
				Assert::AreEqual(17, pkb.getFromResultTable(static_cast<Relations>(i), 0, 7));
				pkb.insertToResultTable(static_cast<Relations>(i), 0, 8, 18);
				Assert::AreEqual(18, pkb.getFromResultTable(static_cast<Relations>(i), 0, 8));
				pkb.insertToResultTable(static_cast<Relations>(i), 1, 8, 19);
				Assert::AreEqual(19, pkb.getFromResultTable(static_cast<Relations>(i), 1, 8));
				
			}
		}

		TEST_METHOD(PKBInsertToNameTableInvalid)
		{
			PKB pkb;

			std::vector<string> null;
			string data = "x";

			for (int i = PROC_TABLE; i < PROC_TABLE + 2; i++) {
				/* Null Test */
				Assert::AreEqual(true, { null == pkb.getFromNameTable(i, 1) });

				Assert::AreEqual(1, pkb.insertToNameTable(i, { data }));
				Assert::AreEqual(1, pkb.insertToNameTable(i, { data }));
			}
		}

		TEST_METHOD(PKBGetProcedureName)
		{
			PKB pkb;

			/* Null Test */
			std::vector<string> null;
			Assert::AreEqual(true, (null == pkb.getFromNameTable(PROC_TABLE, 1)));

			string data = "proc_x" ;

			int return_id;

			return_id = pkb.insertToNameTable(PROC_TABLE, { data });
			Assert::AreEqual(true, ("proc_x" == pkb.getProcedureName(return_id)));
		}

		TEST_METHOD(PKBGetVariableName)
		{
			PKB pkb;

			/* Null Test */
			std::vector<string> null;

			null = {};
			Assert::AreEqual(true, (null == pkb.getFromNameTable(VAR_TABLE, 1)));

			string data = "x";

			int return_id;

			return_id = pkb.insertToNameTable(VAR_TABLE, { data });
			Assert::AreEqual(true, ("x" == pkb.getVariableName(return_id)));
		}

		TEST_METHOD(PKBGetProcedureId)
		{
			PKB pkb;
			
			string data = "proc_x";

			pkb.insertToNameTable(PROC_TABLE, { data });
			Assert::AreEqual(1, pkb.getProcedureId("proc_x"));
			Assert::AreEqual(0, pkb.getProcedureId("PROC_X"));
		}

		TEST_METHOD(PKBGetVariableId)
		{
			PKB pkb;

			/* Null Test */
			std::vector<string> null;
			null = {};
			Assert::AreEqual(true, (null == pkb.getFromNameTable(VAR_TABLE, 1)));

			string data = "x";

			pkb.insertToNameTable(VAR_TABLE, { data });
			Assert::AreEqual(1, pkb.getVariableId("x"));
			Assert::AreEqual(0, pkb.getVariableId("X"));
		}

		TEST_METHOD(PKBGetAllVariables)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllVariables() == data));

			data.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) });
			data.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) });

			Assert::AreEqual(true, (pkb.getAllVariables() == data));
		}

		TEST_METHOD(PKBGetAllProcedures)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllProcedures() == data));

			data.push_back({ pkb.insertToNameTable(PROC_TABLE, {"a"}) });
			data.push_back({ pkb.insertToNameTable(PROC_TABLE, {"b"}) });

			Assert::AreEqual(true, (pkb.getAllProcedures() == data));
		}

		TEST_METHOD(PKBGetAllStatementsFromProcedure)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllStatementsFromProcedure(1) == data));

			pkb.insertToTable(PROC_INFO_TABLE, 1, { {1}, {}, {} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { {0}, {1, 2}, {1} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{3 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4 },{ 3 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 5,6 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 1, { {1}, {}, {}, {1} });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1,2 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{},{},{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 4 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 4 },{},{},{ 1 } });

			data = { {1}, {2},{ 3 },{ 4 },{ 5 },{ 6 } };
			Assert::AreEqual(true, (pkb.getAllStatementsFromProcedure(1) == data));
		}

		TEST_METHOD(PKBGetProcedureFromStatement)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(0, pkb.getProcedureFromStatement(1)[0][0]);

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{},{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1, 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4 },{ 3 } });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 5,6 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1,2 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{},{},{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 4 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 4 },{},{},{ 1 } });

			Assert::AreEqual(1, pkb.getProcedureFromStatement(1)[0][0]);
			Assert::AreEqual(1, pkb.getProcedureFromStatement(2)[0][0]);
			Assert::AreEqual(1, pkb.getProcedureFromStatement(3)[0][0]);
			Assert::AreEqual(1, pkb.getProcedureFromStatement(4)[0][0]);
			Assert::AreEqual(1, pkb.getProcedureFromStatement(5)[0][0]);
			Assert::AreEqual(1, pkb.getProcedureFromStatement(6)[0][0]);
		}

		TEST_METHOD(PKBGetAllStatements)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllStatements() == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			data = { { 1}, {2}, {3} };


			Assert::AreEqual(true, (pkb.getAllStatements() == data));
		}

		TEST_METHOD(PKBGetAllStatementsWithType)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(3) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 2 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 3 } });

			data = { { 1}, {3 } };
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(3) == data));

			data = { { 2 } };
			Assert::AreEqual(true, (pkb.getAllStatementsWithType(2) == data));
		}

		TEST_METHOD(PKBCheckStatementHasType)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(false, pkb.checkStatementHasType(1, 1));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 1 },{},{},{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 1 },{},{},{ 4 } });

			Assert::AreEqual(true, pkb.checkStatementHasType(1, 1));
			Assert::AreEqual(true, pkb.checkStatementHasType(2, 2));
			Assert::AreEqual(true, pkb.checkStatementHasType(3, 3));
			Assert::AreEqual(true, pkb.checkStatementHasType(4, 4));
			Assert::AreEqual(false, pkb.checkStatementHasType(5, 1));
			Assert::AreEqual(false, pkb.checkStatementHasType(2, 1));
		}

		TEST_METHOD(PKBGetWithProcNameVarName)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getWithProcNameVarName()));

			pkb.insertToTable(PROC_NAME_VAR_NAME_TABLE, 1, { {2} });
			pkb.insertToTable(PROC_NAME_VAR_NAME_TABLE, 2, { { 1 } });

			data = { {1,2}, {2,1} };
			Assert::AreEqual(true, (data == pkb.getWithProcNameVarName()));

		}

		TEST_METHOD(PKBGetWithProcNameCallProcName)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getWithProcNameCallProcName()));

			pkb.insertToTable(PROC_NAME_CALL_NAME_TABLE, 1, { { 4 } });
			pkb.insertToTable(PROC_NAME_CALL_NAME_TABLE, 5, { { 2 } });

			data = { { 1,4 },{ 5,2 } };
			Assert::AreEqual(true, (data == pkb.getWithProcNameCallProcName()));

		}

		TEST_METHOD(PKBGetWithVarNameCallProcName)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getWithVarNameCallProcName()));

			pkb.insertToTable(VAR_NAME_CALL_NAME_TABLE, 1, { { 4 } });
			pkb.insertToTable(VAR_NAME_CALL_NAME_TABLE, 5, { { 2 } });

			data = { { 1,4 },{ 5,2 } };
			Assert::AreEqual(true, (data == pkb.getWithVarNameCallProcName()));

		}

		TEST_METHOD(PKBGetWithStmtNoConstValue)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getWithStmtNoConstValue(0)));
			Assert::AreEqual(true, (data == pkb.getWithStmtNoConstValue(1)));
			Assert::AreEqual(true, (data == pkb.getWithStmtNoConstValue(2)));
			Assert::AreEqual(true, (data == pkb.getWithStmtNoConstValue(3)));
			Assert::AreEqual(true, (data == pkb.getWithStmtNoConstValue(4)));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 1,2 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 2,3,4 },{},{},{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{},{},{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 4,5 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 5 },{},{},{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 9, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{ 4,5 },{ 6 },{ 1 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 1, { { 1 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 2, { { 2 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 3, { { 3 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 4, { { 4 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 5, { { 5 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 6, { { 6 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 7, { { 7 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 8, { { 8 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 9, { { 9 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 10, { { 10 } });
			pkb.insertToTable(STMT_NO_CONST_VALUE_TABLE, 11, { { 11 } });

			data = { { 1,1 },{ 2,2 },{ 3,3 },{ 4,4 },{ 5,5 },{ 6,6 },{ 7, 7 },{ 8,8 },{ 9,9 },{ 10,10 },{ 11, 11 } };
			Assert::AreEqual(true, (pkb.getWithStmtNoConstValue(0) == data));

			data = { { 1,1 },{ 2,2 },{ 8,8 },{ 9,9 },{ 10,10 },{ 11, 11 } };
			Assert::AreEqual(true, (pkb.getWithStmtNoConstValue(1) == data));

			data = { { 3,3 },{ 6,6 } };
			Assert::AreEqual(true, (pkb.getWithStmtNoConstValue(2) == data));

			data = { { 4,4 } };
			Assert::AreEqual(true, (pkb.getWithStmtNoConstValue(3) == data));

			data = { { 5,5 },{ 7,7 } };
			Assert::AreEqual(true, (pkb.getWithStmtNoConstValue(4) == data));

		}

		TEST_METHOD(PKBGetPatternOneSyn)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> result;
			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 1,2 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 2,3,4 },{},{},{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{},{},{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 4,5 },{},{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 5 },{},{},{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 9, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{ 4,5 },{ 6 },{ 1 } });

			data = { {1}, {2}, {8}, {9}, {10}, {11} };
			result = pkb.getPatternOneSyn(ASSIGNMENT_TYPE);
			std::sort(result.begin(), result.end());
			Assert::AreEqual(true, (result == data));
			data = { {3}, {6} };
			result = pkb.getPatternOneSyn(WHILE_TYPE);
			std::sort(result.begin(), result.end());
			Assert::AreEqual(true, (result == data));
			data = { {4} };
			result = pkb.getPatternOneSyn(IF_TYPE);
			std::sort(result.begin(), result.end());
			Assert::AreEqual(true, (result == data));
		}

		TEST_METHOD(PKBGetPatternTwoSyn)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> result;
			pkb.insertToTable(PATTERN_ASSIGN_VARIABLE_TABLE, 1, { {6} });
			pkb.insertToTable(PATTERN_ASSIGN_VARIABLE_TABLE, 2, { { 6 } });
			pkb.insertToTable(PATTERN_ASSIGN_VARIABLE_TABLE, 8, { { 6 } });
			pkb.insertToTable(PATTERN_ASSIGN_VARIABLE_TABLE, 9, { { 6 } });
			pkb.insertToTable(PATTERN_ASSIGN_VARIABLE_TABLE, 10, { { 6 } });
			pkb.insertToTable(PATTERN_ASSIGN_VARIABLE_TABLE, 11, { { 6 } });
			pkb.insertToTable(PATTERN_WHILE_VARIABLE_TABLE, 3, { { 3, 6 } });
			pkb.insertToTable(PATTERN_WHILE_VARIABLE_TABLE, 6, { { 6 } });
			pkb.insertToTable(PATTERN_IF_VARIABLE_TABLE, 4, { { 3 } });
			pkb.insertToTable(PATTERN_IF_VARIABLE_TABLE, 4, { { 6 } });

			data = { { 1, 6 },{ 2, 6 },{ 8, 6 },{ 9, 6 },{ 10, 6 },{ 11, 6 } };
			result = pkb.getPatternTwoSyn(ASSIGNMENT_TYPE);
			std::sort(result.begin(), result.end());
			Assert::AreEqual(true, (result == data));
			data = { { 3, 3  },{ 3, 6 },{ 6, 6 } };
			result = pkb.getPatternTwoSyn(WHILE_TYPE);
			std::sort(result.begin(), result.end());
			Assert::AreEqual(true, (result == data));
			data = { { 4,3 }, {4,6} };
			result = pkb.getPatternTwoSyn(IF_TYPE);
			std::sort(result.begin(), result.end());
			Assert::AreEqual(true, (result == data));
		}

		TEST_METHOD(PKBFollows)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;

			/* Null Tests */
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(2)));
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(1)));

			pkb.insertToTable(1, 1, { {1}, {}, {}, {1} });
			pkb.insertToTable(1, 2, { {1}, {}, {}, {1} });

			/* Invalid Tests */
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(2)));
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(1)));

			pkb.insertToTable(2, 1, { {0}, {1, 2}, {1} });

			data = { { 1 } };
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(2)));
			data = { { 2 } };
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(1)));
			data.clear();
			Assert::AreEqual(true, (data == pkb.getFollowsAfter(2)));
			Assert::AreEqual(true, (data == pkb.getFollowsBefore(0)));
		}

		TEST_METHOD(PKBFollowsStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{ 1 } });

			std::vector<std::vector<int>> dataBeforeStar = { {1}, {2} };
		std::vector<std::vector<int>> dataAfterStar = { {2}, {3} };
			std::vector<std::vector<int>> nullData;
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(1) == nullData));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(3) == nullData));
			Assert::AreEqual(true, (pkb.getFollowsBeforeStar(3) == dataBeforeStar));
			Assert::AreEqual(true, (pkb.getFollowsAfterStar(1) == dataAfterStar));
		}

		TEST_METHOD(PKBCheckFollows)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 2));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 2 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 2));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2 },{ 1 } });
			pkb.insertToTable(2, 2, { { 0 },{ 3 },{ 1 } });

			Assert::AreEqual(true, pkb.checkFollows(1, 2));
			Assert::AreEqual(false, pkb.checkFollows(2, 1));
			Assert::AreEqual(false, pkb.checkFollows(1, 3));
			Assert::AreEqual(false, pkb.checkFollows(3, 1));
		}

		TEST_METHOD(PKBCheckFollowsStar)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 3));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 2 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(false, pkb.checkFollows(1, 3));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3 },{ 1 } });
			pkb.insertToTable(2, 2, { { 0 },{ 4 },{ 1 } });

			Assert::AreEqual(true, pkb.checkFollowsStar(1, 3));
			Assert::AreEqual(true, pkb.checkFollowsStar(2, 3));
			Assert::AreEqual(false, pkb.checkFollowsStar(3, 1));
			Assert::AreEqual(false, pkb.checkFollowsStar(3, 2));
			Assert::AreEqual(false, pkb.checkFollowsStar(1, 4));
			Assert::AreEqual(false, pkb.checkFollowsStar(4, 1));
		}

		TEST_METHOD(PKBGetAllFollows)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollows().size()));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 5, { { 2 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollows().size()));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{ 1 } });
			pkb.insertToTable(2, 2, { { 0 },{ 5 },{ 1 } });

			std::vector<std::vector<int>> data = { {1,2}, {2,3}, {3,4} };
			Assert::AreEqual(true, (pkb.getAllFollows() == data));
		}

		TEST_METHOD(PKBGetAllFollowsStar)
		{
			PKB pkb;

			/* Null Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollowsStar().size()));

			pkb.insertToTable(1, 1, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(0, static_cast<int>(pkb.getAllFollowsStar().size()));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{ 1 } });
			
			std::vector<std::vector<int>> follows = pkb.getAllFollowsStar();
			std::vector<std::vector<int>> data = { {1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4} };
			for (int i = 0; i < follows[0].size(); i++) {
				Assert::AreEqual(true, (follows[0][i] == data[0][i]));
				i++;
			}

		}

		TEST_METHOD(PKBParent)
		{
			PKB pkb;

			/* Null Test */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (data == pkb.getParent(2)));
			Assert::AreEqual(true, (data == pkb.getParent(1)));
			Assert::AreEqual(true, (pkb.getChildren(1) == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (data == pkb.getParent(2)));
			Assert::AreEqual(true, (data == pkb.getParent(1)));
			Assert::AreEqual(true, (pkb.getChildren(1) == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1,4 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			data = { { 1 } };
			Assert::AreEqual(true, (data == pkb.getParent(2)));
			data = { { 1 } };
			Assert::AreEqual(true, (data == pkb.getParent(3)));
			data = { { 0 } };
			Assert::AreEqual(true, (data == pkb.getParent(1)));
			data = { { 4 } };
			Assert::AreEqual(true, (data == pkb.getParent(5)));

			std::vector<std::vector<int>> result;
			result = { {2}, {3} };
			Assert::AreEqual(true, ( result == pkb.getChildren(1)));
			result = { {5} };
			Assert::AreEqual(true, (result == pkb.getChildren(4)));
			data.clear();
			Assert::AreEqual(true, (pkb.getChildren(2) == data));
		}

		TEST_METHOD(PKBParentStar)
		{
			PKB pkb;

			/* Null Test */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getParentStar(3) == data));
			Assert::AreEqual(true, (pkb.getParentStar(2) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 5, { { 1, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 6, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getParentStar(3) == data));
			Assert::AreEqual(true, (pkb.getParentStar(2) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1, 5 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4 },{ 3 } });
			pkb.insertToTable(2, 4, { { 5 },{ 6 },{ 2 } });

			std::vector<std::vector<int>> parentStar1 = { { 1 } };
			std::vector<std::vector<int>> parentStar2 = { { 5 } };
			std::vector<std::vector<int>> childStar1 = { { 2}, {3}, {4} };
			std::vector<std::vector<int>> childStar2 = { { 6 } };
			Assert::AreEqual(true, (pkb.getParentStar(1) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(3) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(2) == data));
			Assert::AreEqual(true, (pkb.getChildrenStar(4) == data));
			Assert::AreEqual(true, (pkb.getParentStar(3) == parentStar1));
			Assert::AreEqual(true, (pkb.getParentStar(6) == parentStar2));
			Assert::AreEqual(true, (pkb.getParentStar(2) == parentStar1));
			Assert::AreEqual(true, (pkb.getChildrenStar(1) == childStar1));
			Assert::AreEqual(true, (pkb.getChildrenStar(5) == childStar2));
			Assert::AreEqual(true, (pkb.getChildrenStar(2) == data));
		}

		TEST_METHOD(PKBCheckParent)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkParent(1, 2));
			Assert::AreEqual(false, pkb.checkParent(2, 1));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 1, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(false, pkb.checkParent(1, 2));
			Assert::AreEqual(false, pkb.checkParent(2, 1));

			pkb.insertToTable(2, 1, { { 0 },{ 1,4 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			Assert::AreEqual(true, pkb.checkParent(1, 2));
			Assert::AreEqual(true, pkb.checkParent(1, 3));
			Assert::AreEqual(true, pkb.checkParent(4, 5));
			Assert::AreEqual(false, pkb.checkParent(2, 1));
			Assert::AreEqual(false, pkb.checkParent(3, 1));
			Assert::AreEqual(false, pkb.checkParent(3, 2));
			Assert::AreEqual(false, pkb.checkParent(2, 3));
		}

		TEST_METHOD(PKBCheckParentStar)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkParentStar(1, 2));
			Assert::AreEqual(false, pkb.checkParentStar(1, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 2));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 3, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Tests */
			Assert::AreEqual(false, pkb.checkParentStar(1, 2));
			Assert::AreEqual(false, pkb.checkParentStar(1, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 2));

			pkb.insertToTable(2, 1, { { 0 },{ 1 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3, 4 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			
			Assert::AreEqual(true, pkb.checkParentStar(1, 2));
			Assert::AreEqual(true, pkb.checkParentStar(1, 3));
			Assert::AreEqual(true, pkb.checkParentStar(1, 4));
			Assert::AreEqual(true, pkb.checkParentStar(1, 5));
			Assert::AreEqual(true, pkb.checkParentStar(4, 5));
			Assert::AreEqual(false, pkb.checkParentStar(2, 3));
			Assert::AreEqual(false, pkb.checkParentStar(2, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 1));
			Assert::AreEqual(false, pkb.checkParentStar(3, 2));
			Assert::AreEqual(false, pkb.checkParentStar(4, 1));
			Assert::AreEqual(false, pkb.checkParentStar(4, 2));
			Assert::AreEqual(false, pkb.checkParentStar(4, 3));
		}

		TEST_METHOD(PKBGetAllParent)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			
			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllParent() == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 3, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getAllParent() == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3, 4 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			data = { {1, 2}, {1, 3}, {1, 4}, {4, 5} };
			Assert::AreEqual(true, (pkb.getAllParent() == data));
		}

		TEST_METHOD(PKBGetAllParentStar)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			/* Null Test */
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));

			pkb.insertToTable(1, 1, { { 1, 2, 3 },{},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(1, 3, { { 3 },{},{},{ 1 } });
			pkb.insertToTable(1, 4, { { 3, 4 },{},{},{ 2 } });
			pkb.insertToTable(1, 5, { { 4 },{},{},{ 1 } });

			/* Invalid Test */
			Assert::AreEqual(true, (pkb.getAllParentStar() == data));

			pkb.insertToTable(2, 1, { { 0 },{ 1 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 3, 4 },{ 3 } });
			pkb.insertToTable(2, 4, { { 4 },{ 5 },{ 2 } });

			data.push_back({ 1,2 });
			data.push_back({ 1,3 });
			data.push_back({ 1,4 });
			data.push_back({ 1,5 });
			data.push_back({ 4, 5 });

			Assert::AreEqual(true, (pkb.getAllParentStar() == data));
		}

		TEST_METHOD(PKBUsesStatement)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> null_data;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{1},{},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{  },{},{ 1 } });
			pkb.insertToTable(4, 1, { { 1 },{ } });
			pkb.insertToTable(4, 2, { { },{} });

			data = { { 1 } };

			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(2, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(1) == data));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(2) == null_data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(2) == null_data));
		}

		TEST_METHOD(PKBUsesProcedure)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> data2;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data));

			pkb.insertToTable(3, 1, { {}, {1},{ } });
			pkb.insertToTable(3, 2, { {}, {1},{ } });
			pkb.insertToTable(4, 1, { {},{1,2} });

			data = { { 1 } };
			data2 = { { 1}, {2 }};

			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkProcedureUsesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkProcedureUsesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromUsesVariable(1) == data2));
		}

		TEST_METHOD(PKBUsesContainer)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> data2;
			std::vector<std::vector<int>> data3;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(3) == data2));

			pkb.insertToTable(1, 1, { { 1,2, 3 },{2,3,4,5},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 1 },{2},{ 2 },{ 1 } });
			pkb.insertToTable(1, 6, { { 1,4 },{ 6, 7 },{  },{ 2 } });
			pkb.insertToTable(1, 7, { { 4 },{ 7 },{  },{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 6 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4, 5 },{ 3 } });
			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(4, 2, { { 1, 2 },{} });
			pkb.insertToTable(4, 3, { { 1, 3 },{} });
			pkb.insertToTable(4, 4, { { 1, 4 },{} });
			pkb.insertToTable(4, 5, { { 1, 5 },{} });
			pkb.insertToTable(4, 6, { { 6 },{} });
			pkb.insertToTable(4, 7, { { 6, 7 },{} });

			data = { { 2},{3},{4},{5} };
			data2 = { { 1},{3 }};
			data3 = { { 6}, {7 }};

			Assert::AreEqual(true, pkb.checkStatementUsesVariable(2, 2));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(1, 3));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(6, 6));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(6, 7));
			Assert::AreEqual(false, pkb.checkStatementUsesVariable(7, 6));
			Assert::AreEqual(true, pkb.checkStatementUsesVariable(7, 7));
			Assert::AreEqual(true, (pkb.getUsesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(3) == data2));
			Assert::AreEqual(true, (pkb.getStatementsFromUsesVariable(7) == data3));
		}

		TEST_METHOD(PKBGetAllUsesStatement)
		{
			PKB pkb;
			std::vector<std::vector<int>> stmts;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ 2 },{ 2 },{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{ 3 },{ 3 },{ 1 } });
			pkb.insertToTable(1, 3, { { 1 },{ 4 },{ 4 },{ 1 } });
			pkb.insertToTable(1, 4, { { 1 },{ 5 },{ 5 },{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 2, 3, 4 },{ 2,3,4,5 },{ 2,3,4,5 },{ 1 } });
			pkb.insertToTable(4, 2, { { 1 },{} });
			pkb.insertToTable(4, 3, { { 2 },{} });
			pkb.insertToTable(4, 4, { { 3 },{} });
			pkb.insertToTable(4, 5, { { 4 },{} });
			
			stmts.push_back({1,2});
			stmts.push_back({ 2,3 });
			stmts.push_back({ 3,4 });
			stmts.push_back({ 4,5 });

			pkb.insertToNameTable(VAR_TABLE, { "a" });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 2 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 3 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 4 });

			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllUsesContainer)
		{
			PKB pkb;
			std::vector<std::vector<int>> stmts;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));

			pkb.insertToTable(1, 1, { { 1,2, 3 },{2, 3, 4, 5},{},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{ 2 },{ 2 },{ 1 } });
			pkb.insertToTable(1, 3, { { 2 },{ 3 },{ 3 },{ 1 } });
			pkb.insertToTable(1, 4, { { 3 },{ 4 },{ 4 },{ 1 } });
			pkb.insertToTable(1, 5, { { 3 },{ 5 },{ 5 },{ 1 } });
			pkb.insertToTable(1, 6, { { 1,4 },{ 6, 7 },{},{ 2 } });
			pkb.insertToTable(1, 7, { { 4 },{ 7 }, {}, { 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 6 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4, 5 },{ 3 } });
			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(4, 2, { { 1, 2 },{} });
			pkb.insertToTable(4, 3, { { 1, 3 },{} });
			pkb.insertToTable(4, 4, { { 1, 4 },{} });
			pkb.insertToTable(4, 5, { { 1, 5 },{} });
			pkb.insertToTable(4, 6, { { 6 },{} });
			pkb.insertToTable(4, 7, { { 6, 7 },{} });
			
			stmts.push_back({ 1, 2 });
			stmts.push_back({ 1, 3 });
			stmts.push_back({ 1, 4 });
			stmts.push_back({ 1, 5 });
			stmts.push_back({ 2, 2 });
			stmts.push_back({ 3, 3 });
			stmts.push_back({ 4, 4 });
			stmts.push_back({ 5, 5 });
			stmts.push_back({ 6, 6 });
			stmts.push_back({ 6, 7 });
			stmts.push_back({ 7, 7 });

			pkb.insertToNameTable(VAR_TABLE, { "a" });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) ,1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) ,2 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 3 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 4 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 5 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"f"}) , 6 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"g"}) , 6 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"g"}) , 7 });

			Assert::AreEqual(true, (pkb.getAllStatementUsesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableUsesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllUsesProcedure)
		{
			PKB pkb;
			std::vector<std::vector<int>> procs;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ 1 },{},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{ 1 },{},{ 1 } });
			pkb.insertToTable(3, 1, { { 1},{1}, {} });
			pkb.insertToTable(3, 2, { { 2},{ 1 },{} });
			pkb.insertToTable(4, 1, { {}, {1, 2} });

			procs.push_back({ pkb.insertToNameTable(PROC_TABLE, {"abc"}), 1 });
			procs.push_back({ pkb.insertToNameTable(PROC_TABLE, {"abd"}), 1 });

			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 2 });

			Assert::AreEqual(true, (pkb.getAllProcedureUsesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableUsesProcedures() == vars));
		}

		TEST_METHOD(PKBModifiesStatement)
		{
			PKB pkb;

			std::vector<std::vector<int>> data;
			
			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));

			pkb.insertToTable(1, 1, { { 1 },{},{1},{ 1 } });
			pkb.insertToTable(5, 1, { { 1 },{} });

			data = { { 1 } };

			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(1) == data));
		}

		TEST_METHOD(PKBModifiesContainer)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> data2;
			std::vector<std::vector<int>> data3;
			
			/* Null Tests */
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 1));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(1, 3));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(3) == data2));

			pkb.insertToTable(1, 1, { { 1,2, 3 },{},{2,3,4,5},{ 3 } });
			pkb.insertToTable(1, 2, { { 1 },{ 2 },{ 2 },{ 1 } });
			pkb.insertToTable(1, 6, { { 1,4 },{  },{7},{ 2 } });
			pkb.insertToTable(1, 7, { { 4 },{  },{ 7 },{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 6 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4, 5 },{ 3 } });
			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(5, 2, { { 1, 2 },{} });
			pkb.insertToTable(5, 3, { { 1, 3 },{} });
			pkb.insertToTable(5, 4, { { 1, 4 },{} });
			pkb.insertToTable(5, 5, { { 1, 5 },{} });
			pkb.insertToTable(5, 6, { {  },{} });
			pkb.insertToTable(5, 7, { { 6, 7 },{} });

			data = { { 2},{3},{4},{5 }};
			data2 = { { 1 }, {3 } };
			data3 = { { 6}, {7 }};

			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(2, 2));
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(1, 3));
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(6, 7));
			Assert::AreEqual(false, pkb.checkStatementModifiesVariable(7, 6));
			Assert::AreEqual(true, pkb.checkStatementModifiesVariable(7, 7));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromStatement(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(3) == data2));
			Assert::AreEqual(true, (pkb.getStatementsFromModifiesVariable(7) == data3));
		}

		TEST_METHOD(PKBModifiesProcedure)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;
			std::vector<std::vector<int>> data2;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(1, 1));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data));

			pkb.insertToTable(3, 1, { {},{ },{1} });
			pkb.insertToTable(3, 2, { {},{},{ 1 } });
			pkb.insertToTable(5, 1, { {},{ 1, 2 } });

			data = { { 1 } };
			data2 = { { 1}, {2 }};

			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(1, 1));
			Assert::AreEqual(true, pkb.checkProcedureModifiesVariable(2, 1));
			Assert::AreEqual(false, pkb.checkProcedureModifiesVariable(1, 2));
			Assert::AreEqual(true, (pkb.getModifiesVariablesFromProcedure(1) == data));
			Assert::AreEqual(true, (pkb.getProceduresFromModifiesVariable(1) == data2));
		}

		TEST_METHOD(PKBGetAllModifiesStatement)
		{
			PKB pkb;
			std::vector<std::vector<int>> stmts;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
			
			pkb.insertToTable(1, 1, { { 1 },{  },{1},{ 1 } });
			pkb.insertToTable(1, 2, { { 1 },{},{ 2 },{ 1 } });
			pkb.insertToTable(5, 1, { { 1 },{} });
			pkb.insertToTable(5, 2, { { 1 },{} });

			stmts.push_back({ 1, 1 });
			stmts.push_back({ 2, 2 });

			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) , 1 });

			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllModifiesContainer)
		{
			PKB pkb;
			std::vector<std::vector<int>> stmts;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));

			pkb = PKB();

			pkb.insertToTable(1, 1, { { 1,2, 3 },{2,3,4,5},{2,3,4,5},{ 3 } });
			pkb.insertToTable(1, 2, { { 2 },{ 2 },{ 2 },{ 1 } });
			pkb.insertToTable(1, 3, { { 2 },{ 3 },{ 3 },{ 1 } });
			pkb.insertToTable(1, 4, { { 3 },{ 4 },{ 4 },{ 1 } });
			pkb.insertToTable(1, 5, { { 3 },{ 5 },{ 5 },{ 1 } });
			pkb.insertToTable(1, 6, { { 1,4 },{  },{7},{ 2 } });
			pkb.insertToTable(1, 7, { { 4 },{ },{7},{ 1 } });
			pkb.insertToTable(2, 1, { { 0 },{ 1, 6 },{ 1 } });
			pkb.insertToTable(2, 2, { { 1 },{ 2, 3 },{ 3 } });
			pkb.insertToTable(2, 3, { { 1 },{ 4, 5 },{ 3 } });
			pkb.insertToTable(2, 4, { { 6 },{ 7 },{ 2 } });
			pkb.insertToTable(5, 2, { { 1, 2 },{} });
			pkb.insertToTable(5, 3, { { 1, 3 },{} });
			pkb.insertToTable(5, 4, { { 1, 4 },{} });
			pkb.insertToTable(5, 5, { { 1, 5 },{} });
			pkb.insertToTable(5, 6, { {  },{} });
			pkb.insertToTable(5, 7, { { 6, 7 },{} });

			stmts.push_back({ 1,2 });
			stmts.push_back({ 1,3 });
			stmts.push_back({ 1,4 });
			stmts.push_back({ 1,5 });
			stmts.push_back({ 2,2 });
			stmts.push_back({ 3,3 });
			stmts.push_back({ 4,4 });
			stmts.push_back({ 5,5 });
			stmts.push_back({ 6,7 });
			stmts.push_back({ 7,7 });

			pkb.insertToNameTable(VAR_TABLE, { "a" });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"b"}) , 2 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"c"}) , 3 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"d"}) , 4 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"e"}) , 5 });
			pkb.insertToNameTable(VAR_TABLE, { "f" });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"g"}) , 6 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"g"}) , 7 });

			Assert::AreEqual(true, (pkb.getAllStatementModifiesVariables() == stmts));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesStatements() == vars));
		}

		TEST_METHOD(PKBGetAllModifiesProcedure)
		{
			PKB pkb;

			std::vector<std::vector<int>> procs;
			std::vector<std::vector<int>> vars;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == vars));

			pkb.insertToTable(1, 1, { { 1 },{ },{1},{ 1 } });
			pkb.insertToTable(1, 2, { { 2 },{},{ 1 },{ 1 } });
			pkb.insertToTable(3, 1, { {},{ },{1} });
			pkb.insertToTable(3, 2, { {},{},{ 1 } });
			pkb.insertToTable(5, 1, { {},{ 1,2 } });


			procs.push_back({ pkb.insertToNameTable(PROC_TABLE, {"abc"}), 1 });
			procs.push_back({ pkb.insertToNameTable(PROC_TABLE, {"abd"}), 1 });

			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 1 });
			vars.push_back({ pkb.insertToNameTable(VAR_TABLE, {"a"}) , 2 });

			Assert::AreEqual(true, (pkb.getAllProcedureModifiesVariables() == procs));
			Assert::AreEqual(true, (pkb.getAllVariableModifiesProcedures() == vars));
		}

		TEST_METHOD(PKBGetNextBefore)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getNextBefore(3) == data));
			Assert::AreEqual(true, (pkb.getNextBefore(1) == data));

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 1 } });

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getNextBefore(1) == data));

			/* Valid Tests */
			data = { {1} };
			Assert::AreEqual(true, (pkb.getNextBefore(2) == data));
			Assert::AreEqual(true, (pkb.getNextBefore(3) == data));
		}

		TEST_METHOD(PKBGetNextAfter)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getNextAfter(2) == data));
			Assert::AreEqual(true, (pkb.getNextAfter(1) == data));

			pkb.insertToTable(NEXT_TABLE, 1, { { 2,3 } });

			/* Valid Tests */
			data = { { 2 }, {3} };
			Assert::AreEqual(true, (pkb.getNextAfter(1) == data));
			data.clear();
			Assert::AreEqual(true, (pkb.getNextAfter(2) == data));
		}

		TEST_METHOD(PKBGetNextBeforeStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getNextBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getNextBeforeStar(1) == data));

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getNextBeforeStar(1) == data));

			/* Valid Tests */
			data = { { 1 } };
			Assert::AreEqual(true, (pkb.getNextBeforeStar(2) == data));
			data = { { 1 }, {2} };
			Assert::AreEqual(true, (pkb.getNextBeforeStar(3) == data));
		}

		TEST_METHOD(PKBGetNextAfterStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getNextAfterStar(2) == data));
			Assert::AreEqual(true, (pkb.getNextAfterStar(1) == data));

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });

			/* Valid Tests */
			data = { { 2 },{ 3 } };
			Assert::AreEqual(true, (pkb.getNextAfterStar(1) == data));
			data = { { 3 } };
			Assert::AreEqual(true, (pkb.getNextAfterStar(2) == data));
		}

		TEST_METHOD(PKBGetAllNext)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllNext() == data));

			pkb.insertToTable(NEXT_TABLE, 1, { { 2,5 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3,4 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 1 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 1 } });

			/* Valid Tests */
			data = { { 1,2 },{ 1,5 }, {2,3}, {2,4}, {3,1}, {4,1} };
			Assert::AreEqual(true, (pkb.getAllNext() == data));
		}

		TEST_METHOD(PKBGetAllNextStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllNextStar() == data));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 }, {}, {}, {2} });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 2 },{},{},{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{},{},{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 1 } });

			/* Valid Tests */
			data = { {1,1}, { 1,2 },{1,3}, {2,1}, {2,2},{2,3}, {3,1}, {3,2}, {3,3} };
			Assert::AreEqual(true, (pkb.getAllNextStar() == data));
		}

		TEST_METHOD(PKBCheckNext)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkNext(1,2));

			pkb.insertToTable(NEXT_TABLE, 1, { {2} });
			pkb.insertToTable(NEXT_TABLE, 2, { {3} });

			/* Valid Tests */
			Assert::AreEqual(true, pkb.checkNext(1,2));
			Assert::AreEqual(true, pkb.checkNext(2, 3));
			Assert::AreEqual(false, pkb.checkNext(1, 3));
		}

		TEST_METHOD(PKBCheckNextStar)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkNextStar(1, 2));

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 2 } });

			/* Valid Tests */
			Assert::AreEqual(true, pkb.checkNextStar(1, 2));
			Assert::AreEqual(true, pkb.checkNextStar(1, 3));
			Assert::AreEqual(true, pkb.checkNextStar(1, 4));
			Assert::AreEqual(true, pkb.checkNextStar(2, 2));
			Assert::AreEqual(true, pkb.checkNextStar(2, 3));
			Assert::AreEqual(true, pkb.checkNextStar(2, 4));
			Assert::AreEqual(false, pkb.checkNextStar(1, 1));
			Assert::AreEqual(false, pkb.checkNextStar(3, 1));
		}

		TEST_METHOD(PKBGetCallsBefore)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallsBefore(3) == data));
			Assert::AreEqual(true, (pkb.getCallsBefore(1) == data));

			pkb.insertToTable(CALLS_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(CALLS_INVERSE_TABLE, 3, { { 1 } });

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getCallsBefore(1) == data));

			/* Valid Tests */
			data = { { 1 } };
			Assert::AreEqual(true, (pkb.getCallsBefore(2) == data));
			Assert::AreEqual(true, (pkb.getCallsBefore(3) == data));
		}

		TEST_METHOD(PKBGetCallsAfter)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallsAfter(2) == data));
			Assert::AreEqual(true, (pkb.getCallsAfter(1) == data));

			pkb.insertToTable(CALLS_TABLE, 1, { { 2,3 },{} });

			/* Valid Tests */
			data = { { 2 },{ 3 } };
			Assert::AreEqual(true, (pkb.getCallsAfter(1) == data));
			data.clear();
			Assert::AreEqual(true, (pkb.getCallsAfter(2) == data));
		}

		TEST_METHOD(PKBGetCallsBeforeStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(3) == data));
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(1) == data));

			pkb.insertToTable(CALLS_STAR_INVERSE_TABLE, 2, { { 1 } });
			pkb.insertToTable(CALLS_STAR_INVERSE_TABLE, 3, { { 1,2 } });

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(1) == data));

			/* Valid Tests */
			data = { { 1 } };
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(2) == data));
			data = { { 1 },{ 2 } };
			Assert::AreEqual(true, (pkb.getCallsBeforeStar(3) == data));
		}

		TEST_METHOD(PKBGetCallsAfterStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallsAfterStar(2) == data));
			Assert::AreEqual(true, (pkb.getCallsAfterStar(1) == data));

			pkb.insertToTable(CALLS_STAR_TABLE, 1, { { 2,3 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 2, { { 3 } });

			/* Valid Tests */
			data = { { 2 },{ 3 } };
			Assert::AreEqual(true, (pkb.getCallsAfterStar(1) == data));
			data = { { 3 } };
			Assert::AreEqual(true, (pkb.getCallsAfterStar(2) == data));
		}

		TEST_METHOD(PKBGetAllCalls)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllCalls() == data));

			pkb.insertToTable(CALLS_TABLE, 1, { { 2,5 },{} });
			pkb.insertToTable(CALLS_TABLE, 2, { { 3,4 },{} });
			pkb.insertToTable(CALLS_TABLE, 3, { { 1 },{} });
			pkb.insertToTable(CALLS_TABLE, 4, { { 1 },{} });

			/* Valid Tests */
			data = { { 1,2 },{ 1,5 },{ 2,3 },{ 2,4 },{ 3,1 },{ 4,1 } };
			Assert::AreEqual(true, (pkb.getAllCalls() == data));
		}

		TEST_METHOD(PKBGetAllCallsStar)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllCallsStar() == data));

			pkb.insertToTable(CALLS_STAR_TABLE, 1, { { 1,2,3 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 2, { { 1,2,3 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 3, { { 1,2,3 } });

			/* Valid Tests */
			data = { { 1,1 },{ 1,2 },{ 1,3 },{ 2,1 },{ 2,2 },{ 2,3 },{ 3,1 },{ 3,2 },{ 3,3 } };
			Assert::AreEqual(true, (pkb.getAllCallsStar() == data));
		}

		TEST_METHOD(PKBCheckCalls)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkCalls(1, 2));

			pkb.insertToTable(CALLS_TABLE, 1, { { 2 }, {} });
			pkb.insertToTable(CALLS_TABLE, 2, { { 3 }, {} });

			/* Valid Tests */
			Assert::AreEqual(true, pkb.checkCalls(1, 2));
			Assert::AreEqual(true, pkb.checkCalls(2, 3));
			Assert::AreEqual(false, pkb.checkCalls(1, 3));
		}

		TEST_METHOD(PKBCheckCallsStar)
		{
			PKB pkb;

			/* Null Tests */
			Assert::AreEqual(false, pkb.checkCallsStar(1, 2));

			pkb.insertToTable(CALLS_STAR_TABLE, 1, { { 2,3,4 } });
			pkb.insertToTable(CALLS_STAR_TABLE, 2, { { 2,3,4 } });

			/* Valid Tests */
			Assert::AreEqual(true, pkb.checkCallsStar(1, 2));
			Assert::AreEqual(true, pkb.checkCallsStar(1, 3));
			Assert::AreEqual(true, pkb.checkCallsStar(1, 4));
			Assert::AreEqual(true, pkb.checkCallsStar(2, 2));
			Assert::AreEqual(true, pkb.checkCallsStar(2, 3));
			Assert::AreEqual(true, pkb.checkCallsStar(2, 4));
			Assert::AreEqual(false, pkb.checkCallsStar(1, 1));
			Assert::AreEqual(false, pkb.checkCallsStar(3, 1));
		}

		TEST_METHOD(PKBGetCallStatementsCallingProcedure)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getCallStatementsCallingProcedure(2) == data));

			pkb.insertToTable(CALLS_TABLE, 2, { {}, { 2,3,4 } });

			/* Valid Tests */
			data = { {2}, {3}, {4} };
			Assert::AreEqual(true, (pkb.getCallStatementsCallingProcedure(2) == data));
			
		}

		TEST_METHOD(PKBGetProcedureCalledByCallStatement)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getProcedureCalledByCallStatement(1) == data));

			pkb.insertToTable(CALL_STATEMENT_TABLE, 1, { { 2 } });

			/* Valid Tests */
			data = { { 2 } };
			Assert::AreEqual(true, (pkb.getProcedureCalledByCallStatement(1) == data));

		}

		TEST_METHOD(PKBGetAllCallStatementsCallingProcedure)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllCallStatementsCallingProcedure() == data));

			pkb.insertToTable(CALLS_TABLE, 2, { {}, { 2,3,4 } });
			pkb.insertToTable(CALLS_TABLE, 3, { {},{ 5,6 } });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			/* Valid Tests */
			data = { {2,2}, {2,3}, {2,4}, {3,5}, {3,6} };
			Assert::AreEqual(true, (pkb.getAllCallStatementsCallingProcedure() == data));
			
		}

		TEST_METHOD(PKBGetAllProcedureCalledByCallStatement)
		{
			PKB pkb;

			/* Null Tests */
			std::vector<std::vector<int>> data;
			Assert::AreEqual(true, (pkb.getAllProcedureCalledByCallStatement() == data));

			pkb.insertToTable(CALL_STATEMENT_TABLE, 1, { { 2 } });
			pkb.insertToTable(CALL_STATEMENT_TABLE, 2, { { 3 } });

			/* Valid Tests */
			data = { { 1,2 }, {2,3} };
			Assert::AreEqual(true, (pkb.getAllProcedureCalledByCallStatement() == data));

		}
		
		TEST_METHOD(PKBGetStatementsWithPattern)
		{
			PKB pkb;

			Pattern p;
			Param ep;
			Param lp;
			Param rp;

			std::vector<std::vector<int>> null;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p) == null));

			//a = (b + c) * d + e * f + g; 
			//h = d + e;
			//i = e * f;
			pkb.insertToNameTable(VAR_TABLE, { "a" });
			pkb.insertToNameTable(VAR_TABLE, { "b" });
			pkb.insertToNameTable(VAR_TABLE, { "c" });
			pkb.insertToNameTable(VAR_TABLE, { "d" });
			pkb.insertToNameTable(VAR_TABLE, { "e" });
			pkb.insertToNameTable(VAR_TABLE, { "f" });
			pkb.insertToNameTable(VAR_TABLE, { "g" });
			pkb.insertToNameTable(VAR_TABLE, { "h" });
			pkb.insertToNameTable(VAR_TABLE, { "i" });
			pkb.insertToNameTable(VAR_TABLE, { "j" });
			pkb.insertToNameTable(VAR_TABLE, { "k" });
			pkb.insertToNameTable(VAR_TABLE, { "l" });
			pkb.insertToNameTable(VAR_TABLE, { "m" });
			pkb.insertToTable(STATEMENT_TABLE, 1, { {1}, {2,3,4,5,6,7}, {1}, {1} });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 4,5 },{ 8 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 1 },{ 5,6 },{ 9 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 1 },{ 10 },{  },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 1 },{ 11 },{  },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 1 },{ 12 },{},{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 1 },{ 13 },{},{ 2 } });
			pkb.insertToNameTable(PATTERN_TABLE, {"a", "b|c|+|d|*|e|f|*|g|+|"});
			pkb.insertToNameTable(PATTERN_TABLE, { "h", "d|e|+|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "i", "e|f|*|" });
			pkb.insertToNameTable(PATTERN_TABLE, { "j" });
			pkb.insertToNameTable(PATTERN_TABLE, { "k" });
			pkb.insertToNameTable(PATTERN_TABLE, { "l" });
			pkb.insertToNameTable(PATTERN_TABLE, { "m" });

			//a(_,_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p1(ep, lp, rp);
			std::vector<std::vector<int>> expected = { {1}, {2}, {3} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p1) == expected));

			//a(_,_"e*f"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p2(ep, lp, rp);
			expected = { {1},{3} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p2) == expected));

			//a(_,"e*f")
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p3(ep, lp, rp);
			expected = { { 3 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p3) == expected));

			//a(v, _)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p4(ep, lp, rp);
			expected = { {1,1}, {2,8}, { 3,9 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p4) == expected));

			//a("a", _)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "a";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p5(ep, lp, rp);
			expected = { { 1 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p5) == expected));

			//a(v,_"e*f"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p6(ep, lp, rp);
			expected = { {1, 1}, {3, 9} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p6) == expected));

			//a(v,"e*f")
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p7(ep, lp, rp);
			expected = { { 3, 9 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p7) == expected));

			//a("a", _"e*f"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "a";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p8(ep, lp, rp);
			expected = { { 1 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p8) == expected));

			//a("a", "e*f")
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "a";
			lp.attribute = NONE;

			rp.type = EXPR_EXACT;
			rp.value = "e|f|*|";
			rp.attribute = NONE;

			Pattern p9(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p9) == expected));

			//a(_, _"e*f+g"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|g|+|";
			rp.attribute = NONE;

			Pattern p10(ep, lp, rp);
			expected = { {1} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p10) == expected));

			//a("y", _"e*f+g"_)
			ep.type = ASSIGN;
			ep.value = "a";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "y";
			lp.attribute = NONE;

			rp.type = EXPR;
			rp.value = "e|f|*|g|+|";
			rp.attribute = NONE;

			Pattern p11(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p11) == expected));

			//ifs(_,_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p12(ep, lp, rp);
			expected = { {4}, {5} };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p12) == expected));

			//ifs(v,_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p13(ep, lp, rp);
			expected = { { 4, 10 },{ 5, 11 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p13) == expected));

			//ifs("j",_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "j";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p14(ep, lp, rp);
			expected = { { 4 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p14) == expected));

			//ifs("k",_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "k";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p15(ep, lp, rp);
			expected = { { 5 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p15) == expected));

			//ifs("l",_,_)
			ep.type = IF;
			ep.value = "ifs";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "l";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p16(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p16) == expected));

			//w(_,_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = ALL;
			lp.value = "_";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p17(ep, lp, rp);
			expected = { { 6 },{ 7 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p17) == expected));

			//w(v,_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = VARIABLE;
			lp.value = "v";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p18(ep, lp, rp);
			expected = { { 6, 12 },{ 7, 13 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p18) == expected));

			//w("l",_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "l";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p19(ep, lp, rp);
			expected = { { 6 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p19) == expected));

			//w("m",_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "m";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p20(ep, lp, rp);
			expected = { { 7 } };
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p20) == expected));

			//w("n",_)
			ep.type = WHILE;
			ep.value = "w";
			ep.attribute = NONE;

			lp.type = IDENT;
			lp.value = "n";
			lp.attribute = NONE;

			rp.type = ALL;
			rp.value = "_";
			rp.attribute = NONE;

			Pattern p21(ep, lp, rp);
			expected.clear();
			Assert::AreEqual(true, (pkb.getStatementsWithPattern(p21) == expected));

		}

		TEST_METHOD(PKBGetAffectsBefore)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			Assert::AreEqual(true, (pkb.getAffectsBefore(4) == data));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1,2,3,4 },{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{ 3,4,2,1 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3 },{},{ 2 },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 3 },{ 1,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 4 },{ 1,2 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 9, { { 5 },{},{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{},{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 5 },{ 1,2 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 14, { { 9 },{ 1,2 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 15, { { 5 },{ 1,2,3,4 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 16, { { 10 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 17, { { 11 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 18, { { 11 },{ 1,3,4 },{ 2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 19, { { 12 },{ 3,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 20, { { 12 },{ 1,4 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 21, { { 13 },{ 4,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 22, { { 13 },{ 4,3 },{ 2 },{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4,8 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 5 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 9, { { 10,11 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 13,14 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 15 } });
			pkb.insertToTable(NEXT_TABLE, 14, { { 15 } });

			pkb.insertToTable(NEXT_TABLE, 16, { { 17 } });
			pkb.insertToTable(NEXT_TABLE, 17, { { 18 } });
			pkb.insertToTable(NEXT_TABLE, 18, { { 19,16 } });
			pkb.insertToTable(NEXT_TABLE, 19, { { 20 } });
			pkb.insertToTable(NEXT_TABLE, 20, { { 21,18 } });
			pkb.insertToTable(NEXT_TABLE, 21, { { 22 } });
			pkb.insertToTable(NEXT_TABLE, 22, { { 20 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,7,8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 3 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 10, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 10,11 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 14, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 15, { { 13,14 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 16, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 17, { { 16 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 18, { { 17,20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 19, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 20, { { 19,22 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 21, { { 20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 22, { { 21 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4,5,6,7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 8 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 0 },{ 9,12,15 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 9 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 9 },{ 13 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 14 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 9, { { 12 },{ 13 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 10, { { 0 },{ 16 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 11, { { 16 },{ 17,18 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 12, { { 18 },{ 19,20 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 13, { { 20 },{ 21,22 },{} });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 5 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 10 },{},{} });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			data = { {8}, {7}, {5}, { 1 } };
			Assert::AreEqual(true, (pkb.getAffectsBefore(8) == data));

			data = { { 14 }, {11}, {10}, {13} };
			Assert::AreEqual(true, (pkb.getAffectsBefore(15) == data));

			data = { { 11}, {10} };
			Assert::AreEqual(true, (pkb.getAffectsBefore(13) == data));

			data = { { 11 },{ 10 } };
			Assert::AreEqual(true, (pkb.getAffectsBefore(14) == data));

			data = { { 22 },{ 21 },{19} };
			Assert::AreEqual(true, (pkb.getAffectsBefore(19) == data));
			
			data = { { 22 },{ 19 } };
			Assert::AreEqual(true, (pkb.getAffectsBefore(21) == data));
		}

		TEST_METHOD(PKBGetAffectsAfter)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			Assert::AreEqual(true, (pkb.getAffectsAfter(1) == data));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1,2,3,4 },{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{ 3,4,2,1 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3 },{},{ 2 },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 3 },{ 1,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 4 },{ 1,2 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 9, { { 5 },{},{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{},{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 5 },{ 1,2 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 14, { { 9 },{ 1,2 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 15, { { 5 },{ 1,2,3,4 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 16, { { 10 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 17, { { 11 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 18, { { 11 },{ 1,3,4 },{ 2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 19, { { 12 },{ 3,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 20, { { 12 },{ 1,4 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 21, { { 13 },{ 4,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 22, { { 13 },{ 4,3 },{ 2 },{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4,8 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 5 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 9, { { 10,11 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 13,14 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 15 } });
			pkb.insertToTable(NEXT_TABLE, 14, { { 15 } });

			pkb.insertToTable(NEXT_TABLE, 16, { { 17 } });
			pkb.insertToTable(NEXT_TABLE, 17, { { 18 } });
			pkb.insertToTable(NEXT_TABLE, 18, { { 19,16 } });
			pkb.insertToTable(NEXT_TABLE, 19, { { 20 } });
			pkb.insertToTable(NEXT_TABLE, 20, { { 21,18 } });
			pkb.insertToTable(NEXT_TABLE, 21, { { 22 } });
			pkb.insertToTable(NEXT_TABLE, 22, { { 20 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,7,8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 3 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 10, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 10,11 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 14, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 15, { { 13,14 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 16, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 17, { { 16 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 18, { { 17,20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 19, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 20, { { 19,22 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 21, { { 20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 22, { { 21 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4,5,6,7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 8 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 0 },{ 9,12,15 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 9 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 9 },{ 13 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 14 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 9, { { 12 },{ 13 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 10, { { 0 },{ 16 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 11, { { 16 },{ 17,18 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 12, { { 18 },{ 19,20 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 13, { { 20 },{ 21,22 },{} });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 5 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 10 },{},{} });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			data = { { 8 } , {4}, {5} };
			Assert::AreEqual(true, (pkb.getAffectsAfter(1) == data));

			data = { { 14 }, {15}, {13} };
			Assert::AreEqual(true, (pkb.getAffectsAfter(10) == data));

			data = { { 14 },{ 15 },{ 13 } };
			Assert::AreEqual(true, (pkb.getAffectsAfter(11) == data));

			data = { { 15} };
			Assert::AreEqual(true, (pkb.getAffectsAfter(13) == data));

			data.clear();
			Assert::AreEqual(true, (pkb.getAffectsAfter(17) == data));

			data = { { 17 }, {19}, {21} };
			Assert::AreEqual(true, (pkb.getAffectsAfter(19) == data));

			data = { { 22 }, {19} };
			Assert::AreEqual(true, (pkb.getAffectsAfter(21) == data));


		}

		TEST_METHOD(PKBGetAffectsBeforeStar)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			Assert::AreEqual(true, (pkb.getAffectsBeforeStar(4) == data));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1,2,3,4 },{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{ 3,4,2,1 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3 },{},{ 2 },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 3 },{ 1,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 4 },{ 1,2 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 9, { { 5 },{},{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{},{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 5 },{ 1,2 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 14, { { 9 },{ 1,2 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 15, { { 5 },{ 1,2,3,4 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 16, { { 10 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 17, { { 11 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 18, { { 11 },{ 1,3,4 },{ 2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 19, { { 12 },{ 3,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 20, { { 12 },{ 1,4 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 21, { { 13 },{ 4,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 22, { { 13 },{ 4,3 },{ 2 },{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4,8 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 5 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 9, { { 10,11 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 13,14 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 15 } });
			pkb.insertToTable(NEXT_TABLE, 14, { { 15 } });

			pkb.insertToTable(NEXT_TABLE, 16, { { 17 } });
			pkb.insertToTable(NEXT_TABLE, 17, { { 18 } });
			pkb.insertToTable(NEXT_TABLE, 18, { { 19,16 } });
			pkb.insertToTable(NEXT_TABLE, 19, { { 20 } });
			pkb.insertToTable(NEXT_TABLE, 20, { { 21,18 } });
			pkb.insertToTable(NEXT_TABLE, 21, { { 22 } });
			pkb.insertToTable(NEXT_TABLE, 22, { { 20 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,7,8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 3 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 10, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 10,11 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 14, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 15, { { 13,14 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 16, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 17, { { 16 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 18, { { 17,20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 19, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 20, { { 19,22 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 21, { { 20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 22, { { 21 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4,5,6,7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 8 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 0 },{ 9,12,15 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 9 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 9 },{ 13 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 14 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 9, { { 12 },{ 13 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 10, { { 0 },{ 16 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 11, { { 16 },{ 17,18 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 12, { { 18 },{ 19,20 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 13, { { 20 },{ 21,22 },{} });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 5 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 10 },{},{} });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			data = { { 8 },{ 7 },{ 5 },{4}, { 1 } };
			Assert::AreEqual(true, (pkb.getAffectsBeforeStar(8) == data));

			data = { { 14 },{ 11 },{ 10 },{ 13 } };
			Assert::AreEqual(true, (pkb.getAffectsBeforeStar(15) == data));

			data = { { 11 },{ 10 } };
			Assert::AreEqual(true, (pkb.getAffectsBeforeStar(13) == data));

			data = { { 11 },{ 10 } };
			Assert::AreEqual(true, (pkb.getAffectsBeforeStar(14) == data));

			data = { { 22 },{ 21 },{ 19 } };
			Assert::AreEqual(true, (pkb.getAffectsBeforeStar(19) == data));

			data = { { 22 },{ 21 } ,{ 19 } };
			Assert::AreEqual(true, (pkb.getAffectsBeforeStar(21) == data));
		}

		TEST_METHOD(PKBGetAffectsAfterStar)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			Assert::AreEqual(true, (pkb.getAffectsAfterStar(1) == data));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1,2,3,4 },{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{ 3,4,2,1 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3 },{},{ 2 },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 3 },{ 1,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 4 },{ 1,2 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 9, { { 5 },{},{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{},{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 5 },{ 1,2 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 14, { { 9 },{ 1,2 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 15, { { 5 },{ 1,2,3,4 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 16, { { 10 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 17, { { 11 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 18, { { 11 },{ 1,3,4 },{ 2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 19, { { 12 },{ 3,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 20, { { 12 },{ 1,4 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 21, { { 13 },{ 4,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 22, { { 13 },{ 4,3 },{ 2 },{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4,8 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 5 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 9, { { 10,11 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 13,14 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 15 } });
			pkb.insertToTable(NEXT_TABLE, 14, { { 15 } });

			pkb.insertToTable(NEXT_TABLE, 16, { { 17 } });
			pkb.insertToTable(NEXT_TABLE, 17, { { 18 } });
			pkb.insertToTable(NEXT_TABLE, 18, { { 19,16 } });
			pkb.insertToTable(NEXT_TABLE, 19, { { 20 } });
			pkb.insertToTable(NEXT_TABLE, 20, { { 21,18 } });
			pkb.insertToTable(NEXT_TABLE, 21, { { 22 } });
			pkb.insertToTable(NEXT_TABLE, 22, { { 20 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,7,8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 3 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 10, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 10,11 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 14, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 15, { { 13,14 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 16, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 17, { { 16 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 18, { { 17,20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 19, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 20, { { 19,22 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 21, { { 20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 22, { { 21 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4,5,6,7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 8 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 0 },{ 9,12,15 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 9 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 9 },{ 13 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 14 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 9, { { 12 },{ 13 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 10, { { 0 },{ 16 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 11, { { 16 },{ 17,18 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 12, { { 18 },{ 19,20 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 13, { { 20 },{ 21,22 },{} });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 5 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 10 },{},{} });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			data = { { 4 },{ 5 }, {7}, {8} };
			Assert::AreEqual(true, (pkb.getAffectsAfterStar(1) == data));

			data = { { 13 },{ 14 },{15} };
			Assert::AreEqual(true, (pkb.getAffectsAfterStar(10) == data));

			data = { { 13 },{ 14 },{ 15 } };
			Assert::AreEqual(true, (pkb.getAffectsAfterStar(11) == data));

			data = { { 15 } };
			Assert::AreEqual(true, (pkb.getAffectsAfterStar(13) == data));

			data.clear();
			Assert::AreEqual(true, (pkb.getAffectsAfterStar(17) == data));

			data = { { 17 },{ 19 },{ 21 }, {22} };
			Assert::AreEqual(true, (pkb.getAffectsAfterStar(19) == data));

			data = { { 17 },{ 19 },{21}, { 22 } };
			Assert::AreEqual(true, (pkb.getAffectsAfterStar(21) == data));


		}

		TEST_METHOD(PKBCheckAffects)
		{
			PKB pkb;

			Assert::AreEqual(false, pkb.checkAffects(1,2));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1,2,3,4 },{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{ 3,4,2,1 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3 },{},{ 2 },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 3 },{ 1,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 4 },{ 1,2 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 9, { { 5 },{},{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{},{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 5 },{ 1,2 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 14, { { 9 },{ 1,2 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 15, { { 5 },{ 1,2,3,4 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 16, { { 10 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 17, { { 11 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 18, { { 11 },{ 1,3,4 },{ 2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 19, { { 12 },{ 3,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 20, { { 12 },{ 1,4 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 21, { { 13 },{ 4,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 22, { { 13 },{ 4,3 },{ 2 },{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4,8 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 5 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 9, { { 10,11 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 13,14 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 15 } });
			pkb.insertToTable(NEXT_TABLE, 14, { { 15 } });

			pkb.insertToTable(NEXT_TABLE, 16, { { 17 } });
			pkb.insertToTable(NEXT_TABLE, 17, { { 18 } });
			pkb.insertToTable(NEXT_TABLE, 18, { { 19,16 } });
			pkb.insertToTable(NEXT_TABLE, 19, { { 20 } });
			pkb.insertToTable(NEXT_TABLE, 20, { { 21,18 } });
			pkb.insertToTable(NEXT_TABLE, 21, { { 22 } });
			pkb.insertToTable(NEXT_TABLE, 22, { { 20 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,7,8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 3 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 10, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 10,11 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 14, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 15, { { 13,14 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 16, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 17, { { 16 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 18, { { 17,20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 19, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 20, { { 19,22 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 21, { { 20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 22, { { 21 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4,5,6,7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 8 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 0 },{ 9,12,15 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 9 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 9 },{ 13 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 14 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 9, { { 12 },{ 13 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 10, { { 0 },{ 16 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 11, { { 16 },{ 17,18 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 12, { { 18 },{ 19,20 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 13, { { 20 },{ 21,22 },{} });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 5 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 10 },{},{} });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			Assert::AreEqual(true, pkb.checkAffects(1, 4));
			Assert::AreEqual(true, pkb.checkAffects(1, 5));
			Assert::AreEqual(false, pkb.checkAffects(1, 6));
			Assert::AreEqual(true, pkb.checkAffects(4, 5));
			Assert::AreEqual(false, pkb.checkAffects(4, 7));
			Assert::AreEqual(true, pkb.checkAffects(1, 8));

			Assert::AreEqual(false, pkb.checkAffects(10, 11));
			Assert::AreEqual(false, pkb.checkAffects(10, 12));
			Assert::AreEqual(true, pkb.checkAffects(10, 13));
			Assert::AreEqual(true, pkb.checkAffects(10, 14));
			Assert::AreEqual(true, pkb.checkAffects(10, 15));
			Assert::AreEqual(false, pkb.checkAffects(11, 10));
			Assert::AreEqual(false, pkb.checkAffects(11, 12));
			Assert::AreEqual(true, pkb.checkAffects(11, 13));
			Assert::AreEqual(true, pkb.checkAffects(11, 14));
			Assert::AreEqual(true, pkb.checkAffects(11, 15));
			Assert::AreEqual(false, pkb.checkAffects(12, 15));
			Assert::AreEqual(true, pkb.checkAffects(13, 15));
			Assert::AreEqual(true, pkb.checkAffects(14, 15));
			Assert::AreEqual(true, pkb.checkAffects(22, 21));
			Assert::AreEqual(true, pkb.checkAffects(19, 21));
			Assert::AreEqual(false, pkb.checkAffects(17, 19));
			Assert::AreEqual(true, pkb.checkAffects(19, 17));
			Assert::AreEqual(true, pkb.checkAffects(22, 19));
			Assert::AreEqual(true, pkb.checkAffects(22, 17));
		}



		TEST_METHOD(PKBCheckAffectsStar)
		{
			PKB pkb;

			Assert::AreEqual(false, pkb.checkAffectsStar(1, 2));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1,2,3,4 },{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{ 3,4,2,1 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3 },{},{ 2 },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 3 },{ 1,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 4 },{ 1,2 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 9, { { 5 },{},{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{},{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 5 },{ 1,2 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 14, { { 9 },{ 1,2 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 15, { { 5 },{ 1,2,3,4 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 16, { { 10 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 17, { { 11 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 18, { { 11 },{ 1,3,4 },{ 2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 19, { { 12 },{ 3,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 20, { { 12 },{ 1,4 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 21, { { 13 },{ 4,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 22, { { 13 },{ 4,3 },{ 2 },{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4,8 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 5 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 9, { { 10,11 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 13,14 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 15 } });
			pkb.insertToTable(NEXT_TABLE, 14, { { 15 } });

			pkb.insertToTable(NEXT_TABLE, 16, { { 17 } });
			pkb.insertToTable(NEXT_TABLE, 17, { { 18 } });
			pkb.insertToTable(NEXT_TABLE, 18, { { 19,16 } });
			pkb.insertToTable(NEXT_TABLE, 19, { { 20 } });
			pkb.insertToTable(NEXT_TABLE, 20, { { 21,18 } });
			pkb.insertToTable(NEXT_TABLE, 21, { { 22 } });
			pkb.insertToTable(NEXT_TABLE, 22, { { 20 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,7,8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 3 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 10, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 10,11 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 14, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 15, { { 13,14 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 16, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 17, { { 16 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 18, { { 17,20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 19, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 20, { { 19,22 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 21, { { 20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 22, { { 21 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4,5,6,7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 8 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 0 },{ 9,12,15 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 9 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 9 },{ 13 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 14 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 9, { { 12 },{ 13 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 10, { { 0 },{ 16 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 11, { { 16 },{ 17,18 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 12, { { 18 },{ 19,20 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 13, { { 20 },{ 21,22 },{} });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 5 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 10 },{},{} });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			Assert::AreEqual(true, pkb.checkAffectsStar(1, 4));
			Assert::AreEqual(true, pkb.checkAffectsStar(1, 5));
			Assert::AreEqual(false, pkb.checkAffectsStar(1, 6));
			Assert::AreEqual(true, pkb.checkAffectsStar(4, 5));
			Assert::AreEqual(true, pkb.checkAffectsStar(4, 7));
			Assert::AreEqual(true, pkb.checkAffectsStar(1, 8));
			Assert::AreEqual(true, pkb.checkAffectsStar(4, 8));
			Assert::AreEqual(true, pkb.checkAffectsStar(5, 8));
			Assert::AreEqual(true, pkb.checkAffectsStar(7, 8));
			Assert::AreEqual(true, pkb.checkAffectsStar(8, 8));

			Assert::AreEqual(false, pkb.checkAffectsStar(10, 11));
			Assert::AreEqual(false, pkb.checkAffectsStar(10, 12));
			Assert::AreEqual(true, pkb.checkAffectsStar(10, 13));
			Assert::AreEqual(true, pkb.checkAffectsStar(10, 14));
			Assert::AreEqual(true, pkb.checkAffectsStar(10, 15));
			Assert::AreEqual(false, pkb.checkAffectsStar(11, 10));
			Assert::AreEqual(false, pkb.checkAffectsStar(11, 12));
			Assert::AreEqual(true, pkb.checkAffectsStar(11, 13));
			Assert::AreEqual(true, pkb.checkAffectsStar(11, 14));
			Assert::AreEqual(true, pkb.checkAffectsStar(11, 15));
			Assert::AreEqual(false, pkb.checkAffectsStar(12, 15));
			Assert::AreEqual(true, pkb.checkAffectsStar(13, 15));
			Assert::AreEqual(true, pkb.checkAffectsStar(14, 15));

			Assert::AreEqual(true, pkb.checkAffectsStar(22, 21));
			Assert::AreEqual(true, pkb.checkAffectsStar(21, 21));
			Assert::AreEqual(true, pkb.checkAffectsStar(19, 21));
			Assert::AreEqual(false, pkb.checkAffectsStar(17, 19));
			Assert::AreEqual(true, pkb.checkAffectsStar(19, 17));
			Assert::AreEqual(true, pkb.checkAffectsStar(21, 19));
			Assert::AreEqual(true, pkb.checkAffectsStar(22, 19));
			Assert::AreEqual(true, pkb.checkAffectsStar(22, 17));
		}

		TEST_METHOD(PKBGetAllAffects)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			Assert::AreEqual(true, (pkb.getAllAffects() == data));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1,2,3,4 },{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{ 3,4,2,1 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3 },{},{ 2 },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 3 },{ 1,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 4 },{ 1,2 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 9, { { 5 },{},{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{},{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 5 },{ 1,2 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 14, { { 9 },{ 1,2 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 15, { { 5 },{ 1,2,3,4 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 16, { { 10 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 17, { { 11 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 18, { { 11 },{ 1,3,4 },{ 2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 19, { { 12 },{ 3,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 20, { { 12 },{ 1,4 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 21, { { 13 },{ 4,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 22, { { 13 },{ 4,3 },{ 2 },{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4,8 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 5 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 9, { { 10,11 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 13,14 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 15 } });
			pkb.insertToTable(NEXT_TABLE, 14, { { 15 } });

			pkb.insertToTable(NEXT_TABLE, 16, { { 17 } });
			pkb.insertToTable(NEXT_TABLE, 17, { { 18 } });
			pkb.insertToTable(NEXT_TABLE, 18, { { 19,16 } });
			pkb.insertToTable(NEXT_TABLE, 19, { { 20 } });
			pkb.insertToTable(NEXT_TABLE, 20, { { 21,18 } });
			pkb.insertToTable(NEXT_TABLE, 21, { { 22 } });
			pkb.insertToTable(NEXT_TABLE, 22, { { 20 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,7,8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 3 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 10, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 10,11 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 14, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 15, { { 13,14 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 16, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 17, { { 16 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 18, { { 17,20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 19, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 20, { { 19,22 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 21, { { 20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 22, { { 21 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { {0}, {1,2}, {} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4,5,6,7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 8 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 0 },{9,12,15},{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 9 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 9 },{ 13 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 14 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 9, { { 12 },{ 13 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 10, { { 0 },{ 16 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 11, { { 16 },{ 17,18 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 12, { { 18 },{ 19,20 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 13, { { 20 },{ 21,22 },{} });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 }, {}, {} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 5 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 10 },{},{} });

			pkb.insertToNameTable(PROC_TABLE, {"a"});
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			data = { {1,4}, {1,5}, {1,8}, {4,5}, {4,7},{ 5,4 },{ 5,5 },{ 5,7 },{ 5,8 },{7,8}, {8, 4}, {8,5}, {8,8}, {10, 13}, {10,14}, {10,15}, {11,13}, {11,14}, {11,15},{13,15}, {14,15}, {19,17}, {19,19},{ 19,21 },{21, 19},{ 21, 22 },{22, 17},{ 22, 19 },{ 22, 21 } };
			Assert::AreEqual(true, (pkb.getAllAffects() == data));
		}

		TEST_METHOD(PKBGetAllAffectsStar)
		{
			PKB pkb;
			std::vector<std::vector<int>> data;

			Assert::AreEqual(true, (pkb.getAllAffects() == data));

			pkb.insertToTable(STATEMENT_TABLE, 1, { { 1 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 2, { { 1 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 3, { { 2 },{ 1,2,3,4 },{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 4, { { 3 },{ 1 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 5, { { 3 },{ 3,4,2,1 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 6, { { 3 },{},{ 2 },{ 4 } });
			pkb.insertToTable(STATEMENT_TABLE, 7, { { 3 },{ 1,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 8, { { 4 },{ 1,2 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 9, { { 5 },{},{ 1,2 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 10, { { 6 },{},{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 11, { { 7 },{},{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 12, { { 5 },{ 1,2 },{ 3,4 },{ 3 } });
			pkb.insertToTable(STATEMENT_TABLE, 13, { { 8 },{ 1,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 14, { { 9 },{ 1,2 },{ 4 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 15, { { 5 },{ 1,2,3,4 },{ 1 },{ 1 } });

			pkb.insertToTable(STATEMENT_TABLE, 16, { { 10 },{ 1,2,3,4 },{ 1,2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 17, { { 11 },{ 2 },{ 1 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 18, { { 11 },{ 1,3,4 },{ 2 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 19, { { 12 },{ 3,2 },{ 2 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 20, { { 12 },{ 1,4 },{ 3 },{ 2 } });
			pkb.insertToTable(STATEMENT_TABLE, 21, { { 13 },{ 4,2 },{ 3 },{ 1 } });
			pkb.insertToTable(STATEMENT_TABLE, 22, { { 13 },{ 4,3 },{ 2 },{ 1 } });

			pkb.insertToTable(NEXT_TABLE, 1, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 2, { { 3 } });
			pkb.insertToTable(NEXT_TABLE, 3, { { 4,8 } });
			pkb.insertToTable(NEXT_TABLE, 4, { { 5 } });
			pkb.insertToTable(NEXT_TABLE, 5, { { 6 } });
			pkb.insertToTable(NEXT_TABLE, 6, { { 7 } });
			pkb.insertToTable(NEXT_TABLE, 7, { { 2 } });
			pkb.insertToTable(NEXT_TABLE, 8, { { 2 } });

			pkb.insertToTable(NEXT_TABLE, 9, { { 10,11 } });
			pkb.insertToTable(NEXT_TABLE, 10, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 11, { { 12 } });
			pkb.insertToTable(NEXT_TABLE, 12, { { 13,14 } });
			pkb.insertToTable(NEXT_TABLE, 13, { { 15 } });
			pkb.insertToTable(NEXT_TABLE, 14, { { 15 } });

			pkb.insertToTable(NEXT_TABLE, 16, { { 17 } });
			pkb.insertToTable(NEXT_TABLE, 17, { { 18 } });
			pkb.insertToTable(NEXT_TABLE, 18, { { 19,16 } });
			pkb.insertToTable(NEXT_TABLE, 19, { { 20 } });
			pkb.insertToTable(NEXT_TABLE, 20, { { 21,18 } });
			pkb.insertToTable(NEXT_TABLE, 21, { { 22 } });
			pkb.insertToTable(NEXT_TABLE, 22, { { 20 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 2, { { 1,7,8 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 3, { { 2 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 4, { { 3 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 5, { { 4 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 6, { { 5 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 7, { { 6 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 8, { { 3 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 10, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 11, { { 9 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 12, { { 10,11 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 13, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 14, { { 12 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 15, { { 13,14 } });

			pkb.insertToTable(NEXT_INVERSE_TABLE, 16, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 17, { { 16 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 18, { { 17,20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 19, { { 18 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 20, { { 19,22 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 21, { { 20 } });
			pkb.insertToTable(NEXT_INVERSE_TABLE, 22, { { 21 } });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 1, { { 0 },{ 1,2 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 2, { { 2 },{ 3 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 3, { { 3 },{ 4,5,6,7 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 4, { { 3 },{ 8 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 5, { { 0 },{ 9,12,15 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 6, { { 9 },{ 10 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 7, { { 9 },{ 13 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 8, { { 12 },{ 14 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 9, { { 12 },{ 13 },{} });

			pkb.insertToTable(STATEMENT_LIST_TABLE, 10, { { 0 },{ 16 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 11, { { 16 },{ 17,18 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 12, { { 18 },{ 19,20 },{} });
			pkb.insertToTable(STATEMENT_LIST_TABLE, 13, { { 20 },{ 21,22 },{} });

			pkb.insertToTable(PROC_INFO_TABLE, 1, { { 1 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 2, { { 5 },{},{} });
			pkb.insertToTable(PROC_INFO_TABLE, 3, { { 10 },{},{} });

			pkb.insertToNameTable(PROC_TABLE, { "a" });
			pkb.insertToNameTable(PROC_TABLE, { "b" });
			pkb.insertToNameTable(PROC_TABLE, { "c" });

			data = { { 1,4 },{ 1,5 },{1,7}, { 1,8 },{ 4,4 },{ 4,5 },{ 4,7 },{ 4,8 },{ 5,4 },{ 5,5 },{ 5,7 },{ 5,8 },{ 7,8 },{ 8, 4 },{ 8,5 },{ 8,7 },{ 8,8 },{ 10, 13 },{ 10,14 },{ 10,15 },{ 11,13 },{ 11,14 },{ 11,15 },{ 13,15 },{ 14,15 },{ 19,17 },{ 19,19 },{ 19,21 },{ 19,22 },{ 21, 17 },{ 21, 19 },{ 21, 21 },{ 21, 22 },{ 22, 17 },{ 22, 19 },{ 22, 21 },{ 22, 22 } };
			Assert::AreEqual(true, (pkb.getAllAffectsStar() == data));
		}
		
		TEST_METHOD(PKBGetStatementsWithConstant)
		{
			PKB pkb;
			
			std::vector<std::vector<int>> initial_data;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == initial_data));

			pkb.insertToTable(CONST_TABLE, 1, { { 1 } });
			std::vector<std::vector<int>> data = { {1} };
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(1) == data));
			Assert::AreEqual(true, (pkb.getStatementsWithConstant(2) == initial_data));

		}

		TEST_METHOD(PKBGetAllConstants)
		{
			PKB pkb;

			std::vector<std::vector<int>> initial_data;

			/* Null Tests */
			Assert::AreEqual(true, (pkb.getAllConstants() == initial_data));

			pkb.insertToTable(CONST_TABLE, 1, { { 1 } });
			pkb.insertToTable(CONST_TABLE, 2, { { 1 } });
			std::vector<std::vector<int>> data = { { 1 }, {2} };
			Assert::AreEqual(true, (pkb.getAllConstants() == data));

		}


	};

}
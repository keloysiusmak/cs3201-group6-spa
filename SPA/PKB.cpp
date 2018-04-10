#pragma once

#include <algorithm>
#include <set>;
#include <queue>;
#include <stack>;

using namespace std;

#include "PKB.h"
#include "../SPA/Constants.h"

/* Parser/DE Operations */
bool PKB::insertToTable(int table_id, int key_id, std::vector<std::vector<int>> value)
{
	unordered_map<int, std::vector<std::vector<int>>> table = tables[table_id - 1];
	unordered_map<int, std::vector<std::vector<int>>>::const_iterator got = table.find(key_id);

	unsigned int tableValuesCount;
	switch (table_id) {
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
	case STATEMENT_LIST_INFO_TABLE:
		tableValuesCount = 1;
		break;
	default:
		return false;
	}

	/* Return false if the supplied arguments does not equal to the number of columns specified in the PKB documentation*/
	if (static_cast<int>(value.size()) != tableValuesCount) {
		return false;
	}

	/* Row does not exist */
	if (got == table.end()) {
		
		std::vector<std::vector<int>> tableValues;
		for (unsigned int i = 0; i < tableValuesCount; i++) {
			tableValues.push_back(value[i]);
		}
		
		tables[table_id - 1].insert({ key_id, tableValues });
	}
	/* Row exists */
	else {

		std::vector<std::vector<int>> tableValues = got->second;
		std::vector<int> data;

		for (unsigned int i = 0; i < tableValuesCount; i++) {
			data = tableValues[i];

			/* Insert every element in vector */
			for (unsigned int j = 0; j < static_cast<int>(value[i].size()); j++) {
				data.push_back(value[i][j]);
			}
			std::sort(data.begin(), data.end());
			data.erase(unique(data.begin(), data.end()), data.end());
			tableValues[i] = data;
		}
		tables[table_id - 1].erase(key_id);
		tables[table_id - 1].insert({ key_id, tableValues });
	}
	return true;
}

int PKB::insertToNameTable(int table_id, std::vector<string> value)
{
	unordered_map<int, std::vector<string>> table = nameTables[table_id - PATTERN_TABLE];
	if (table_id != PROC_TABLE && table_id != VAR_TABLE && table_id != PATTERN_TABLE) {
		return 0;
	}
	else if (table_id == PROC_TABLE && PKB::getProcedureId(value[0]) != 0) {
		return PKB::getProcedureId(value[0]);
	}
	else if (table_id == VAR_TABLE && PKB::getVariableId(value[0]) != 0) {
		return PKB::getVariableId(value[0]);
	}
	else {

		/* Variable does not exist */
		int size = static_cast<int>(table.size()) + 1;
		nameTables[table_id - PATTERN_TABLE].insert({size,  value });

		return size;
	}
}

bool PKB::insertToResultTable(Relations r, int firstParam, int secondParam, int calcResult) {
	unordered_map<int, unordered_map<int, unordered_map<int, int>>>::const_iterator got = resultTables.find(r);
	if (got == resultTables.end()) {
		unordered_map<int, int> second;
		second.insert({secondParam, calcResult});
		unordered_map<int, unordered_map<int, int>> first;
		first.insert({ firstParam, second });
		resultTables.insert({ r, first });
		return true;
	}
	unordered_map<int, unordered_map<int, int>> result = got->second;
	unordered_map<int, unordered_map<int, int>>::const_iterator got2 = result.find(firstParam);
	if (got2 == result.end()) {
		unordered_map<int, int> second;
		second.insert({ secondParam, calcResult });
		resultTables[r].insert({firstParam, second});
		return true;
	}
	unordered_map<int, int> result2 = got2->second;
	unordered_map<int, int>::const_iterator got3 = result2.find(secondParam);
	if (got3 == result2.end()) {
		resultTables[r][firstParam].insert({ secondParam, calcResult });
		return true;
	}
	return false;
}

std::vector<std::vector<int>> PKB::getFromTable(int table_id, int key_id)
{
	if (table_id < 1 || table_id >= PATTERN_TABLE) {
		std::vector<std::vector<int>> data;
		return data;
	}
	else if (table_id >= PROC_NAME_VAR_NAME_TABLE && table_id <= STMT_NO_CONST_VALUE_TABLE) {
		unordered_map<int, std::vector<std::vector<int>>> table = tables[table_id - 1];
		std::vector<std::vector<int>> data;
		for (auto it = table.begin(); it != table.end(); ++it) {
			data.push_back({it->first, it->second[0][0]});
		}
		return data;
	}
	else {
		std::unordered_map<int, std::vector<std::vector<int>>>::const_iterator got = tables[table_id - 1].find(key_id);
		if (got == tables[table_id - 1].end()) {
			std::vector<std::vector<int>> data;
			return data;
		}
		return tables[table_id - 1][key_id];
	}
}

std::vector<string> PKB::getFromNameTable(int table_id, int key_id)
{
	std::unordered_map<int, std::vector<string>>::const_iterator got = nameTables[table_id - PATTERN_TABLE].find(key_id);
	if (got == nameTables[table_id - PATTERN_TABLE].end()) {
		std::vector<string> data;
		return data;
	}
	return got->second;
}

int PKB::getFromResultTable(Relations r, int param1, int param2)
{
	unordered_map<int, unordered_map<int, unordered_map<int, int>>>::const_iterator got = resultTables.find(r);
	if (got == resultTables.end()) {
		return 0;
	}
	unordered_map<int, unordered_map<int, int>> result = got->second;
	unordered_map<int, unordered_map<int, int>>::const_iterator got2 = result.find(param1);
	if (got2 == result.end()) {
		return 0;
	}
	unordered_map<int, int> result2 = got2->second;
	unordered_map<int, int>::const_iterator got3 = result2.find(param2);
	if (got3 == result2.end()) {
		return 0;
	}
	return got3->second;
}

int PKB::getProcedureId(std::string proc_name) {
	unordered_map<int, std::vector<string>> table = nameTables[PROC_TABLE - PATTERN_TABLE];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0] == proc_name) {
			return it->first;
		}
	}

	return 0;
}

int PKB::getVariableId(std::string var_name) {
	unordered_map<int, std::vector<string>> table = nameTables[VAR_TABLE - PATTERN_TABLE];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0] == var_name) {
			return it->first;
		}
	}

	return 0;
}
std::string PKB::getProcedureName(int proc_id) {
	std::vector<string> data = PKB::getFromNameTable(PROC_TABLE, proc_id);
	if (data.size() > 0) {
		return data[0];
	}
	else {
		return "";
	}
}

std::string PKB::getVariableName(int var_id) {
	std::vector<string> data = PKB::getFromNameTable(VAR_TABLE, var_id);
	if (data.size() > 0) {
		return data[0];
	}
	else {
		return "";
	}
}

/* General Operations */
std::vector<std::vector<int>> PKB::getAllVariables() {

	std::vector<std::vector<int>> new_data;

	unordered_map<int, std::vector<string>> table = nameTables[VAR_TABLE - PATTERN_TABLE];
	for (auto it = table.begin(); it != table.end(); ++it) {
		new_data.push_back({ it->first });
	}

	return new_data;

}

std::vector<std::vector<int>> PKB::getAllStatements() {

	std::vector<std::vector<int>> new_data;

	unordered_map<int, std::vector<std::vector<int>>> table = tables[STATEMENT_TABLE - 1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		new_data.push_back({ it->first });
	}

	return new_data;

}

std::vector<std::vector<int>> PKB::getAllStatementsWithType(int stmt_type) {

	std::vector<std::vector<int>> new_data;

	unordered_map<int, std::vector<std::vector<int>>> table = tables[STATEMENT_TABLE - 1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[3][0] == stmt_type) {
			new_data.push_back({ it->first });
		}
	}

	return new_data;

}

std::vector<std::vector<int>> PKB::getAllProcedures() {

	std::vector<std::vector<int>> new_data;

	unordered_map<int, std::vector<string>> table = nameTables[PROC_TABLE - PATTERN_TABLE];
	for (auto it = table.begin(); it != table.end(); ++it) {
		new_data.push_back({ it->first });
	}
	return new_data;

}

std::vector<std::vector<int>> PKB::getAllStatementsFromProcedure(int proc) {
	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> table = PKB::getFromTable(PROC_INFO_TABLE, proc);
	if (table.size() > 0 && table[0].size() > 0) {
		table = PKB::getFromTable(STATEMENT_LIST_TABLE, table[0][0]);
		if (table.size() > 0) {
			for (int i = 0; i < table[1].size(); i++) {
				data.push_back({ table[1][i] });
			}
			if (static_cast<int>(data.size()) > 0) {
				for (unsigned int i = 0; i < static_cast<int>(data.size()); i++) {
					std::vector<std::vector<int>> recur_output = PKB::getChildren(data[i][0]);
					for (unsigned int j = 0; j < static_cast<int>(recur_output.size()); j++) {
						data.push_back({ recur_output[j][0] });
					}
				}
			}
		}
	}

	return data;
}

std::vector<std::vector<int>> PKB::getProcedureFromStatement(int stmt) {
	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> parent = PKB::getParent(stmt);

	int tempParent = 0;
	if (parent.size() > 0 && parent[0].size() > 0) {
		while (parent[0][0] != 0) {
			tempParent = parent[0][0];
			parent = PKB::getParent(parent[0][0]);
		}
		parent = { {tempParent} };
	}
	int result = 0;
	if (tempParent != 0 && parent.size() > 0 && parent[0].size() > 0) {
		output = PKB::getFromTable(STATEMENT_TABLE, parent[0][0]);
		if (output.size() > 0 && output[0].size() > 0) {
			output = PKB::getFromTable(STATEMENT_LIST_TABLE, output[0][0]);
			if (output.size() > 0 && output[0].size() > 0) {
				result = output[2][0];
			}
		}
	}
	else if (tempParent == 0) {
		output = PKB::getFromTable(STATEMENT_TABLE, stmt);
		if (output.size() > 0 && output[0].size() > 0) {
			output = PKB::getFromTable(STATEMENT_LIST_TABLE, output[0][0]);
			if (output.size() > 0 && output[0].size() > 0) {
				result = output[2][0];
			}
		}
	}
	return { {result} };
}

bool PKB::checkStatementHasType(int stmt, int stmt_type) {

	std::vector<std::vector<int>> new_data;

	std::vector<std::vector<int>> table = PKB::getFromTable(STATEMENT_TABLE, stmt);
	if (table.size() == 0) {
		return false;
	}
	else {
		return (table[3][0] == stmt_type);
	}

}

/* Statement List Operations */
std::vector<std::vector<int>> PKB::getAllStatementLists() {
	unordered_map<int, std::vector<std::vector<int>>> table = tables[STATEMENT_LIST_TABLE - 1];
	std::vector<std::vector<int>> output;
	for (auto it = table.begin(); it != table.end(); ++it) {
		output.push_back({it->second[1][0]});
	}
	return output;
}
std::vector<std::vector<int>> PKB::getAllStatementListsFirstStmt() {
	std::vector<int> result = PKB::getFromTable(STATEMENT_LIST_INFO_TABLE, 1)[0];
	std::vector<std::vector<int>> output;
	for (int a : result) {
		output.push_back({ a });
	}
	return output;
}

/* Optimization Operations */
std::vector<std::vector<int>> PKB::getWithProcNameVarName() {
	std::vector<std::vector<int>> data = PKB::getFromTable(PROC_NAME_VAR_NAME_TABLE, 1);
	return data;
}
std::vector<std::vector<int>> PKB::getWithProcNameCallProcName() {
	std::vector<std::vector<int>> data = PKB::getFromTable(PROC_NAME_CALL_NAME_TABLE, 1);
	return data;
}
std::vector<std::vector<int>> PKB::getWithVarNameCallProcName() {
	std::vector<std::vector<int>> data = PKB::getFromTable(VAR_NAME_CALL_NAME_TABLE, 1);
	return data;
}
std::vector<std::vector<int>> PKB::getWithStmtNoConstValue(int type) {
	std::vector<std::vector<int>> data = PKB::getFromTable(STMT_NO_CONST_VALUE_TABLE, 1);
	if (type > 0) {
		std::vector<std::vector<int>> filtered;
		for (int i = 0; i < data.size(); i++) {
			if (PKB::checkStatementHasType(data[i][0], type)) {
				filtered.push_back({data[i][0], data[i][1]});
			}
		}
		data = filtered;
	}
	std::sort(data.begin(), data.end());
	return data;
}
std::vector<std::vector<int>> PKB::getPatternOneSyn(TYPES t) {
	std::vector<std::vector<int>> data;
	if (t == ASSIGNMENT_TYPE || t == WHILE_TYPE || t == IF_TYPE) {
		data = PKB::getAllStatementsWithType(t);
	}
	return data;
}
std::vector<std::vector<int>> PKB::getPatternTwoSyn(TYPES t) {
	std::vector<std::vector<int>> data;
	if (t == ASSIGNMENT_TYPE) {
		unordered_map<int, std::vector<std::vector<int>>> result = tables[PATTERN_ASSIGN_VARIABLE_TABLE - 1];
		for (auto it = result.begin(); it != result.end(); ++it) {
			for (int i = 0; i < it->second[0].size(); i++) {
				data.push_back({ it->first, it->second[0][i] });
			}
		}
	}
	else if (t == WHILE_TYPE) {
		unordered_map<int, std::vector<std::vector<int>>> result = tables[PATTERN_WHILE_VARIABLE_TABLE - 1];
		for (auto it = result.begin(); it != result.end(); ++it) {
			for (int i = 0; i < it->second[0].size(); i++) {
				data.push_back({ it->first, it->second[0][i] });
			}
		}
	}
	else if (t == IF_TYPE) {
		unordered_map<int, std::vector<std::vector<int>>> result = tables[PATTERN_IF_VARIABLE_TABLE - 1];
		for (auto it = result.begin(); it != result.end(); ++it) {
			for (int i = 0; i < it->second[0].size(); i++) {
				data.push_back({ it->first, it->second[0][i] });
			}
		}
	}
	return data;
}

/* Follows Operations */
std::vector<std::vector<int>> PKB::getFollowsBefore(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	if (static_cast<int>(data.size()) > 0) {
		int stmtListID = data[0][0];
		data = PKB::getFromTable(STATEMENT_LIST_TABLE, stmtListID);

		if (static_cast<int>(data.size()) > 0) {

			for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
				if (data[1][i] == stmt) {
					if (i > 0) {
						std::vector<std::vector<int>> result = { { data[1][i - 1] } };
						return result;
					}
					break;
				}
			}

		}
	}
	
	std::vector<std::vector<int>> empty;
	return empty;
}

std::vector<std::vector<int>> PKB::getFollowsAfter(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(STATEMENT_TABLE, stmt);
	if (static_cast<int>(data.size()) > 0) {
		int stmtListID = data[0][0];
		data = PKB::getFromTable(STATEMENT_LIST_TABLE, stmtListID);

		if (static_cast<int>(data.size()) > 0) {

			for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
				if (data[1][i] == stmt) {
					if ((i + 1) < static_cast<int>(data[1].size())) {
						std::vector<std::vector<int>> result = { { data[1][i + 1] } };
						return result;
					}
					break;
				}
			}

		}
	}
	
	std::vector<std::vector<int>> empty;
	return empty;
}

std::vector<std::vector<int>> PKB::getFollowsBeforeStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	if (static_cast<int>(data.size()) > 0) {
		int stmtListID = data[0][0];
		data = PKB::getFromTable(STATEMENT_LIST_TABLE, stmtListID);

		if (static_cast<int>(data.size()) > 0) {

			for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
				if (i > 0 && data[1][i] == stmt) {
					data[1].erase(data[1].begin() + i, data[1].begin() + static_cast<int>(data[1].size()));

					std::vector<std::vector<int>> result;
					for (int i = 0; i < data[1].size(); i++) {
						result.push_back({ data[1][i] });
					}
					return result;
					break;
				}
			}

		}
	}

	std::vector<std::vector<int>> blank_data;
	return blank_data;
}

std::vector<std::vector<int>> PKB::getFollowsAfterStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	if (static_cast<int>(data.size()) > 0) {
		int stmtListID = data[0][0];
		data = PKB::getFromTable(STATEMENT_LIST_TABLE, stmtListID);

		if (static_cast<int>(data.size()) > 0) {

			for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
				if ((i+1) < data[1].size() && data[1][i] == stmt) {
					data[1].erase(data[1].begin(), data[1].begin() + i + 1);
					std::vector<std::vector<int>> result;
					for (int i = 0; i < data[1].size(); i++) {
						result.push_back({data[1][i]});
					}
					return result;
					break;
				}
			}

		}
	}

	std::vector<std::vector<int>> blank_data;
	return blank_data;
}

bool PKB::checkFollows(int stmt1, int stmt2) {
	std::vector<std::vector<int>> data = PKB::getFollowsAfter(stmt1);
	if (data.size() > 0 && data[0].size() > 0) {
		return (stmt2 == data[0][0]);
	}
	return false;
}

bool PKB::checkFollowsStar(int stmt1, int stmt2) {
	std::vector<std::vector<int>> stmtLists = PKB::getFollowsAfterStar(stmt1);
	if (stmtLists.size() > 0) {

		for (unsigned int i = 0; i < static_cast<int>(stmtLists.size()); i++) {
			if (stmt2 == stmtLists[i][0]) {
				return true;
			}
		}
	}

	return false;
}

std::vector<std::vector<int>> PKB::getAllFollows() {

	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[STATEMENT_LIST_TABLE - 1];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[1];
		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()) - 1; i++) {
			std::vector<int> newOutput;
			newOutput.push_back(it->second[1][i]);
			newOutput.push_back(it->second[1][i + 1]);
			output.push_back(newOutput);
		}
	}

	return output;
}

std::vector<std::vector<int>> PKB::getAllFollowsStar() {

	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[STATEMENT_LIST_TABLE - 1];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[1];

		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()) - 1; i++) {
			for (unsigned int j = i + 1; j < static_cast<int>(stmtList.size()); j++) {
				output.push_back({ stmtList[i], stmtList[j] });
			}
		}
	}

	return output;
}

/* Parent Operations */
std::vector<std::vector<int>> PKB::getParent(int stmt) {
	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> result;
	data = PKB::getFromTable(STATEMENT_TABLE, stmt);
	if (static_cast<int>(data.size()) > 0 && static_cast<int>(data[0].size()) > 0) {
		data = PKB::getFromTable(STATEMENT_LIST_TABLE, data[0][0]);
		if (static_cast<int>(data.size()) > 0 && static_cast<int>(data[0].size()) > 0) {
			result.push_back({ data[0][0] });
			return result;
		}
	}

	return result;
}

std::vector<std::vector<int>> PKB::getParentStar(int stmt) {
	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> parent = PKB::getParent(stmt);
	
	if (parent.size() > 0 && parent[0].size() > 0) {
		while (parent[0][0] != 0) {
			output.push_back({ parent[0][0] });
			parent = PKB::getParent(parent[0][0]);
		}
	}

	return output;
}

std::vector<std::vector<int>> PKB::getChildren(int stmt) {
	unordered_map<int, std::vector<std::vector<int>>> table = tables[STATEMENT_LIST_TABLE - 1];


	std::vector<std::vector<int>> res;

	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0][0] == stmt) {
			for (int i = 0; i < it->second[1].size(); i++) {
				res.push_back({ it->second[1][i] });
			}
		}
	}
	return res;
}

std::vector<std::vector<int>> PKB::getChildrenStar(int stmt) {
	std::vector<std::vector<int>> data;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[STATEMENT_LIST_TABLE - 1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0][0] == stmt) {
			for (unsigned int i = 0; i < static_cast<int>(it->second[1].size()); i++) {
				data.push_back({ it->second[1][i] });
			}
		}
	}
	if (static_cast<int>(data.size()) > 0) {
		for (unsigned int i = 0; i < static_cast<int>(data.size()); i++) {
			std::vector<std::vector<int>> recur_output = PKB::getChildren(data[i][0]);
			for (unsigned int j = 0; j < static_cast<int>(recur_output.size()); j++) {
				data.push_back({ recur_output[j][0] });
			}
		}
	}

	return data;
}

bool PKB::checkParent(int stmt1, int stmt2) {
	std::vector<std::vector<int>> children = PKB::getChildren(stmt1);

	if (children.size() > 0 && children[0].size() > 0) {
		for (unsigned int i = 0; i < static_cast<int>(children.size()); i++) {
			if (stmt2 == children[i][0]) {
				return true;
			}
		}
	}

	return false;
}

bool PKB::checkParentStar(int stmt1, int stmt2) {
	std::vector<std::vector<int>> children = PKB::getChildrenStar(stmt1);

	if (children.size() > 0 && children[0].size() > 0) {
		for (unsigned int i = 0; i < static_cast<int>(children.size()); i++) {
			if (stmt2 == children[i][0]) {
				return true;
			}
		}
	}

	return false;
}

std::vector<std::vector<int>> PKB::getAllParent() {

	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[STATEMENT_LIST_TABLE - 1];
	std::vector<int> newOutput;

	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0][0] != 0) {
			std::vector<int> stmtList = it->second[1];
			for (unsigned int i = 0; i < static_cast<int>(stmtList.size()); i++) {
				newOutput.clear();
				newOutput.push_back(it->second[0][0]);
				newOutput.push_back(stmtList[i]);
				output.push_back(newOutput);
			}
		}
	}

	return output;
}

std::vector<std::vector<int>> PKB::getAllParentStar() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> data = PKB::getAllStatements();
	
	for (int j = 0; j < data.size(); j++) {
		if (data.size() > 0 && data[j].size() > 0) {
			std::vector<std::vector<int>> stmtList = PKB::getChildrenStar(data[j][0]);
			if (stmtList.size() > 0 && stmtList[0].size() > 0) {
				for (int i = 0; i < stmtList.size(); i++) {
					output.push_back({ data[j][0], stmtList[i][0] });
				}
			}
		}
	}

	return output;
}

/* Uses Operations */
std::vector<std::vector<int>> PKB::getUsesVariablesFromStatement(int stmt) {

	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> table = PKB::getFromTable(STATEMENT_TABLE, stmt);

	if (table.size() > 0 && table[1].size() > 0) {
		for (int i = 0; i < table[1].size(); i++) {
			data.push_back({ table[1][i] });
		}
	}

	return data;

}

std::vector<std::vector<int>> PKB::getStatementsFromUsesVariable(int varId) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(USES_TABLE, varId);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<std::vector<int>> res;
		for (int i = 0; i < data[0].size(); i++) {
			res.push_back({ data[0][i] });
		}
		return res;
	}
	else {
		std::vector<std::vector<int>> null_data;
		return null_data;
	}

}

std::vector<std::vector<int>> PKB::getUsesVariablesFromProcedure(int proc) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(PROC_INFO_TABLE, proc);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<std::vector<int>> res;
		for (int i = 0; i < data[1].size(); i++) {
			res.push_back({ data[1][i] });
		}
		return res;
	}
	else {
		std::vector<std::vector<int>> null_data;
		return null_data;
	}

}

std::vector<std::vector<int>> PKB::getProceduresFromUsesVariable(int varId) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(USES_TABLE, varId);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<std::vector<int>> res;
		for (int i = 0; i < data[1].size(); i++) {
			res.push_back({ data[1][i] });
		}
		return res;
	}
	else {
		std::vector<std::vector<int>> null_data;
		return null_data;
	}

}

std::vector<std::vector<int>> PKB::getAllStatementUsesVariables() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> stmts = PKB::getAllStatements();
	std::vector<std::vector<int>> data;

	for (int i = 0; i < static_cast<int>(stmts.size()); i++) {
		data = PKB::getUsesVariablesFromStatement(stmts[i][0]);
		if (data.size() > 0) {
			for (int j = 0; j < static_cast<int>(data.size()); j++) {
				output.push_back({ stmts[i][0], data[j][0] });
			}
		}
	}

	return output;

}

std::vector<std::vector<int>> PKB::getAllVariableUsesStatements() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> vars = PKB::getAllVariables();
	std::vector<std::vector<int>> data;

	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		data = PKB::getStatementsFromUsesVariable(vars[i][0]);
		if (data.size() > 0) {
			for (int j = 0; j < static_cast<int>(data.size()); j++) {
				output.push_back({ vars[i][0], data[j][0] });
			}
		}
	}

	return output;

}

std::vector<std::vector<int>> PKB::getAllProcedureUsesVariables() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> procs = PKB::getAllProcedures();

	for (int i = 0; i < static_cast<int>(procs.size()); i++) {
		data = PKB::getUsesVariablesFromProcedure(procs[i][0]);
		for (int j = 0; j < static_cast<int>(data.size()); j++) {
			output.push_back({ procs[i][0], data[j][0] });
		}
	}

	return output;

}

std::vector<std::vector<int>> PKB::getAllVariableUsesProcedures() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> vars = PKB::getAllVariables();
	std::vector<std::vector<int>> data;

	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		data = PKB::getProceduresFromUsesVariable(vars[i][0]);
		if (data.size() > 0) {
			for (int j = 0; j < static_cast<int>(data.size()); j++) {
				output.push_back({ vars[i][0], data[j][0] });
			}
		}
	}

	return output;

}

bool PKB::checkStatementUsesVariable(int stmt, int varId) {

	std::vector<std::vector<int>> vars = PKB::getUsesVariablesFromStatement(stmt);
	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		if (vars[i][0] == varId) {
			return true;
		}
	}

	return false;

}

bool PKB::checkProcedureUsesVariable(int procId, int varId) {

	std::vector<std::vector<int>> vars = PKB::getUsesVariablesFromProcedure(procId);
	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		if (vars[i][0] == varId) {
			return true;
		}
	}

	return false;

}

/* Modifies Operations */
std::vector<std::vector<int>> PKB::getModifiesVariablesFromStatement(int stmt) {

	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> table = PKB::getFromTable(STATEMENT_TABLE, stmt);

	if (table.size() > 0 && table[2].size() > 0) {
		for (int i = 0; i < table[2].size(); i++) {
			data.push_back({ table[2][i] });
		}
	}

	return data;

}

std::vector<std::vector<int>> PKB::getStatementsFromModifiesVariable(int varId) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(MODIFIES_TABLE, varId);

	if (static_cast<int>(data.size()) > 0) {
		std::vector<std::vector<int>> output;
		for (int i = 0; i < data[0].size(); i++) {
			output.push_back({ data[0][i] });
		}
		return output;
	}
	else {
		std::vector<std::vector<int>> null_data;
		return null_data;
	}

}

std::vector<std::vector<int>> PKB::getModifiesVariablesFromProcedure(int proc) {

	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> table = PKB::getFromTable(PROC_INFO_TABLE, proc);

	if (static_cast<int>(table.size()) > 0) {

		std::vector<std::vector<int>> res;
		for (int i = 0; i < table[2].size(); i++) {
			res.push_back({ table[2][i] });
		}
		return res;
	}

	return data;

}

std::vector<std::vector<int>> PKB::getProceduresFromModifiesVariable(int varId) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(MODIFIES_TABLE, varId);
	
	if (static_cast<int>(data.size()) > 0) {

		std::vector<std::vector<int>> res;
		for (int i = 0; i < data[1].size(); i++) {
			res.push_back({ data[1][i] });
		}
		return res;
	}
	else {
		std::vector<std::vector<int>> null_data;
		return null_data;
	}

}

std::vector<std::vector<int>> PKB::getAllStatementModifiesVariables() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> stmts = PKB::getAllStatements();
	std::vector<std::vector<int>> data;

	if (stmts.size() > 0 && stmts[0].size() > 0) {
		for (int i = 0; i < static_cast<int>(stmts.size()); i++) {
			data = PKB::getModifiesVariablesFromStatement(stmts[i][0]);
			if (data.size() > 0) {
				for (int j = 0; j < static_cast<int>(data.size()); j++) {
					output.push_back({ stmts[i][0], data[j][0] });
				}
			}
		}
	}

	return output;

}

std::vector<std::vector<int>> PKB::getAllVariableModifiesStatements() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> vars = PKB::getAllVariables();

	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		data = PKB::getFromTable(MODIFIES_TABLE, vars[i][0]);
		
		if (data.size() > 0 && data[0].size() > 0) {
			for (int j = 0; j < static_cast<int>(data[0].size()); j++) {
				output.push_back({ vars[i][0], data[0][j] });
			}
		}
	}

	return output;

}

std::vector<std::vector<int>> PKB::getAllProcedureModifiesVariables() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> procs = PKB::getAllProcedures();

	if (procs.size() > 0) {
		for (int i = 0; i < static_cast<int>(procs.size()); i++) {
			data = PKB::getModifiesVariablesFromProcedure(procs[i][0]);
			for (int j = 0; j < static_cast<int>(data.size()); j++) {
				output.push_back({ procs[i][0], data[j][0] });
			}
		}
	}

	return output;

}

std::vector<std::vector<int>> PKB::getAllVariableModifiesProcedures() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> vars = PKB::getAllVariables();

	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		data = PKB::getProceduresFromModifiesVariable(vars[i][0]);

		if (data.size() > 0 && data[0].size() > 0) {
			for (int j = 0; j < static_cast<int>(data.size()); j++) {
				output.push_back({ vars[i][0], data[j][0] });
			}
		}
	}

	return output;

}

bool PKB::checkStatementModifiesVariable(int stmt, int varId) {

	std::vector<std::vector<int>> vars = PKB::getModifiesVariablesFromStatement(stmt);
	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		if (vars[i][0] == varId) {
			return true;
		}
	}

	return false;

}

bool PKB::checkProcedureModifiesVariable(int procId, int varId) {

	std::vector<std::vector<int>> vars = PKB::getModifiesVariablesFromProcedure(procId);
	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		if (vars[i][0] == varId) {
			return true;
		}
	}

	return false;

}

/* Next Operations */
std::vector<std::vector<int>> PKB::getNextBefore(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(NEXT_INVERSE_TABLE, stmt);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<int> result = data[0];
		std::vector<std::vector<int>> output;
		for (int i = 0; i < result.size(); i++) {
			output.push_back({result[i]});
		}
		return output;
	}

	std::vector<std::vector<int>> empty;
	return empty;
}

std::vector<std::vector<int>> PKB::getNextAfter(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(NEXT_TABLE, stmt);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<int> result = data[0];
		std::vector<std::vector<int>> output;
		for (int i = 0; i < result.size(); i++) {
			output.push_back({ result[i] });
		}
		return output;
	}

	std::vector<std::vector<int>> empty;
	return empty;
}

std::vector<std::vector<int>> PKB::getNextBeforeStar(int stmt) {

	std::vector<std::vector<int>> output;
	std::set<int> nextStar;
	std::queue<int> next;

	next.empty();

	std::vector<std::vector<int>> data = PKB::getFromTable(NEXT_INVERSE_TABLE, stmt);
	if (data.size() > 0 && data[0].size() > 0) {
		while (data[0].size() > 0) {
			next.push(data[0].back());
			nextStar.insert(data[0].back());
			data[0].pop_back();
		}
		while (next.size() > 0) {
			int nextStatement = next.front();
			next.pop();
			data = PKB::getFromTable(NEXT_INVERSE_TABLE, nextStatement);
			if (data.size() > 0) {
				std::vector<std::vector<int>> tempData = data;
				for (int i = 0; i < tempData[0].size(); i++) {
					int initialSize = nextStar.size();
					int newData = tempData[0][i];
					nextStar.insert(newData);
					if (nextStar.size() > initialSize) {
						next.push(newData);
					}
				}
			}
		}

		std::vector<int> insert(nextStar.begin(), nextStar.end());
		for (int i = 0; i < insert.size(); i++) {
			output.push_back({insert[i]});
		}

		nextStar.clear();
	}
	return output;
}

std::vector<std::vector<int>> PKB::getNextAfterStar(int stmt) {
	std::vector<std::vector<int>> output;
	std::set<int> nextStar;
	std::queue<int> next;

	next.empty();

	std::vector<std::vector<int>> data = PKB::getFromTable(NEXT_TABLE, stmt);
	if (data.size() > 0 && data[0].size() > 0) {
		while (data[0].size() > 0) {
			next.push(data[0].back());
			nextStar.insert(data[0].back());
			data[0].pop_back();
		}
		while (next.size() > 0) {
			int nextStatement = next.front();
			next.pop();
			data = PKB::getFromTable(NEXT_TABLE, nextStatement);
			if (data.size() > 0) {
				std::vector<std::vector<int>> tempData = data;
				for (int i = 0; i < tempData[0].size(); i++) {
					int initialSize = nextStar.size();
					int newData = tempData[0][i];
					nextStar.insert(newData);
					if (nextStar.size() > initialSize) {
						next.push(newData);
					}
				}
			}
		}

		std::vector<int> insert(nextStar.begin(), nextStar.end());
		for (int i = 0; i < insert.size(); i++) {
			output.push_back({ insert[i] });
		}

		nextStar.clear();
	}
	return output;
}

bool PKB::checkNext(int stmt1, int stmt2) {
	std::vector<std::vector<int>> data = PKB::getNextAfter(stmt1);
	if (data.size() > 0) {
		for (int i = 0; i < data.size(); i++) {
			if (stmt2 == data[i][0]) {
				return true;
			}
		}
	}
	return false;
}

bool PKB::checkNextStar(int stmt1, int stmt2) {
	std::vector<std::vector<int>> data = PKB::getNextAfterStar(stmt1);
	if (data.size() > 0) {
		for (int i = 0; i < data.size(); i++) {
			if (stmt2 == data[i][0]) {
				return true;
			}
		}
	}
	return false;
}

std::vector<std::vector<int>> PKB::getAllNext() {

	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[NEXT_TABLE - 1];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[0];
		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()); i++) {
			output.push_back({it->first, stmtList[i]});
		}
	}

	return output;
}

std::vector<std::vector<int>> PKB::getAllNextStar() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> stmts = PKB::getAllStatements();

	for (int i = 0; i < stmts.size(); i++) {
		std::vector<std::vector<int>> stmtList = PKB::getNextAfterStar(stmts[i][0]);
		for (int j = 0; j < stmtList.size(); j++) {
			output.push_back({ stmts[i][0], stmtList[j][0] });
		}
	}

	return output;
}

/* Calls Operations */
std::vector<std::vector<int>> PKB::getCallsBefore(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(CALLS_INVERSE_TABLE, stmt);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<int> result = data[0];
		std::vector<std::vector<int>> output;
		for (int i = 0; i < result.size(); i++) {
			output.push_back({ result[i] });
		}
		return output;
	}

	std::vector<std::vector<int>> empty;
	return empty;
}

std::vector<std::vector<int>> PKB::getCallsAfter(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(CALLS_TABLE, stmt);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<int> result = data[0];
		std::vector<std::vector<int>> output;
		for (int i = 0; i < result.size(); i++) {
			output.push_back({ result[i] });
		}
		return output;
	}

	std::vector<std::vector<int>> empty;
	return empty;
}

std::vector<std::vector<int>> PKB::getCallsBeforeStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(CALLS_STAR_INVERSE_TABLE, stmt);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<int> result = data[0];
		std::vector<std::vector<int>> output;
		for (int i = 0; i < result.size(); i++) {
			output.push_back({ result[i] });
		}
		return output;
	}

	std::vector<std::vector<int>> empty;
	return empty;
}

std::vector<std::vector<int>> PKB::getCallsAfterStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(CALLS_STAR_TABLE, stmt);
	if (static_cast<int>(data.size()) > 0) {
		std::vector<int> result = data[0];
		std::vector<std::vector<int>> output;
		for (int i = 0; i < result.size(); i++) {
			output.push_back({ result[i] });
		}
		return output;
	}

	std::vector<std::vector<int>> empty;
	return empty;
}

bool PKB::checkCalls(int stmt1, int stmt2) {
std::vector<std::vector<int>> data = PKB::getCallsAfter(stmt1);
	if (data.size() > 0) {
		for (int i = 0; i < data.size(); i++) {
			if (stmt2 == data[i][0]) {
				return true;
			}
		}
	}
	return false;
}

bool PKB::checkCallsStar(int stmt1, int stmt2) {
	std::vector<std::vector<int>> data = PKB::getCallsAfterStar(stmt1);
	if (data.size() > 0) {
		for (int i = 0; i < data.size(); i++) {
			if (stmt2 == data[i][0]) {
				return true;
			}
		}
	}
	return false;
}

std::vector<std::vector<int>> PKB::getAllCalls() {

	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[CALLS_TABLE - 1];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[0];
		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()); i++) {
			output.push_back({ it->first, stmtList[i] });
		}
	}

	return output;
}

std::vector<std::vector<int>> PKB::getAllCallsStar() {

	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[CALLS_STAR_TABLE - 1];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[0];
		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()); i++) {
			output.push_back({ it->first, stmtList[i] });
		}
	}

	return output;
}

/* Call Statement Operations */
std::vector<std::vector<int>> PKB::getCallStatementsCallingProcedure(int calledProc) {
	std::vector<std::vector<int>> res = PKB::getFromTable(CALLS_TABLE, calledProc);
	std::vector<std::vector<int>> output;
	if (res.size() > 1) {
		for (int i = 0; i < res[1].size(); i++) {
			output.push_back({ res[1][i] });
		}
	}
	return output;
}
std::vector<std::vector<int>> PKB::getProcedureCalledByCallStatement(int callStmt) {
	std::vector<std::vector<int>> res = PKB::getFromTable(CALL_STATEMENT_TABLE, callStmt);
	std::vector<std::vector<int>> output;
	if (res.size() > 0) {
		for (int i = 0; i < res[0].size(); i++) {
			output.push_back({ res[0][i] });
		}
	}
	return output;
}
std::vector<std::vector<int>> PKB::getAllCallStatementsCallingProcedure() {
	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> procs = PKB::getAllProcedures();
	for (int j = 0; j < procs.size(); j++) {
		std::vector<std::vector<int>> res = PKB::getFromTable(CALLS_TABLE, procs[j][0]);
		if (res.size() > 1) {
			for (int i = 0; i < res[1].size(); i++) {
				output.push_back({ procs[j][0], res[1][i] });
			}
		}
	}
	return output;
}
std::vector<std::vector<int>> PKB::getAllProcedureCalledByCallStatement() {
	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[CALL_STATEMENT_TABLE - 1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		output.push_back({ it->first, it->second[0][0] });
	}
	return output;
}

/* Pattern Operations */
std::vector<std::vector<int>> PKB::getStatementsWithPattern(Pattern p) {
	std::vector<std::vector<int>> result;
	std::vector<int> intermediate;
	if (p.getEntity().type == ASSIGN) {
		
		//all, all
		if (p.getLeftParam().type == ALL && p.getRightParam().type == ALL) {
			return PKB::getAllStatementsWithType(1);
		}
		//right - all
		else if (p.getRightParam().type == ALL) {
			std::vector<std::vector<int>> temp = PKB::getAllStatementsWithType(1);
			for (int i = 0; i < temp.size(); i++) {
				intermediate.push_back(temp[i][0]);
			}
		}
		//right - expr_exact
		else if (p.getRightParam().type == EXPR_EXACT) {
			unordered_map<int, std::vector<string>> table = nameTables[0];
			for (auto it = table.begin(); it != table.end(); ++it) {
				if (it->second.size() > 1) {
					if (it->second[1] == p.getRightParam().value) {
						intermediate.push_back(it->first);
					}
				}
			}
		}
		//right - expr
		else if (p.getRightParam().type == EXPR) {
			unordered_map<int, std::vector<string>> table = nameTables[0];
			std::size_t found;
			for (auto it = table.begin(); it != table.end(); ++it) {
				if (it->second.size() > 1) {
					found = it->second[1].find(p.getRightParam().value);
					if (found != string::npos) {
						intermediate.push_back(it->first);
					}
				}
			}
		}

		//continuing with intermediate
		if (p.getLeftParam().type == VARIABLE) {
			for (int i = 0; i < intermediate.size(); i++) {
				string modifies = PKB::getFromNameTable(PATTERN_TABLE, intermediate[i])[0];
				result.push_back({intermediate[i], PKB::getVariableId(modifies)});
			}
		} 
		else if (p.getLeftParam().type == ALL) {
			for (int i = 0; i < intermediate.size(); i++) {
				result.push_back({ intermediate[i] });
			}
		} 
		else if (p.getLeftParam().type == VAR_IDENT) {
			for (int i = 0; i < intermediate.size(); i++) {
				string modifies = PKB::getFromNameTable(PATTERN_TABLE, intermediate[i])[0];
				if (modifies == p.getLeftParam().value) {
					result.push_back({ intermediate[i] });
				}
			}
		}

		return result;

	}
	else if (p.getEntity().type == IF || p.getEntity().type == WHILE) {

		int entityType = p.getEntity().type;
		//entity(_,_,_) or (_,_)
		std::vector<std::vector<int>> output;
		if (entityType == IF) {
			output = PKB::getAllStatementsWithType(3);
		} else if (entityType == WHILE) {
			output = PKB::getAllStatementsWithType(2);
		}
			
		if (p.getLeftParam().type == VARIABLE || p.getLeftParam().type == VAR_IDENT) {
			for (int i = 0; i < output.size(); i++) {
				string modifies = PKB::getFromNameTable(PATTERN_TABLE, output[i][0])[0];
				if (p.getLeftParam().type == VARIABLE){
					output[i].push_back(PKB::getVariableId(modifies));
				}
				else if (p.getLeftParam().type == VAR_IDENT && p.getLeftParam().value != modifies) {
					output.erase(output.begin() + i, output.begin() + i + 1);
					i--;
				}
			}
			return output;
		}
		else {
			return output;
		}

	}
	return result;
}

/* Affects Operations */
std::vector<std::vector<int>> PKB::getAffectsBefore(int stmt) {

	std::vector<std::vector<int>> data;

	if (PKB::checkStatementHasType(stmt, 1)) {

		std::vector<std::vector<int>> next = PKB::getNextBeforeStar(stmt);
		std::vector<std::vector<int>> variablesUsed = PKB::getUsesVariablesFromStatement(stmt);
		set<int> checkedStmts;
		unordered_map<int, std::vector<int>> maxModifiableVariables;

		while (next.size() > 0) {
			int stmtNo = next.back()[0];
			next.pop_back();
			maxModifiableVariables.insert({ stmtNo, { } });
		}

		std::vector<int> oriUsed;
		for (int i = 0; i < variablesUsed.size(); i++) {
			oriUsed.push_back(variablesUsed[i][0]);
		}

		next = PKB::getNextBefore(stmt);
		for (int i = 0; i < next.size(); i++) {
			maxModifiableVariables.erase(next[i][0]);
			maxModifiableVariables.insert({ next[i][0], oriUsed });
			checkedStmts.insert(next[i][0]);
		}

		while (next.size() > 0) {
			int nextStmt = next.back()[0];
			next.pop_back();

			std::vector<int> modifiableVariables = maxModifiableVariables[nextStmt];
			std::vector<std::vector<int>> thisStmtModifies = PKB::getModifiesVariablesFromStatement(nextStmt);
			std::vector<int> newModifiableVariables;
			if (modifiableVariables.size() > 0) {
				for (int i = 0; i < modifiableVariables.size(); i++) {
					if (thisStmtModifies.size() > 0) {
						if (thisStmtModifies[0][0] == modifiableVariables[i] && (!PKB::checkStatementHasType(nextStmt, 2)) && (!PKB::checkStatementHasType(nextStmt, 3))) {
							if (PKB::checkStatementHasType(nextStmt, 1)) {
								data.push_back({ nextStmt });
							}
						}
						else {
							newModifiableVariables.push_back(modifiableVariables[i]);
						}
					}
					else {
						newModifiableVariables.push_back(modifiableVariables[i]);
					}
				}
				std::vector<std::vector<int>> thisNext = PKB::getNextBefore(nextStmt);
				for (int i = 0; i < thisNext.size(); i++) {

					unordered_map<int, std::vector<int>>::const_iterator got = maxModifiableVariables.find(thisNext[i][0]);
					std::vector<int> tableValues = got->second;
					if (newModifiableVariables.size() > 0) {

						std::vector<int> data = newModifiableVariables;

						for (unsigned int j = 0; j < data.size(); j++) {
							tableValues.push_back(data[j]);
						}
						std::sort(tableValues.begin(), tableValues.end());
						tableValues.erase(unique(tableValues.begin(), tableValues.end()), tableValues.end());

						maxModifiableVariables.erase(thisNext[i][0]);
						maxModifiableVariables.insert({ thisNext[i][0], tableValues });

					}
					if (tableValues.size() > 0) {
						int initSize = checkedStmts.size();
						checkedStmts.insert(thisNext[i][0]);
						if (checkedStmts.size() > initSize) {
							next.push_back({ thisNext[i][0] });
						}
					}
				}
			}
		}
	}

	return data;

}

std::vector<std::vector<int>> PKB::getAffectsAfter(int stmt) {

	std::vector<std::vector<int>> data;

	if (PKB::checkStatementHasType(stmt, 1)) {

		std::vector<std::vector<int>> next = PKB::getNextAfter(stmt);
		std::vector<std::vector<int>> variablesModified = PKB::getModifiesVariablesFromStatement(stmt);
		int variableModified = 0;
		if (variablesModified.size() > 0) {
			variableModified = variablesModified[0][0];
			set<int> checkedStmts;

			while (next.size() > 0) {
				int nextStmt = next.back()[0];
				next.pop_back();

				if (PKB::checkStatementHasType(nextStmt, 1)) {
					std::vector<std::vector<int>> variableUses = PKB::getUsesVariablesFromStatement(nextStmt);
					for (int i = 0; i < variableUses.size(); i++) {
						if (variableUses[i][0] == variableModified) {
							data.push_back({ nextStmt });
						}
					}
				}


				std::vector<std::vector<int>> variableModifies = PKB::getModifiesVariablesFromStatement(nextStmt);
				std::vector<std::vector<int>> newNext;
				if (variableModifies.size() > 0 && variableModifies[0][0] == variableModified && (!PKB::checkStatementHasType(nextStmt, 2)) && (!PKB::checkStatementHasType(nextStmt, 3))) {
					newNext.clear();
				}
				else {
					newNext = PKB::getNextAfter(nextStmt);
				}
				for (int j = 0; j < newNext.size(); j++) {
					int initSize = checkedStmts.size();
					checkedStmts.insert(newNext[j][0]);
					if (checkedStmts.size() > initSize) {
						next.push_back({ newNext[j][0] });
					}
				}
			}
		}
	}

	return data;

}

std::vector<std::vector<int>> PKB::getAffectsBeforeStar(int stmt) {

	std::vector<std::vector<int>> data;

	if (PKB::checkStatementHasType(stmt, 1)) {

		std::vector<std::vector<int>> next = PKB::getNextBeforeStar(stmt);
		std::vector<std::vector<int>> variablesUsed = PKB::getUsesVariablesFromStatement(stmt);
		set<int> checkedStmts;
		unordered_map<int, std::vector<int>> maxModifiableVariables;

		while (next.size() > 0) {
			int stmtNo = next.back()[0];
			next.pop_back();
			maxModifiableVariables.insert({ stmtNo,{} });
		}

		std::vector<int> oriUsed;
		for (int i = 0; i < variablesUsed.size(); i++) {
			oriUsed.push_back(variablesUsed[i][0]);
		}

		next = PKB::getNextBefore(stmt);
		for (int i = 0; i < next.size(); i++) {
			maxModifiableVariables.erase(next[i][0]);
			maxModifiableVariables.insert({ next[i][0], oriUsed });
			checkedStmts.insert(next[i][0]);
		}

		while (next.size() > 0) {
			int nextStmt = next.back()[0];
			next.pop_back();

			std::vector<int> modifiableVariables = maxModifiableVariables[nextStmt];
			std::vector<std::vector<int>> thisStmtModifies = PKB::getModifiesVariablesFromStatement(nextStmt);
			std::vector<int> newModifiableVariables;
			if (modifiableVariables.size() > 0) {
				for (int i = 0; i < modifiableVariables.size(); i++) {
					if (thisStmtModifies.size() > 0) {
						if (thisStmtModifies[0][0] == modifiableVariables[i] && (!PKB::checkStatementHasType(nextStmt, 2)) && (!PKB::checkStatementHasType(nextStmt, 3))) {
							if (PKB::checkStatementHasType(nextStmt, 1)) {
								data.push_back({ nextStmt });
								std::vector<std::vector<int>> thisStmtUses = PKB::getUsesVariablesFromStatement(nextStmt);
								for (int j = 0; j < thisStmtUses.size(); j++) {
									newModifiableVariables.push_back(thisStmtUses[j][0]);
								}
								std::sort(newModifiableVariables.begin(), newModifiableVariables.end());
								newModifiableVariables.erase(unique(newModifiableVariables.begin(), newModifiableVariables.end()), newModifiableVariables.end());
							}
						}
						else {
							newModifiableVariables.push_back(modifiableVariables[i]);
						}
					}
					else {
						newModifiableVariables.push_back(modifiableVariables[i]);
					}
				}
				std::vector<std::vector<int>> thisNext = PKB::getNextBefore(nextStmt);
				for (int i = 0; i < thisNext.size(); i++) {

					unordered_map<int, std::vector<int>>::const_iterator got = maxModifiableVariables.find(thisNext[i][0]);
					std::vector<int> tableValues = got->second;
					if (newModifiableVariables.size() > 0) {

						std::vector<int> data = newModifiableVariables;

						for (unsigned int j = 0; j < data.size(); j++) {
							tableValues.push_back(data[j]);
						}
						std::sort(tableValues.begin(), tableValues.end());
						tableValues.erase(unique(tableValues.begin(), tableValues.end()), tableValues.end());

						maxModifiableVariables.erase(thisNext[i][0]);
						maxModifiableVariables.insert({ thisNext[i][0], tableValues });

					}
					if (tableValues.size() > 0) {
						int initSize = checkedStmts.size();
						checkedStmts.insert(thisNext[i][0]);
						if (checkedStmts.size() > initSize) {
							next.push_back({ thisNext[i][0] });
						}
					}
				}
			}
		}
	}

	return data;

}

std::vector<std::vector<int>> PKB::getAffectsAfterStar(int stmt) {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> allStmtsId;
	unordered_map<int, std::vector<int>> allStmts;
	std::vector<int> potentialAffects;
	std::vector<int> stmts;
	set<int> checkedStmts;
	stack<int> whileStack;
	set<int> completedWhiles;

	int currStmt;
	int firstStmt;

	firstStmt = stmt;
	stack<int> tempWhileStack;
	std::vector<std::vector<int>> parent = PKB::getParent(firstStmt);
	while (parent.size() > 0) {
		if (PKB::checkStatementHasType(parent[0][0], 2)) {
			tempWhileStack.push(parent[0][0]);
		}
		parent = PKB::getParent(parent[0][0]);
	}
	while (tempWhileStack.size() > 0) {
		whileStack.push(tempWhileStack.top());
		tempWhileStack.pop();
	}

	vector<int> variableAffect;
	allStmtsId = PKB::getNextAfterStar(firstStmt);
	allStmtsId.push_back({ firstStmt });
	variableAffect.clear();
	for (int i = 0; i < allStmtsId.size(); i++) {
		allStmts.insert({ allStmtsId[i][0], variableAffect });
	}

	queue<int> next;
	next.push(firstStmt);
	while (next.size() > 0) {
		currStmt = next.front();
		next.pop();

		variableAffect = allStmts[currStmt];

		if (PKB::checkStatementHasType(currStmt, 1)) {
			bool linkAffect;
			if (checkedStmts.size() > 0) {
				linkAffect = false;
			}
			else {
				linkAffect = true;
			}

			/* What statements this statement uses */
			std::vector<std::vector<int>> variableUses = PKB::getUsesVariablesFromStatement(currStmt);
			potentialAffects = allStmts[currStmt]; 
			for (int i = 0; i < variableUses.size(); i++) {
				if (potentialAffects.size() > 0) {
					for (int j = 0; j < potentialAffects.size(); j++) {
						if (variableUses[i][0] == potentialAffects[j]) {
							output.push_back({currStmt});
							std::sort(output.begin(), output.end());
							output.erase(unique(output.begin(), output.end()), output.end());
							linkAffect = true;
						}
					}
				}
			}

			/* What statements this statement modifies */
			std::vector<std::vector<int>> variableModifies = PKB::getModifiesVariablesFromStatement(currStmt);
			for (int i = 0; i < variableModifies.size(); i++) {
				for (int j = 0; j < potentialAffects.size(); j++) {
					if (variableModifies[i][0] == potentialAffects[j]) {
						if (!linkAffect) {
							potentialAffects.erase(potentialAffects.begin() + j);
						}
					}
				}
				if (linkAffect) {
					potentialAffects.push_back({ variableModifies[i][0] });
					std::sort(potentialAffects.begin(), potentialAffects.end());
					potentialAffects.erase(unique(potentialAffects.begin(), potentialAffects.end()), potentialAffects.end());
				}
			}

		}
		else if (PKB::checkStatementHasType(currStmt, 2)) {
			set<int>::iterator it = completedWhiles.find(currStmt);
			if (it == completedWhiles.end()) {
				whileStack.push(currStmt);
			}
		}

		/* Push to next */
		std::vector<std::vector<int>> newNext;

		newNext = PKB::getNextAfter(currStmt);
		for (int j = 0; j < newNext.size(); j++) {
			if (!(PKB::checkStatementHasType(currStmt, 2) && PKB::checkStatementHasType(newNext[j][0], 2))) {
				int initSize = checkedStmts.size();
				checkedStmts.insert(newNext[j][0]);

				std::vector<int> alreadyInserted = allStmts[newNext[j][0]];
				std::vector<int> toInsert = potentialAffects;
				for (int k = 0; k < toInsert.size(); k++) {
					alreadyInserted.push_back(toInsert[k]);
				}
				std::sort(alreadyInserted.begin(), alreadyInserted.end());
				alreadyInserted.erase(unique(alreadyInserted.begin(), alreadyInserted.end()), alreadyInserted.end());
				allStmts.erase(newNext[j][0]);
				allStmts.insert({ newNext[j][0], alreadyInserted });

				if (checkedStmts.size() > initSize) {
					next.push({ newNext[j][0] });
				}
			}
		}

		while (next.size() == 0 && whileStack.size() > 0) {
			int returnWhile = whileStack.top();
			whileStack.pop();
			int initialSize = completedWhiles.size();
			completedWhiles.insert(returnWhile);
			if (completedWhiles.size() > initialSize) {

				if (whileStack.size() > 0) {
					std::vector<int> alreadyInserted = allStmts[whileStack.top()];
					std::vector<int> toInsert = allStmts[returnWhile];
					for (int k = 0; k < toInsert.size(); k++) {
						alreadyInserted.push_back(toInsert[k]);
					}
					std::sort(alreadyInserted.begin(), alreadyInserted.end());
					alreadyInserted.erase(unique(alreadyInserted.begin(), alreadyInserted.end()), alreadyInserted.end());
					allStmts.erase(whileStack.top());
					allStmts.insert({ whileStack.top(), alreadyInserted });
				}

				std::vector<std::vector<int>> children = PKB::getChildrenStar(returnWhile);
				for (int i = 0; i < children.size(); i++) {
					checkedStmts.erase(children[i][0]);
				}
				checkedStmts.erase(returnWhile);
				next.push(returnWhile);
			}
		}

	}





	return output;

}

bool PKB::checkAffects(int stmt1, int stmt2) {

	std::vector<std::vector<int>> data;

	if (PKB::checkStatementHasType(stmt1, 1) && PKB::checkStatementHasType(stmt2, 1)) {

		std::vector<std::vector<int>> next = PKB::getNextAfter(stmt1);
		std::vector<std::vector<int>> variablesModified = PKB::getModifiesVariablesFromStatement(stmt1);
		int variableModified = 0;
		if (variablesModified.size() > 0) {
			variableModified = variablesModified[0][0];
			set<int> checkedStmts;

			while (next.size() > 0) {
				int nextStmt = next.back()[0];
				next.pop_back();

				if (PKB::checkStatementHasType(nextStmt, 1)) {
					std::vector<std::vector<int>> variableUses = PKB::getUsesVariablesFromStatement(nextStmt);
					for (int i = 0; i < variableUses.size(); i++) {
						if (variableUses[i][0] == variableModified && nextStmt == stmt2) {
							return true;
						}
					}
				}


				std::vector<std::vector<int>> variableModifies = PKB::getModifiesVariablesFromStatement(nextStmt);
				std::vector<std::vector<int>> newNext;
				if (variableModifies.size() > 0 && variableModifies[0][0] == variableModified && (!PKB::checkStatementHasType(nextStmt, 2)) && (!PKB::checkStatementHasType(nextStmt, 3))) {
					newNext.clear();
				}
				else {
					newNext = PKB::getNextAfter(nextStmt);
				}
				for (int j = 0; j < newNext.size(); j++) {
					int initSize = checkedStmts.size();
					checkedStmts.insert(newNext[j][0]);
					if (checkedStmts.size() > initSize) {
						next.push_back({ newNext[j][0] });
					}
				}
			}
		}
	}

	return false;

}

bool PKB::checkAffectsStar(int stmt1, int stmt2) {

	std::vector<std::vector<int>> data;

	unordered_map<int, std::vector<std::vector<int>>> allStmts;
	std::vector<std::vector<int>> allStmtsId = PKB::getNextAfterStar(stmt1);
	allStmtsId.push_back({ stmt1 });
	std::vector<std::vector<int>> variableAffect;
	for (int i = 0; i < allStmtsId.size(); i++) {
		allStmts.insert({ allStmtsId[i][0], variableAffect });
	}

	if (PKB::checkStatementHasType(stmt1, 1) && PKB::checkStatementHasType(stmt2, 1)) {

		std::vector<std::vector<int>> next = PKB::getNextAfter(stmt1);
		std::vector<std::vector<int>> variablesModified = PKB::getModifiesVariablesFromStatement(stmt1);
		
		allStmts.erase(stmt1);
		allStmts.insert({ stmt1, variablesModified });

		for (int i = 0; i < next.size(); i++) {
			allStmts.erase(next[i][0]);
			allStmts.insert({ next[i][0], variablesModified });
		}

		int variableModified = 0;
		if (variablesModified.size() > 0) {
			set<int> checkedStmts;

			while (next.size() > 0) {
				int nextStmt = next.back()[0];
				next.pop_back();
				variablesModified = allStmts[nextStmt];

				if (PKB::checkStatementHasType(nextStmt, 1)) {
					std::vector<std::vector<int>> variableUses = PKB::getUsesVariablesFromStatement(nextStmt);
					for (int i = 0; i < variableUses.size(); i++) {
						for (int j = 0; j < variablesModified.size(); j++) {
							variableModified = variablesModified[j][0];
							if (variableUses[i][0] == variableModified && nextStmt == stmt2) {
								return true;
							}
							else if (variableUses[i][0] == variableModified) {
								variablesModified.push_back({PKB::getModifiesVariablesFromStatement(nextStmt)[0][0]});
								break;
							}
						}
					}
				}

				allStmts.erase(nextStmt);
				allStmts.insert({ nextStmt, variablesModified });

				std::vector<std::vector<int>> newNext;
				if (variablesModified.size() == 0) {
					newNext.clear();
				}
				else {
					newNext = PKB::getNextAfter(nextStmt);
				}
				for (int j = 0; j < newNext.size(); j++) {
					int initSize = checkedStmts.size();
					checkedStmts.insert(newNext[j][0]);
					if (checkedStmts.size() > initSize) {
						next.push_back({ newNext[j][0] });

						std::vector<std::vector<int>> presentVariablesModified = allStmts[newNext[j][0]];
						for (int k = 0; k < presentVariablesModified.size(); k++) {
							variablesModified.push_back({presentVariablesModified[k][0]});
						}
						std::sort(variablesModified.begin(), variablesModified.end());
						variablesModified.erase(unique(variablesModified.begin(), variablesModified.end()), variablesModified.end());

						allStmts.erase(newNext[j][0]);
						allStmts.insert({ newNext[j][0], variablesModified });
					}
				}
			}
		}
	}

	return false;

}

std::vector<std::vector<int>> PKB::getAllAffects() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> allStmtsId;
	unordered_map<int, unordered_map<int, std::vector<int>>> allStmts;
	std::vector<int> stmts;
	set<int> checkedStmts;
	stack<int> whileStack;
	set<int> completedWhiles;

	int currStmt;
	int firstStmt;
	std::vector<std::vector<int>> procs = PKB::getAllProcedures();
	

	

	for (int i = 0; i < static_cast<int>(procs.size()); i++) {
		firstStmt = PKB::getFromTable(STATEMENT_LIST_TABLE, PKB::getFromTable(PROC_INFO_TABLE, procs[i][0])[0][0])[1][0];
		unordered_map<int, std::vector<int>> lastModified;
		lastModified.clear();
		allStmtsId = PKB::getNextAfterStar(firstStmt);
		allStmtsId.push_back({ firstStmt });
		for (int i = 0; i < allStmtsId.size(); i++) {
			allStmts.insert({allStmtsId[i][0], lastModified});
		}

		queue<int> next;
		next.push(firstStmt);
		while (next.size() > 0) {
			currStmt = next.front();
			next.pop();

			lastModified = allStmts[currStmt];
			
			if (PKB::checkStatementHasType(currStmt, 1)) {
				/* What statements this statement uses */
				std::vector<std::vector<int>> variableUses = PKB::getUsesVariablesFromStatement(currStmt);
				for (int i = 0; i < variableUses.size(); i++) {
					unordered_map<int, std::vector<int>>::const_iterator got = lastModified.find(variableUses[i][0]);
					if (got != lastModified.end()) {
						std::vector<int> stmtsAffects = got->second;
						for (int j = 0; j < stmtsAffects.size(); j++) {
							output.push_back({stmtsAffects[j], currStmt});
							std::sort(output.begin(), output.end());
							output.erase(unique(output.begin(), output.end()), output.end());

						}
					}
				}

				/* What statements this statement affects */
				std::vector<std::vector<int>> variableModifies = PKB::getModifiesVariablesFromStatement(currStmt);
				for (int i = 0; i < variableModifies.size(); i++) {
					unordered_map<int, std::vector<int>>::const_iterator got = lastModified.find(variableModifies[i][0]);
					if (got == lastModified.end()) {
						lastModified.insert({ variableModifies[i][0], {currStmt} });
					}
					else {
						stmts = { currStmt };
						lastModified.erase({ variableModifies[i][0] });
						lastModified.insert({ variableModifies[i][0], stmts });
					}
				}

			}
			else if (PKB::checkStatementHasType(currStmt, 2)) {
				set<int>::iterator it = completedWhiles.find(currStmt);
				if (it == completedWhiles.end()) {
					whileStack.push(currStmt);
				}
			}

			/* Push to next */
			std::vector<std::vector<int>> newNext;

			newNext = PKB::getNextAfter(currStmt);
			for (int j = 0; j < newNext.size(); j++) {
				if (!(PKB::checkStatementHasType(currStmt, 2) && PKB::checkStatementHasType(newNext[j][0], 2))) {
					int initSize = checkedStmts.size();
					checkedStmts.insert(newNext[j][0]);

					auto result = lastModified;
					unordered_map<int, std::vector<int>> nextLastModified = allStmts[newNext[j][0]];
					result.insert(nextLastModified.begin(), nextLastModified.end());
					bool edited = false;
					for (auto it = nextLastModified.begin(); it != nextLastModified.end(); ++it) {
						std::vector<int> alreadyInserted = result[it->first];
						std::vector<int> toInsert = it->second;
						for (int k = 0; k < toInsert.size(); k++) {
							alreadyInserted.push_back(toInsert[k]);
						}
						std::sort(alreadyInserted.begin(), alreadyInserted.end());
						alreadyInserted.erase(unique(alreadyInserted.begin(), alreadyInserted.end()), alreadyInserted.end());
						result.erase(it->first);
						result.insert({ it->first, alreadyInserted });
					}

					allStmts.erase(newNext[j][0]);
					allStmts.insert({ newNext[j][0], result });

					if (checkedStmts.size() > initSize) {
						next.push({ newNext[j][0] });
					}
				}
			}

			while (next.size() == 0 && whileStack.size() > 0) {
				int returnWhile = whileStack.top();
				whileStack.pop();
				int initialSize = completedWhiles.size();
				completedWhiles.insert(returnWhile);
				if (completedWhiles.size() > initialSize) {

					if (whileStack.size() > 0) {
						auto result = allStmts[returnWhile];
						unordered_map<int, std::vector<int>> nextLastModified = allStmts[whileStack.top()];
						result.insert(nextLastModified.begin(), nextLastModified.end());
						bool edited = false;
						for (auto it = nextLastModified.begin(); it != nextLastModified.end(); ++it) {
							std::vector<int> alreadyInserted = result[it->first];
							std::vector<int> toInsert = it->second;
							for (int k = 0; k < toInsert.size(); k++) {
								alreadyInserted.push_back(toInsert[k]);
							}
							std::sort(alreadyInserted.begin(), alreadyInserted.end());
							alreadyInserted.erase(unique(alreadyInserted.begin(), alreadyInserted.end()), alreadyInserted.end());
							result.erase(it->first);
							result.insert({ it->first, alreadyInserted });
						}

						allStmts.erase(whileStack.top());
						allStmts.insert({ whileStack.top(), result });
					}

					std::vector<std::vector<int>> children = PKB::getChildrenStar(returnWhile);
					for (int i = 0; i < children.size(); i++) {
						checkedStmts.erase(children[i][0]);
					}
					checkedStmts.erase(returnWhile);
					next.push(returnWhile);
				}
			}

		}



		
	}

	return output;

}

std::vector<std::vector<int>> PKB::getAllAffectsStar() {

	std::vector<std::vector<int>> output;
	std::vector<std::vector<int>> allStmtsId;
	unordered_map<int, unordered_map<int, std::vector<int>>> allStmts;
	std::vector<int> stmts;
	set<int> checkedStmts;
	stack<int> whileStack;
	set<int> completedWhiles;

	int currStmt;
	int firstStmt;
	std::vector<std::vector<int>> procs = PKB::getAllProcedures();




	for (int i = 0; i < static_cast<int>(procs.size()); i++) {
		firstStmt = PKB::getFromTable(STATEMENT_LIST_TABLE, PKB::getFromTable(PROC_INFO_TABLE, procs[i][0])[0][0])[1][0];
		unordered_map<int, std::vector<int>> lastModified;
		lastModified.clear();
		allStmtsId = PKB::getNextAfterStar(firstStmt);
		allStmtsId.push_back({ firstStmt });
		for (int i = 0; i < allStmtsId.size(); i++) {
			allStmts.insert({ allStmtsId[i][0], lastModified });
		}

		queue<int> next;
		next.push(firstStmt);
		while (next.size() > 0) {
			currStmt = next.front();
			next.pop();

			lastModified = allStmts[currStmt];

			if (PKB::checkStatementHasType(currStmt, 1)) {
				/* What statements this statement uses */

				unordered_map<int, std::vector<int>> toAdd;
				std::vector<std::vector<int>> variableUses = PKB::getUsesVariablesFromStatement(currStmt);
				for (int i = 0; i < variableUses.size(); i++) {
					unordered_map<int, std::vector<int>>::const_iterator got = lastModified.find(variableUses[i][0]);
					if (got != lastModified.end()) {
						std::vector<int> stmtsAffects = got->second;
						for (int j = 0; j < stmtsAffects.size(); j++) {
							output.push_back({ stmtsAffects[j], currStmt });
							std::sort(output.begin(), output.end());
							output.erase(unique(output.begin(), output.end()), output.end());
							
							int variableInQn = PKB::getModifiesVariablesFromStatement(currStmt)[0][0];
							unordered_map<int, std::vector<int>>::const_iterator findToAdd = toAdd.find(variableInQn);
							if (findToAdd != toAdd.end()) {
								stmts = toAdd[variableInQn];
								stmts.push_back({ stmtsAffects[j] });
								std::sort(stmts.begin(), stmts.end());
								stmts.erase(unique(stmts.begin(), stmts.end()), stmts.end());

								toAdd.erase({ variableInQn });
								toAdd.insert({ variableInQn, stmts });
							}
							else {
								toAdd.insert({ variableInQn, { stmtsAffects[j] } });
							}
						}
					}
				}

				/* What statements this statement affects */
				std::vector<std::vector<int>> variableModifies = PKB::getModifiesVariablesFromStatement(currStmt);
				for (int i = 0; i < variableModifies.size(); i++) {
					unordered_map<int, std::vector<int>>::const_iterator got = lastModified.find(variableModifies[i][0]);
					
					stmts = { currStmt };
					unordered_map<int, std::vector<int>>::const_iterator findToAdd = toAdd.find(variableModifies[i][0]);
					if (findToAdd != toAdd.end()) {
						stmts = findToAdd->second;
						stmts.push_back(currStmt);
					}
					
					if (got == lastModified.end()) {
						lastModified.insert({ variableModifies[i][0], stmts });
					}
					else {
						lastModified.erase({ variableModifies[i][0] });
						lastModified.insert({ variableModifies[i][0], stmts });
					}

				}

			}
			else if (PKB::checkStatementHasType(currStmt, 2)) {
				set<int>::iterator it = completedWhiles.find(currStmt);
				if (it == completedWhiles.end()) {
					whileStack.push(currStmt);
				}
			}

			/* Push to next */
			std::vector<std::vector<int>> newNext;

			newNext = PKB::getNextAfter(currStmt);
			for (int j = 0; j < newNext.size(); j++) {
				if (!(PKB::checkStatementHasType(currStmt, 2) && PKB::checkStatementHasType(newNext[j][0], 2))) {
					int initSize = checkedStmts.size();
					checkedStmts.insert(newNext[j][0]);

					auto result = lastModified;
					unordered_map<int, std::vector<int>> nextLastModified = allStmts[newNext[j][0]];
					result.insert(nextLastModified.begin(), nextLastModified.end());
					bool edited = false;
					for (auto it = nextLastModified.begin(); it != nextLastModified.end(); ++it) {
						std::vector<int> alreadyInserted = result[it->first];
						std::vector<int> toInsert = it->second;
						for (int k = 0; k < toInsert.size(); k++) {
							alreadyInserted.push_back(toInsert[k]);
						}
						std::sort(alreadyInserted.begin(), alreadyInserted.end());
						alreadyInserted.erase(unique(alreadyInserted.begin(), alreadyInserted.end()), alreadyInserted.end());
						result.erase(it->first);
						result.insert({ it->first, alreadyInserted });
					}

					allStmts.erase(newNext[j][0]);
					allStmts.insert({ newNext[j][0], result });

					if (checkedStmts.size() > initSize) {
						next.push({ newNext[j][0] });
					}
				}
			}

			while (next.size() == 0 && whileStack.size() > 0) {
				int returnWhile = whileStack.top();
				whileStack.pop();
				int initialSize = completedWhiles.size();
				completedWhiles.insert(returnWhile);
				if (completedWhiles.size() > initialSize) {

					if (whileStack.size() > 0) {
						auto result = allStmts[returnWhile];
						unordered_map<int, std::vector<int>> nextLastModified = allStmts[whileStack.top()];
						result.insert(nextLastModified.begin(), nextLastModified.end());
						bool edited = false;
						for (auto it = nextLastModified.begin(); it != nextLastModified.end(); ++it) {
							std::vector<int> alreadyInserted = result[it->first];
							std::vector<int> toInsert = it->second;
							for (int k = 0; k < toInsert.size(); k++) {
								alreadyInserted.push_back(toInsert[k]);
							}
							std::sort(alreadyInserted.begin(), alreadyInserted.end());
							alreadyInserted.erase(unique(alreadyInserted.begin(), alreadyInserted.end()), alreadyInserted.end());
							result.erase(it->first);
							result.insert({ it->first, alreadyInserted });
						}

						allStmts.erase(whileStack.top());
						allStmts.insert({ whileStack.top(), result });
					}

					std::vector<std::vector<int>> children = PKB::getChildrenStar(returnWhile);
					for (int i = 0; i < children.size(); i++) {
						checkedStmts.erase(children[i][0]);
					}
					checkedStmts.erase(returnWhile);
					next.push(returnWhile);
				}
			}

		}




	}

	return output;

}

/* Constant Operations */
std::vector<std::vector<int>> PKB::getStatementsWithConstant(int constant) {

	std::vector<std::vector<int>> data;
	std::vector<std::vector<int>> table = PKB::getFromTable(CONST_TABLE, constant);

	if (static_cast<int>(table.size()) > 0) {
		for (int i = 0; i < table[0].size(); i++) {
			data.push_back({table[0][i]});
		}
	}
	return data;

}

std::vector<std::vector<int>> PKB::getAllConstants() {
	std::vector<std::vector<int>> data;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[CONST_TABLE - 1];

	for (auto it = table.begin(); it != table.end(); ++it) {
		data.push_back({ it->first });
	}

	return data;
}
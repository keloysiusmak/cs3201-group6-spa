#pragma once

#include <algorithm>

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
	case 1:
		tableValuesCount = 4;
		break;
	case 2:
		tableValuesCount = 3;
		break;
	case 3:
		tableValuesCount = 3;
		break;
	case 4:
		tableValuesCount = 2;
		break;
	case 5:
		tableValuesCount = 2;
		break;
	case 6:
		tableValuesCount = 1;
		break;
	case 7:
		tableValuesCount = 2;
		break;
	case 8:
		tableValuesCount = 1;
		break;
	case 9:
		tableValuesCount = 1;
		break;
	case 10:
		tableValuesCount = 1;
		break;
	case 11:
		tableValuesCount = 1;
		break;
	case 12:
		tableValuesCount = 1;
		break;
	case 13:
		tableValuesCount = 1;
		break;
	case 14:
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

std::vector<std::vector<int>> PKB::getFromTable(int table_id, int key_id)
{
	if (table_id < 1 || table_id > NEXT_STAR_INVERSE_TABLE) {
		std::vector<std::vector<int>> data;
		return data;
	} else {
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

int PKB::getProcedureId(std::string proc_name) {
	unordered_map<int, std::vector<string>> table = nameTables[PROC_TABLE - 15];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0] == proc_name) {
			return it->first;
		}
	}

	return 0;
}

int PKB::getVariableId(std::string var_name) {
	unordered_map<int, std::vector<string>> table = nameTables[VAR_TABLE - 15];
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
			std::vector<std::vector<int>> recur_output = PKB::getChildrenStar(data[i][0]);
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
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(NEXT_STAR_INVERSE_TABLE, stmt);
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

std::vector<std::vector<int>> PKB::getNextAfterStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(NEXT_STAR_TABLE, stmt);
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
	unordered_map<int, std::vector<std::vector<int>>> table = tables[NEXT_STAR_TABLE - 1];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[0];
		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()); i++) {
			output.push_back({ it->first, stmtList[i] });
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
				if (it->second[1] == p.getRightParam().value) {
					intermediate.push_back(it->first);
				}
			}
		}
		//right - expr
		else if (p.getRightParam().type == EXPR) {
			unordered_map<int, std::vector<string>> table = nameTables[0];
			std::size_t found;
			for (auto it = table.begin(); it != table.end(); ++it) {
				found = it->second[1].find(p.getRightParam().value);
				if (found != string::npos) {
					intermediate.push_back(it->first);
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
		else if (p.getLeftParam().type == IDENT) {
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
		if (p.getLeftParam().type == ALL && p.getRightParam().type == ALL) {
			if (entityType == IF) {
				return PKB::getAllStatementsWithType(1);
			} else if (entityType == WHILE) {
				return PKB::getAllStatementsWithType(1);
			}
		}

	}
	return result;
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
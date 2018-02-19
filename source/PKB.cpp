#pragma once

using namespace std;

#include "PKB.h"

/* PKB Operations */
bool PKB::insertToTable(int table_id, int key_id, std::vector<std::vector<int>> value)
{
	unordered_map<int, std::vector<std::vector<int>>> table = tables[table_id];
	unordered_map<int, std::vector<std::vector<int>>>::const_iterator got = table.find(key_id);

	/* Row does not exist */
	if (got == table.end()) {
		int tableValuesCount;
		switch (table_id) {
			case 1:
				tableValuesCount = 4;
				break;
			case 2:
				tableValuesCount = 5;
				break;
			case 3:
				tableValuesCount = 2;
				break;
			case 4:
				tableValuesCount = 2;
				break;
			case 5:
				tableValuesCount = 2;
				break;
			case 6:
				tableValuesCount = 2;
				break;
			case 7:
				tableValuesCount = 1;
				break;
			default:
				return false;
		}

		/* Return false if the supplied arguments does not equal to the number of columns specified in the PKB documentation*/
		if (static_cast<int>(value.size()) != tableValuesCount) {
			return false;
		}

		std::vector<std::vector<int>> tableValues;
		for (int i = 0; i < tableValuesCount; i++) {
			tableValues.push_back(value[i]);
		}
		
		tables[table_id].insert({ key_id, tableValues });
	}
	/* Row exists */
	else {
		std::vector<std::vector<int>> tableValues = got->second;
		std::vector<int> data;

		for (unsigned int i = 0; i < static_cast<int>(value.size()); i++) {
			data = tableValues[i];

			/* Insert every element in vector */
			for (unsigned int j = 0; j < static_cast<int>(value[i].size()); j++) {
				data.push_back(value[i][j]);
			}
			tableValues[i] = data;
		}

		tables[table_id].insert({ key_id, tableValues });
	}
	return true;
}

bool PKB::insertToNameTable(int table_id, int key_id, std::vector<std::vector<std::string>> value)
{
	unordered_map<int, std::vector<std::vector<std::string>>> table = nameTables[table_id];
	unordered_map<int, std::vector<std::vector<std::string>>>::const_iterator got = table.find(key_id);

	/* Row does not exist */
	if (got == table.end()) {

		if (table_id != 8 && table_id != 9) {
			return false;
		}
		int tableValuesCount = 1;

		/* Return false if the supplied arguments does not equal to the number of columns specified in the PKB documentation*/
		if (static_cast<int>(value.size()) != tableValuesCount)
			return false;

		std::vector<std::vector<std::string>> tableValues;
		for (int i = 0; i < tableValuesCount; i++) {
			tableValues.push_back(value[i]);
		}

		nameTables[table_id].insert({ key_id, tableValues });
	}
	/* Row exists */
	else {
		std::vector<std::vector<std::string>> tableValues = got->second;
		std::vector<std::string> data;

		for (unsigned int i = 0; i < static_cast<int>(value.size()); i++) {
			data = tableValues[i];

			/* Insert every element in vector */
			for (unsigned int j = 0; j < static_cast<int>(value[i].size()); j++) {
				data.push_back(value[i][j]);
			}
			tableValues[i] = data;
		}

		nameTables[table_id].insert({ key_id, tableValues });
	}
	return true;
}

std::vector<std::vector<int>> PKB::getFromTable(int table_id, int key_id)
{
	unordered_map<int, std::vector<std::vector<int>>> table = tables[table_id - 1];
	std::unordered_map<int, std::vector<std::vector<int>>>::const_iterator got = table.find(key_id);

	/* Row does not exist */
	if (got == table.end()) {
		std::vector<std::vector<int>> data;
		return data;
	}
	else {
		return got->second;
	}
}

std::vector<std::vector<std::string>> PKB::getFromNameTable(int table_id, int key_id)
{
	unordered_map<int, std::vector<std::vector<std::string>>> table = nameTables[table_id - 8];
	std::unordered_map<int, std::vector<std::vector<std::string>>>::const_iterator got = table.find(key_id);

	/* Row does not exist */
	if (got == table.end()) {
		std::vector<std::vector<std::string>> data;
		return data;
	}
	else {
		return got->second;
	}
}

/* Accessor Operations */
std::string PKB::getProcedureName(int proc_id) {
	std::vector<std::vector<std::string>> data;
	data = PKB::getFromNameTable(8, proc_id);

	return data[0][0];
}

std::string PKB::getVariableName(int var_id) {
	std::vector<std::vector<std::string>> data;
	data = PKB::getFromNameTable(9, var_id);

	return data[0][0];
}

int PKB::getProcedureId(std::string proc_name) {
	unordered_map<int, std::vector<std::vector<std::string>>> table = nameTables[0];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0][0] == proc_name) {
			return it->first;
		}
	}

	return 0;
}

int PKB::getVariableId(std::string var_name) {
	unordered_map<int, std::vector<std::vector<std::string>>> table = nameTables[1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0][0] == var_name) {
			return it->first;
		}
	}

	return 0;
}

/* Follows Operations */
int PKB::getFollowsBefore(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	int parentID = data[0][0];
	data = PKB::getFromTable(2, parentID);
	
	for (unsigned int i = 0; i < static_cast<int>(data[0].size()); i++) {
		if (data[0][i] == stmt) {
			return ((i - 1) < 0) ? 0 : data[0][i - 1];
			break;
		}
	}
	
	return 0;
}

int PKB::getFollowsAfter(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	int parentID = data[0][0];
	data = PKB::getFromTable(2, parentID);
	
	for (unsigned int i = 0; i < static_cast<int>(data[0].size()); i++) {
		if (data[0][i] == stmt) {
			return ((i + 1) == static_cast<int>(data[0].size())) ? 0 : data[0][i + 1];
			break;
		}
	}
	
	return 0;
}

std::vector<int> PKB::getFollowsBeforeStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	int parentID = data[0][0];
	data = PKB::getFromTable(2, parentID);

	for (unsigned int i = 0; i < static_cast<int>(data[0].size()); i++) {
		if (data[0][i] == stmt) {
			data[0].erase(data[0].begin() + i, data[0].begin() + static_cast<int>(data[0].size()));
			return data[0];
			break;
		}
	}
}

std::vector<int> PKB::getFollowsAfterStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	int parentID = data[0][0];
	data = PKB::getFromTable(2, parentID);

	for (unsigned int i = 0; i < static_cast<int>(data[0].size()); i++) {
		if (data[0][i] == stmt) {
			data[0].erase(data[0].begin(), data[0].begin() + i);
			return data[0];
			break;
		}
	}
}

bool PKB::checkFollows(int stmt1, int stmt2) {
	return (stmt2 == PKB::getFollowsAfter(stmt1));
}

bool PKB::checkFollowsStar(int stmt1, int stmt2) {
	std::vector<int> stmtList = PKB::getFollowsAfterStar(stmt1);
	
	for (unsigned int i = 0; i < static_cast<int>(stmtList.size()); i++) {
		if (stmt2 == stmtList[i]) {
			return true;
		}
	}

	return false;
}

std::vector<std::vector<int>> PKB::getAllFollows() {

	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[2];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[0];
		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()) - 1; i++) {
			std::vector<int> newOutput;
			newOutput.push_back(it->second[0][i]);
			newOutput.push_back(it->second[0][i + 1]);
			output.push_back(newOutput);
		}
	}

	return output;
}

unordered_map<int, std::vector<int>> PKB::getAllFollowsStar() {

	unordered_map<int, std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[2];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[0];
		std::vector<int> output_stmt;

		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()) - 1; i++) {
			for (unsigned int j = i; j < static_cast<int>(stmtList.size()); j++) {
				output_stmt.push_back(stmtList[i]);
			}
			output.insert({stmtList[i], output_stmt});
			output_stmt.clear();
		}
	}

	return output;
}

/* Parent Operations */
int PKB::getParent(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	return data[0][0];
}

std::vector<int> PKB::getParentStar(int stmt) {
	std::vector<int> output;
	int parent = PKB::getParent(stmt);
	
	while (parent != 0) {
		output.push_back(parent);
		parent = PKB::getParent(stmt);
	}

	return output;
}

std::vector<int> PKB::getChildren(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(2, stmt);
	return data[0];
}

std::vector<int> PKB::getChildrenStar(int stmt) {
	std::vector<int> output;
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(2, stmt);

	for (unsigned int i = 0; i < static_cast<int>(data[0].size()); i++) {
		output.push_back(data[0][i]);
		if (static_cast<int>(PKB::getChildren(data[0][i]).size()) > 0) {
			std::vector<int> recur_output = PKB::getChildrenStar(data[0][i]);
			for (unsigned int j = 0; j < static_cast<int>(recur_output.size()); j++) {
				output.push_back(recur_output[j]);
			}
		}
	}

	return output;
}

bool PKB::checkParent(int stmt1, int stmt2) {
	std::vector<int> children = PKB::getChildren(stmt1);

	for (unsigned int i = 0; i < static_cast<int>(children.size()); i++) {
		if (stmt2 == children[i]) {
			return true;
		}
	}

	return false;
}

bool PKB::checkParentStar(int stmt1, int stmt2) {
	std::vector<int> children = PKB::getChildrenStar(stmt1);

	for (unsigned int i = 0; i < static_cast<int>(children.size()); i++) {
		if (stmt2 == children[i]) {
			return true;
		}
	}

	return false;
}

std::vector<std::vector<int>> PKB::getAllParent() {

	std::vector<std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[2];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[0];
		for (unsigned int i = 0; i < static_cast<int>(stmtList.size()); i++) {
			std::vector<int> newOutput;
			newOutput.push_back(it->second[0][i]);
			newOutput.push_back(it->second[0][i + 1]);
			output.push_back(newOutput);
		}
	}

	return output;
}

unordered_map<int, std::vector<int>> PKB::getAllParentStar() {

	unordered_map<int, std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[2];
	
	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = PKB::getChildrenStar(it->first);
		output.insert({ it->first, stmtList });
	}

	return output;
}
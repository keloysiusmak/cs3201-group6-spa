#pragma once

using namespace std;

#include "PKB.h"

/* PKB Operations */
bool PKB::insertToTable(int table_id, std::string key_id, std::vector<std::vector<std::string>> value)
{
	unordered_map<std::string, std::vector<std::vector<std::string>>> table = tables[table_id];
	std::unordered_map<std::string, std::vector<std::vector<std::string>>>::const_iterator got = table.find(key_id);

	/* Row does not exist */
	if (got == table.end()) {
		int tableValuesCount;
		switch (table_id) {
			case 1:
				tableValuesCount = 4;
			case 2:
				tableValuesCount = 2;
			case 3:
				tableValuesCount = 2;
			case 4:
				tableValuesCount = 2;
			case 5:
				tableValuesCount = 2;
			case 6:
				tableValuesCount = 2;
			case 7:
				tableValuesCount = 1;
			default:
				return false;
		}

		/* Return false if the supplied arguments does not equal to the number of columns specified in the PKB documentation*/
		if (value.size() != tableValuesCount)
			return false;

		std::vector<std::vector<std::string>> tableValues;
		for (int i = 0; i < tableValuesCount; i++) {
			tableValues.push_back(value[i]);
		}
		
		table.insert({ key_id, tableValues });
	}
	/* Row exists */
	else {
		std::vector<std::vector<std::string>> tableValues = got->second;
		std::vector<std::string> data;

		for (int i = 0; i < value.size(); i++) {
			data = tableValues[i];

			/* Insert every element in vector */
			for (int j = 0; j < value[i].size(); j++) {
				data.push_back(value[i][j]);
			}
			tableValues[i] = data;
		}
	}

	
}

std::vector<std::vector<std::string>> PKB::getFromTable(int table_id, std::string key_id)
{
	unordered_map<std::string, std::vector<std::vector<std::string>>> table = tables[table_id];
	std::unordered_map<std::string, std::vector<std::vector<std::string>>>::const_iterator got = table.find(key_id);

	/* Row does not exist */
	if (got == table.end()) {
		std::vector<std::vector<std::string>> data;
		return data;
	}
	else {
		return got->second;
	}
}

/* Follows Operations */
std::string PKB::getFollowsBefore(std::string stmt) {
	std::vector<std::vector<std::string>> data;
	data = PKB::getFromTable(1, stmt);
	std::string parentID = data[0][0];
	data = PKB::getFromTable(2, parentID);
	
	for (int i = 0; i < data[0].size; i++) {
		if (data[0][i] == stmt) {
			return ((i - 1) < 0) ? "0" : data[0][i - 1];
			break;
		}
	}
	
	return "0";
}

std::string PKB::getFollowsAfter(std::string stmt) {
	std::vector<std::vector<std::string>> data;
	data = PKB::getFromTable(1, stmt);
	std::string parentID = data[0][0];
	data = PKB::getFromTable(2, parentID);
	
	for (int i = 0; i < data[0].size; i++) {
		if (data[0][i] == stmt) {
			return ((i + 1) == data[0].size) ? "0" : data[0][i + 1];
			break;
		}
	}
	
	return "0";
}

std::vector<std::string> PKB::getFollowsBeforeStar(std::string stmt) {
	std::vector<std::vector<std::string>> data;
	data = PKB::getFromTable(1, stmt);
	std::string parentID = data[0][0];
	data = PKB::getFromTable(2, parentID);

	for (int i = 0; i < data[0].size; i++) {
		if (data[0][i] == stmt) {
			data[0].erase(data[0].begin() + i, data[0].begin() + data[0].size);
			return data[0];
			break;
		}
	}
}

std::vector<std::string> PKB::getFollowsAfterStar(std::string stmt) {
	std::vector<std::vector<std::string>> data;
	data = PKB::getFromTable(1, stmt);
	std::string parentID = data[0][0];
	data = PKB::getFromTable(2, parentID);

	for (int i = 0; i < data[0].size; i++) {
		if (data[0][i] == stmt) {
			data[0].erase(data[0].begin(), data[0].begin() + i);
			return data[0];
			break;
		}
	}
}

bool PKB::checkFollows(std::string stmt1, std::string stmt2) {
	return (stmt2 == PKB::getFollowsAfter(stmt1));
}

bool PKB::checkFollowsStar(std::string stmt1, std::string stmt2) {
	std::vector<std::string> stmtList = PKB::getFollowsAfterStar(stmt1);
	
	for (int i = 0; i < stmtList.size; i++) {
		if (stmt2 == stmtList[i]) {
			return true;
		}
	}

	return false;
}

std::vector<std::string[2]> PKB::getAllFollows() {

	std::vector<std::string[2]> output;
	unordered_map<std::string, std::vector<std::vector<std::string>>> table = tables[2];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<std::string> stmtList = it->second[0];
		for (int i = 0; i < stmtList.size - 1; i++) {
			string newOutput[2] = {it->second[0][i], it->second[0][i+1]};
			output.push_back(newOutput);
		}
	}

	return output;
}

unordered_map<std::string, std::vector<std::string>> PKB::getAllFollowsStar() {

	unordered_map<std::string, std::vector<std::string>> output;
	unordered_map<std::string, std::vector<std::vector<std::string>>> table = tables[2];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<std::string> stmtList = it->second[0];
		std::vector<std::string> output_stmt;

		for (int i = 0; i < stmtList.size - 1; i++) {
			for (int j = i; j < stmtList.size; j++) {
				output_stmt.push_back(stmtList[i]);
			}
			output.insert({stmtList[i], output_stmt});
			output_stmt.erase;
		}

	}
}
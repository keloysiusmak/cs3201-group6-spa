#pragma once

#include <algorithm>

using namespace std;

#include "PKB.h"

/* PKB Operations */
bool PKB::insertToTable(int table_id, int key_id, std::vector<std::vector<int>> value)
{
	unordered_map<int, std::vector<std::vector<int>>> table = tables[table_id - 1];
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
				tableValuesCount = 3;
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
		
		tables[table_id - 1].insert({ key_id, tableValues });
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
		tables[table_id - 1].erase(key_id);
		tables[table_id - 1].insert({ key_id, tableValues });
	}
	return true;
}

int PKB::insertToNameTable(int table_id, string value)
{
	unordered_map<int, string> table = nameTables[table_id - 8];
	if (table_id != 8 && table_id != 9) {
		return 0;
	}
	else if (table_id == 8 && PKB::getProcedureId(value) != 0) {
		return PKB::getProcedureId(value);
	}
	else if (table_id == 9 && PKB::getVariableId(value) != 0) {
		return PKB::getVariableId(value);
	}

	/* Variable does not exist */
	int size = static_cast<int>(table.size()) + 1;
	nameTables[table_id - 8].insert({ size , value });
	
	return size;
}

std::vector<std::vector<int>> PKB::getFromTable(int table_id, int key_id)
{
	std::unordered_map<int, std::vector<std::vector<int>>>::const_iterator got = tables[table_id - 1].find(key_id);
	if (got == tables[table_id - 1].end()) {
		std::vector<std::vector<int>> data;
		return data;
	}
	return tables[table_id - 1][key_id];
}

string PKB::getFromNameTable(int table_id, int key_id)
{
	if (nameTables[table_id - 8][key_id] == "") {
		return "";
	}
	return nameTables[table_id - 8][key_id];
}

std::vector<int> PKB::getAllVariables() {

	std::vector<int> data;

	unordered_map<int, string> table = nameTables[1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		data.push_back(it->first);
	}

	return data;

}

std::vector<int> PKB::getAllStatements() {

	std::vector<int> data;

	unordered_map<int, std::vector<std::vector<int>>> table = tables[0];
	for (auto it = table.begin(); it != table.end(); ++it) {
		data.push_back(it->first);
	}

	return data;

}

std::vector<int> PKB::getAllStatementsWithType(int stmt_type) {

	std::vector<int> data;

	unordered_map<int, std::vector<std::vector<int>>> table = tables[0];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[3][0] == stmt_type) {
			data.push_back(it->first);
		}
	}

	return data;

}

std::vector<int> PKB::getAllProcedures() {

	std::vector<int> data;

	unordered_map<int, string> table = nameTables[0];
	for (auto it = table.begin(); it != table.end(); ++it) {
		data.push_back(it->first);
	}

	return data;

}

/* Accessor Operations */
std::string PKB::getProcedureName(int proc_id) {
	return PKB::getFromNameTable(8, proc_id);
}

std::string PKB::getVariableName(int var_id) {
	return PKB::getFromNameTable(9, var_id);
}

int PKB::getProcedureId(std::string proc_name) {
	unordered_map<int, string> table = nameTables[0];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second == proc_name) {
			return it->first;
		}
	}

	return 0;
}

int PKB::getVariableId(std::string var_name) {
	unordered_map<int, string> table = nameTables[1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second == var_name) {
			return it->first;
		}
	}

	return 0;
}

/* Follows Operations */
int PKB::getFollowsBefore(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	if (static_cast<int>(data.size()) > 0) {
		int stmtListID = data[0][0];
		data = PKB::getFromTable(2, stmtListID);

		if (static_cast<int>(data.size()) > 0) {

			for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
				if (data[1][i] == stmt) {
					return ((i - 1) < 0) ? 0 : data[1][i - 1];
					break;
				}
			}

		}
	}
	
	return 0;
}

int PKB::getFollowsAfter(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	if (static_cast<int>(data.size()) > 0) {
		int stmtListID = data[0][0];
		data = PKB::getFromTable(2, stmtListID);

		if (static_cast<int>(data.size()) > 0) {

			for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
				if (data[1][i] == stmt) {
					return ((i + 1) == static_cast<int>(data[1].size())) ? 0 : data[1][i + 1];
					break;
				}
			}

		}
	}
	
	return 0;
}

std::vector<int> PKB::getFollowsBeforeStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	if (static_cast<int>(data.size()) > 0) {
		int stmtListID = data[0][0];
		data = PKB::getFromTable(2, stmtListID);

		if (static_cast<int>(data.size()) > 0) {

			for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
				if (data[1][i] == stmt) {
					data[1].erase(data[1].begin() + i, data[1].begin() + static_cast<int>(data[1].size()));
					return data[1];
					break;
				}
			}

		}
	}

	std::vector<int> blank_data;
	return blank_data;
}

std::vector<int> PKB::getFollowsAfterStar(int stmt) {
	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(1, stmt);
	if (static_cast<int>(data.size()) > 0) {
		int stmtListID = data[0][0];
		data = PKB::getFromTable(2, stmtListID);

		if (static_cast<int>(data.size()) > 0) {

			for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
				if (data[1][i] == stmt) {
					data[1].erase(data[1].begin(), data[1].begin() + i + 1);
					return data[1];
					break;
				}
			}

		}
	}

	std::vector<int> blank_data;
	return blank_data;
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
	unordered_map<int, std::vector<std::vector<int>>> table = tables[1];

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

unordered_map<int, std::vector<int>> PKB::getAllFollowsStar() {

	unordered_map<int, std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[1];

	for (auto it = table.begin(); it != table.end(); ++it) {
		std::vector<int> stmtList = it->second[1];
		std::vector<int> output_stmt;

		for (unsigned int i = 0; i < static_cast<unsigned int>(stmtList.size()) - 1; i++) {
			for (unsigned int j = i + 1; j < static_cast<int>(stmtList.size()); j++) {
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
	if (static_cast<int>(data.size()) > 0) {
		data = PKB::getFromTable(2, data[0][0]);
		if (static_cast<int>(data.size()) > 0) {
			return data[0][0];
		}
	}
	return 0;
}

std::vector<int> PKB::getParentStar(int stmt) {
	std::vector<int> output;
	int parent = PKB::getParent(stmt);
	
	while (parent != 0) {
		output.push_back(parent);
		parent = PKB::getParent(parent);
	}

	return output;
}

std::vector<int> PKB::getChildren(int stmt) {
	unordered_map<int, std::vector<std::vector<int>>> table = tables[1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0][0] == stmt) {
			return it->second[1];
		}
	}
	std::vector<int> data;
	return data;
}

std::vector<int> PKB::getChildrenStar(int stmt) {
	std::vector<int> output;
	std::vector<std::vector<int>> data;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[1];
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0][0] == stmt) {
			data = it->second;
		}
	}
	if (static_cast<int>(data.size()) > 0) {
		for (unsigned int i = 0; i < static_cast<int>(data[1].size()); i++) {
			output.push_back(data[1][i]);
			if (static_cast<int>(PKB::getChildren(data[1][i]).size()) > 0) {
				std::vector<int> recur_output = PKB::getChildrenStar(data[1][i]);
				for (unsigned int j = 0; j < static_cast<int>(recur_output.size()); j++) {
					output.push_back(recur_output[j]);
				}
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
	unordered_map<int, std::vector<std::vector<int>>> table = tables[1];
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

unordered_map<int, std::vector<int>> PKB::getAllParentStar() {

	unordered_map<int, std::vector<int>> output;
	unordered_map<int, std::vector<std::vector<int>>> table = tables[1];
	
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second[0][0] != 0) {
			std::vector<int> stmtList = PKB::getChildrenStar(it->second[0][0]);
			output.insert({ it->second[0][0], stmtList });
		}
	}

	return output;
}

/* Uses Operations */
std::vector<int> PKB::getUsesVariablesFromStatement(int stmt) {

	std::vector<int> data;
	std::vector<std::vector<int>> table = PKB::getFromTable(1, stmt);

	if (static_cast<int>(table.size()) > 0) {
		data = table[1];
		if (table[3][0] == 2 || table[3][0] == 3) {
			std::vector<int> temp = PKB::getFromTable(2, table[0][0])[2];
			data.insert(data.end(), temp.begin(), temp.end());
		}
	}
	return data;

}

std::vector<int> PKB::getStatementsFromUsesVariable(int varId) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(4, varId);
	if (static_cast<int>(data.size()) > 0) {
		return data[0];
	}
	else {
		std::vector<int> null_data;
		return null_data;
	}

}

std::vector<int> PKB::getUsesVariablesFromProcedure(int proc) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(3, proc);
	if (static_cast<int>(data.size()) > 0) {
		return data[1];
	}
	else {
		std::vector<int> null_data;
		return null_data;
	}

}

std::vector<int> PKB::getProceduresFromUsesVariable(int varId) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(4, varId);
	if (static_cast<int>(data.size()) > 0) {
		return data[1];
	}
	else {
		std::vector<int> null_data;
		return null_data;
	}

}

unordered_map<int, std::vector<int>> PKB::getAllStatementUsesVariables() {

	unordered_map<int, std::vector<int>> output;
	std::vector<int> stmts = PKB::getAllStatements();
	std::vector<int> data;

	for (int i = 0; i < static_cast<int>(stmts.size()); i++) {
		data = PKB::getUsesVariablesFromStatement(stmts[i]);
		output.insert({ stmts[i], data });
	}

	return output;

}

unordered_map<int, std::vector<int>> PKB::getAllVariableUsesStatements() {

	unordered_map<int, std::vector<int>> output;
	std::vector<int> vars = PKB::getAllVariables();

	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		output.insert({ vars[i], PKB::getStatementsFromUsesVariable(vars[i]) });
	}

	return output;

}

unordered_map<int, std::vector<int>> PKB::getAllProcedureUsesVariables() {

	unordered_map<int, std::vector<int>> output;
	std::vector<int> procs = PKB::getAllProcedures();

	for (int i = 0; i < static_cast<int>(procs.size()); i++) {
		output.insert({ procs[i], PKB::getUsesVariablesFromProcedure(procs[i]) });
	}

	return output;

}

unordered_map<int, std::vector<int>> PKB::getAllVariableUsesProcedures() {

	unordered_map<int, std::vector<int>> output;
	std::vector<int> vars = PKB::getAllVariables();

	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		output.insert({ vars[i], PKB::getProceduresFromUsesVariable(vars[i]) });
	}

	return output;

}

bool PKB::checkStatementUsesVariable(int stmt, int varId) {

	std::vector<int> vars = PKB::getUsesVariablesFromStatement(stmt);
	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		if (vars[i] == varId) {
			return true;
		}
	}

	return false;

}

bool PKB::checkProcedureUsesVariable(int procId, int varId) {

	std::vector<int> vars = PKB::getUsesVariablesFromProcedure(procId);
	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		if (vars[i] == varId) {
			return true;
		}
	}

	return false;

}

/* Modifies Operations */
std::vector<int> PKB::getModifiesVariablesFromStatement(int stmt) {

	std::vector<int> data;
	std::vector<std::vector<int>> table = PKB::getFromTable(1, stmt);

	if (static_cast<int>(table.size()) > 0) {
		data = table[2];
		if (table[3][0] == 2 || table[3][0] == 3) {
			std::vector<int> temp = PKB::getFromTable(2, table[0][0])[3];
			data.insert(data.end(), temp.begin(), temp.end());
		}
	}

	return data;

}

std::vector<int> PKB::getStatementsFromModifiesVariable(int varId) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(5, varId);

	if (static_cast<int>(data.size()) > 0) {
		return data[0];
	}
	else {
		std::vector<int> null_data;
		return null_data;
	}

}

std::vector<int> PKB::getModifiesVariablesFromProcedure(int proc) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(3, proc);

	if (static_cast<int>(data.size()) > 0) {
		return data[2];
	}
	else {
		std::vector<int> null_data;
		return null_data;
	}

}

std::vector<int> PKB::getProceduresFromModifiesVariable(int varId) {

	std::vector<std::vector<int>> data;
	data = PKB::getFromTable(5, varId);
	
	if (static_cast<int>(data.size()) > 0) {
		return data[1];
	}
	else {
		std::vector<int> null_data;
		return null_data;
	}

}

unordered_map<int, std::vector<int>> PKB::getAllStatementModifiesVariables() {

	unordered_map<int, std::vector<int>> output;
	std::vector<int> stmts = PKB::getAllStatements();
	std::vector<int> data;

	for (int i = 0; i < static_cast<int>(stmts.size()); i++) {
		data = PKB::getModifiesVariablesFromStatement(stmts[i]);
		output.insert({ stmts[i], data });
	}

	return output;

}

unordered_map<int, std::vector<int>> PKB::getAllVariableModifiesStatements() {

	unordered_map<int, std::vector<int>> output;
	std::vector<int> vars = PKB::getAllVariables();

	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		output.insert({ vars[i], PKB::getStatementsFromModifiesVariable(vars[i]) });
	}

	return output;

}

unordered_map<int, std::vector<int>> PKB::getAllProcedureModifiesVariables() {

	unordered_map<int, std::vector<int>> output;
	std::vector<int> procs = PKB::getAllProcedures();

	for (int i = 0; i < static_cast<int>(procs.size()); i++) {
		output.insert({ procs[i], PKB::getModifiesVariablesFromProcedure(procs[i]) });
	}

	return output;

}

unordered_map<int, std::vector<int>> PKB::getAllVariableModifiesProcedures() {

	unordered_map<int, std::vector<int>> output;
	std::vector<int> vars = PKB::getAllVariables();

	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		output.insert({ vars[i], PKB::getProceduresFromModifiesVariable(vars[i]) });
	}

	return output;

}

bool PKB::checkStatementModifiesVariable(int stmt, int varId) {

	std::vector<int> vars = PKB::getModifiesVariablesFromStatement(stmt);
	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		if (vars[i] == varId) {
			return true;
		}
	}

	return false;

}

bool PKB::checkProcedureModifiesVariable(int procId, int varId) {

	std::vector<int> vars = PKB::getModifiesVariablesFromProcedure(procId);
	for (int i = 0; i < static_cast<int>(vars.size()); i++) {
		if (vars[i] == varId) {
			return true;
		}
	}

	return false;

}

std::vector<int> PKB::getStatementsWithPattern(PatternObject p) {

	std::vector<int> dataL;
	if (p.LHS_type == 0) {
		dataL = PKB::getStatementsFromModifiesVariable(p.LHS);
	}

	std::vector<int> dataR;
	if (p.RHS_type == 0) {
		dataR = PKB::getStatementsFromUsesVariable(p.RHS);

		if (p.LHS_type == 1) {
			return dataR;
		}
		else {
			std::vector<int> output(dataL.size() + dataR.size());
			std::vector<int>::iterator it;
			
			it = std::set_intersection(dataL.begin(), dataL.end(), dataR.begin(), dataR.end(), output.begin());
			output.resize(it - output.begin());

			return output;
		}

	}
	else {
		return dataL;
	}


}

bool PKB::checkStatementWithPattern(int stmt, PatternObject p) {

	std::vector<int> stmts = PKB::getStatementsWithPattern(p);
	for (int i = 0; i < static_cast<int>(stmts.size()); i++) {
		if (stmts[i] == stmt) {
			return true;
		}
	}

	return false;

}
#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <vector>
#include <unordered_map>

using namespace std::tr1;
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	bool addToTable(int table_id, int key_id, string value);

private:
	std::vector<unordered_map<int, std::vector<string>>> tables;
};
#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

#include "PKB.h"

bool PKB::addToTable(int table_id, int key_id, string value)
{
	return true;
}

int main()
{
	PKB pkb;
	pkb.addToTable(1, 1, "A");
};
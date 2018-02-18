#pragma once

#include <vector>

using namespace std;

class Parent
{
public:
	Parent();
	array<int, vector<int>> getParentTable();
	bool isFirstParamValid(int, int);
	bool isParent(int, vector<int>);
	vector<int> getChildren(int);
	int getParent(int);
	vector<int, int> getAllParentPairs();

private:
	array<int, vector<int>> tableOne;

};
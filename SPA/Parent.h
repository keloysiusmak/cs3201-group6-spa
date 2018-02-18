#pragma once

#include <vector>
#include <array>
#include <list>

using namespace std;

class Parent
{
public:
	Parent();
	array<int, list<int>> getParentTable();
	bool isFirstParamValid(int, int);
	bool isParent(int, list<int>);
	list<int> getChildren(int);
	int getParent(int);
	list<int, int> getAllParentPairs();

private:
	array<int, list<int>> tableOne;
	array<int, list<int>> tableOneReverse;

};
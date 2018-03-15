#pragma once
#include <list>;
#include <stack>;
#include <vector>;

class Graph {

	int V;
	list<int> *adj;

	void topologicalSortUtil(int v, bool visited[], stack<int> &Stack);

public:
	Graph(int V);
	void addEdge(int v, int w);
	std::vector<int> topologicalSort();
};

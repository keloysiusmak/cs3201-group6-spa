#pragma once
#include "ClauseResults.h"
#include "Utils.h"
#include "IntermediateTable.h"
#include <list>

// UFDS Helper
struct Node {
	Param self;
	Node *parent;
	bool hasParent;

	Node() { ; };
	Node(Param self) {
		this->self = self;
	};

	void setParent(Node &node) {
		this->parent = &node;
	}
};

class EvaluatorHelper {

public:
	static void consolidateClauses(vector<Clause> const &clauses, vector<Clause> &consolidated);
	static void consolidateClauses(vector<Pattern> const &clauses, vector<Clause> &consolidated);
	static vector<vector<Clause>> sortIntoGroups(vector<Clause> &clauses);

	static map<Param, int> ufdsParams(vector<Clause> &clauses);
	static void unionParams(Param p1, Param p2, map<Param, Node> &paramsHash);
	static Node findSet(Node &n, map<Param, Node> &paramsHash);

	static void mergeClauseTable(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void mergeWithoutOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void mergeWithOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);

	static bool clauseParamsInTable(ClauseResults &clauseResults, IntermediateTable &iTable);
	static void addClauseParamToTable(ClauseResults &clauseResults, IntermediateTable &iTable);

	static int withClauseNumSyns(Clause &clause, IntermediateTable &iTable);
};

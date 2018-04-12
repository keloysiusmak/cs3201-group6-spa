#pragma once
#include "Pattern.h"
#include "Clause.h"
#include "Utils.h"
#include "PKB.h";

#include <map>

// UFDS Helper
struct Node {
	Param self;
	Node *parent;
	bool hasParent;

	Node() { ; };
	Node(Param self) {
		this->self = self;
		hasParent = false;
	};

	void setParent(Node* node) {
		this->parent = node;
	}

	bool operator<(const Node &other) const {
		return self < other.self;
	}
};
class QueryOptimization {

public:
	static void consolidateClauses(vector<Clause> const &clauses, vector<Clause> &consolidated);
	static void consolidateClauses(vector<Pattern> const &clauses, vector<Clause> &consolidated);

	static map<int, vector<Clause>> sortIntoGroups(vector<Clause> &clauses);
	static map<int, vector<Clause>> groupClauses(vector<Clause> &clauses, map<Param, Node> &paramsHash);
	static map<Param, Node> ufdsParams(vector<Clause> &clauses);
	static void unionParams(Param &p1, Param &p2, map<Param, Node> &paramsHash);
	static Node* findSet(Node &n, map<Param, Node> &paramsHash);
	
	static vector<Clause> sortWithinGroup(vector<Clause> &clauseGroup, PKB &pkb);
	static int getTotalWeight(Clause &clause, int resultsWeight, int synsWeight, int relationWeight, PKB &pkb);
	static int getNumResultsOfClause(Clause &clause, PKB &pkb);
	static int getSynConstCase(Clause &clause);
	static int getParamIntValue(Param & p, PKB &pkb);
	static int getNumSynsOfClause(Clause &clause);
	static int getRelationWeightOfClause(Clause &clause);

	static vector<vector<Clause>> sortGroupsEvalOrder(map<int, vector<Clause>> &clauseGroups);
};

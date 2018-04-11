#include "QueryOptimization.h"

void QueryOptimization::setPKB(PKB generatedPKB) {
	PKB pkb = generatedPKB;
}

void QueryOptimization::consolidateClauses(vector<Clause> const &clauses, vector<Clause> &consolidated) {
	for (Clause clause : clauses) { consolidated.push_back(clause); } }
void QueryOptimization::consolidateClauses(vector<Pattern> const &clauses, vector<Clause> &consolidated) {
	for (Clause clause : clauses) { consolidated.push_back(clause); } }

// Preliminary sorting by synonyms 
map<int, vector<Clause>> QueryOptimization::sortIntoGroups(vector<Clause> &clauses) {
	map<Param, Node> paramsHash = ufdsParams(clauses);
	map<int, vector<Clause>> groupedClauses = groupClauses(clauses, paramsHash);
	return groupedClauses;
};

// Sort clauses into groups according to grouped syns
map<int, vector<Clause>> QueryOptimization::groupClauses(vector<Clause> &clauses, map<Param, Node> &paramsHash) {

	map<Node, int> nodeInt;
	// Consolidate groups to int
	int numGroups = 1;
	for (pair<Param, Node> paramNode : paramsHash) {
		Node* setNode = findSet(paramNode.second, paramsHash);
		if (nodeInt.find(*setNode) == nodeInt.end()) { // Only add if Node not in map
			nodeInt[*setNode] = numGroups;
			numGroups++;
		}
	}

	map<int, vector<Clause>> groupedClauses;

	// Group clauses
	for (Clause clause : clauses) {
		if (!clause.hasSynonymsNotALL()) { // Group all clauses which do not need merging together
			groupedClauses[0].push_back(clause);
			continue;
		}

		Param lhs = clause.getLeftParam();
		Param rhs = clause.getRightParam();

		if (Utils::isSynonym(lhs) && lhs.type != ALL) { // LHS
			Node* lhsSetNode = findSet(paramsHash[lhs], paramsHash);
			int groupNum = nodeInt[*lhsSetNode];
			groupedClauses[groupNum].push_back(clause);
			continue;
		}

		if (Utils::isSynonym(rhs) && rhs.type != ALL) { // RHS
			Node* rhsSetNode = findSet(paramsHash[rhs], paramsHash);
			int groupNum = nodeInt[*rhsSetNode];
			groupedClauses[groupNum].push_back(clause);
			continue;
		}

		if (clause.getRelRef() == None) { // Pattern
			Pattern* p = static_cast<Pattern*>(&clause);
			Node* entNodeSet = findSet(paramsHash[p->getEntity()], paramsHash);
			int groupNum = nodeInt[*entNodeSet];
			groupedClauses[groupNum].push_back(clause);
			continue;
		}
	}

	return groupedClauses;
}

map<Param, Node> QueryOptimization::ufdsParams(vector<Clause> &clauses) {
	// Hash all params
	map<Param, Node> paramsHash;
	for (Clause clause : clauses) {
		Param lhs = clause.getLeftParam();
		Param rhs = clause.getRightParam();
		if (Utils::isSynonym(lhs) && lhs.type != ALL) {
			Node newNode = Node(lhs);
			paramsHash[lhs] = newNode;
		}
		if (Utils::isSynonym(rhs) && rhs.type != ALL) {
			Node newNode = Node(rhs);
			paramsHash[rhs] = newNode;
		}
		if (clause.getRelRef() == None) { // Check for pattern
			Pattern* p = static_cast<Pattern*>(&clause);
			Param patternEnt = p->getEntity();
			Node newNode = Node(patternEnt);
			paramsHash[patternEnt] = newNode;
		} else { ; }
	}

	// Group params according to clauses
	for (Clause clause : clauses) {
		Param lhs = clause.getLeftParam();
		Param rhs = clause.getRightParam();

		if (clause.getRelRef() == None) { // Pattern
			Pattern* p = static_cast<Pattern*>(&clause);
			Param patternEnt = p->getEntity();
			if (Utils::isSynonym(lhs) && lhs.type != ALL) {
				unionParams(lhs, patternEnt, paramsHash);
			}
		}

		if (Utils::isSynonym(lhs) && lhs.type != ALL &&
			Utils::isSynonym(rhs) && rhs.type != ALL) {
			unionParams(lhs, rhs, paramsHash);
		}
	}

	return paramsHash;
};

// Unions two given params
void QueryOptimization::unionParams(Param &p1, Param &p2, map<Param, Node> &paramsHash) {
	Node* p1Node = &paramsHash[p1];
	Node* p2Node = &paramsHash[p2];
	
	Node* p1NodeSet = findSet(*p1Node, paramsHash);
	Node* p2NodeSet = findSet(*p2Node, paramsHash);

	Param p1Param = p1NodeSet->self;
	Param p2Param = p2NodeSet->self;

	if (!Utils::isSameParam(p1Param, p2Param)) { // Not in the same set
		p1NodeSet->setParent(p2NodeSet);
		p1NodeSet->hasParent = true;
	}
};

// Finds the Set of the given node
Node* QueryOptimization::findSet(Node &n, map<Param, Node> &paramsHash) {
	if (!n.hasParent) return &n;
	Node* parent = findSet(*n.parent, paramsHash);
	n.setParent(parent); // Recursively set parent to root node
	return n.parent;
}

/* Sorts clauses within group to optimize order of evaluation */
vector<Clause> QueryOptimization::sortWithinGroup(vector<Clause> &clauseGroup) {
	
};

/* Gets the total weight of a clause with respective input weights */
int QueryOptimization::getTotalWeight(Clause &clause, int resultsWeight, int synsWeight, int relationWeight) {
	int resultsTotal = resultsWeight * getNumResultsOfClause(clause);
	int synsTotal = synsWeight * getNumSynsOfClause(clause);
	int relationTotal = relationWeight * getRelationWeightOfClause(clause);
	return resultsTotal + synsTotal + relationTotal;
};

/* Gets number of results from evaluation of clause */
int QueryOptimization::getNumResultsOfClause(Clause &clause) {
	if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	}

	Modifies, Uses, Parent, ParentT, Follows, FollowsT,
	Next, NextT, Calls, CallsT, Affects, AffectsT, With, None
};

/* Get number of synonyms within clause */
int QueryOptimization::getNumSynsOfClause(Clause &clause) {
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();
	int numParams = 0;
	if (Utils::isSynonym(leftParam)) numParams++;
	if (Utils::isSynonym(rightParam)) numParams++;
	return numParams;
};

/* Get weight of each relation */
int QueryOptimization::getRelationWeightOfClause(Clause &clause) {
	RelRef clauseRelation = clause.getRelRef();
	switch (clauseRelation) {
	With:
		return 0;
	Follows:
		return 5;
	FollowsT:
		return 8;
	Parent:
		return 5;
	ParentT:
		return 8;
	Uses:
		return 5;
	Modifies:
		return 5;
	Calls:
		return 5;
	CallsT:
		return 8;
	None: // Pattern
		return 10;
	Next:
		return 20;
	NextT:
		return 50;
	Affects:
		return 100;
	AffectsT:
		return 500;
};

/* Sorts according to total weight of each group */
vector<vector<Clause>> sortGroupsEvalOrder(map<int, vector<Clause>> &clauseGroups) {

};

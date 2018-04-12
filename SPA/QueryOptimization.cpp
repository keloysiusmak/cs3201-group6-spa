#include "QueryOptimization.h"

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
vector<Clause> QueryOptimization::sortWithinGroup(vector<Clause> &clauseGroup, PKB &pkb) {
	
};

/* Gets the total weight of a clause with respective input weights */
int QueryOptimization::getTotalWeight(Clause &clause, int resultsWeight, int synsWeight, int relationWeight, PKB &pkb) {
	int resultsTotal = resultsWeight * getNumResultsOfClause(clause, pkb);
	int synsTotal = synsWeight * getNumSynsOfClause(clause);
	int relationTotal = relationWeight * getRelationWeightOfClause(clause);
	return resultsTotal + synsTotal + relationTotal;
};

/* Gets number of results from evaluation of clause */
int QueryOptimization::getNumResultsOfClause(Clause &clause, PKB &pkb) {

	int numResults;

	RelRef clauseRelation = clause.getRelRef();
	Param lhs = clause.getLeftParam();
	Param rhs = clause.getRightParam();

	int synConstCase = getSynConstCase(clause);
	int lhsIntValue = getParamIntValue(lhs, pkb);
	int rhsIntValue = getParamIntValue(rhs, pkb);

	if (clauseRelation == Parent) {
		if (synConstCase = 1) { // Syn, Syn
			numResults = pkb.getFromResultTable(RelationParent, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			numResults = pkb.getFromResultTable(RelationParent, 0, rhsIntValue);
		}
		if (synConstCase = 3) { // Concrete, Syn
			numResults = pkb.getFromResultTable(RelationParent, lhsIntValue, 0);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			numResults = pkb.getFromResultTable(RelationParent, lhsIntValue, rhsIntValue);
		}

	} else if (clauseRelation == ParentT) {
		if (synConstCase = 1) { // Syn, Syn
			numResults = pkb.getFromResultTable(RelationParentStar, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			numResults = pkb.getFromResultTable(RelationParent, 0, rhsIntValue);
		}
		if (synConstCase = 3) { // Concrete, Syn
			numResults = pkb.getFromResultTable(RelationParent, lhsIntValue, 0);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			numResults = pkb.getFromResultTable(RelationParent, lhsIntValue, rhsIntValue);
		}

	} else if (clauseRelation == Follows) {
		if (synConstCase = 1) { // Syn, Syn
			numResults = pkb.getFromResultTable(RelationFollows, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			numResults = pkb.getFromResultTable(RelationFollows, 0, rhsIntValue);
		}
		if (synConstCase = 3) { // Concrete, Syn
			numResults = pkb.getFromResultTable(RelationFollows, lhsIntValue, 0);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			numResults = pkb.getFromResultTable(RelationFollows, lhsIntValue, rhsIntValue);
		}

	} else if (clauseRelation == FollowsT) {
		if (synConstCase = 1) { // Syn, Syn
			numResults = pkb.getFromResultTable(RelationFollowsStar, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			numResults = pkb.getFromResultTable(RelationFollowsStar, 0, rhsIntValue);
		}
		if (synConstCase = 3) { // Concrete, Syn
			numResults = pkb.getFromResultTable(RelationFollowsStar, lhsIntValue, 0);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			numResults = pkb.getFromResultTable(RelationFollowsStar, lhsIntValue, rhsIntValue);
		}

	} else if (clauseRelation == Uses) {
		if (synConstCase = 1) { // Syn, Syn
			if (lhs.type == PROCEDURE) numResults = pkb.getFromResultTable(RelationUsesProcedure, 0, 0);
			else numResults = pkb.getFromResultTable(RelationUsesStatement, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			if (lhs.type == PROCEDURE) numResults = pkb.getFromResultTable(RelationUsesProcedure, 0, rhsIntValue);
			else numResults = pkb.getFromResultTable(RelationUsesStatement, 0, rhsIntValue);
		}
		if (synConstCase = 3) { // Concrete, Syn
			if (lhs.type == PROCEDURE) numResults = pkb.getFromResultTable(RelationUsesProcedure, lhsIntValue, 0);
			else numResults = pkb.getFromResultTable(RelationUsesStatement, lhsIntValue, 0);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			if (lhs.type == PROCEDURE)  numResults = pkb.getFromResultTable(RelationUsesProcedure, lhsIntValue, rhsIntValue);
			else numResults = pkb.getFromResultTable(RelationUsesStatement, lhsIntValue, rhsIntValue);
		}

	} else if (clauseRelation == Modifies) {
		if (synConstCase = 1) { // Syn, Syn
			if (lhs.type == PROCEDURE) numResults = pkb.getFromResultTable(RelationModifiesProcedure, 0, 0);
			else numResults = pkb.getFromResultTable(RelationModifiesStatement, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			if (lhs.type == PROCEDURE) numResults = pkb.getFromResultTable(RelationModifiesProcedure, 0, rhsIntValue);
			else numResults = pkb.getFromResultTable(RelationModifiesStatement, 0, rhsIntValue);
		}
		if (synConstCase = 3) { // Concrete, Syn
			if (lhs.type == PROCEDURE) numResults = pkb.getFromResultTable(RelationModifiesProcedure, lhsIntValue, 0);
			else numResults = pkb.getFromResultTable(RelationModifiesStatement, lhsIntValue, 0);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			if (lhs.type == PROCEDURE) numResults = pkb.getFromResultTable(RelationModifiesProcedure, lhsIntValue, rhsIntValue);
			else numResults = pkb.getFromResultTable(RelationModifiesStatement, lhsIntValue, rhsIntValue);
		}

	} else if (clauseRelation == Calls) {
		if (synConstCase = 1) { // Syn, Syn
			numResults = pkb.getFromResultTable(RelationCalls, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			numResults = pkb.getFromResultTable(RelationCalls, lhsIntValue, 0);
		}
		if (synConstCase = 3) { // Concrete, Syn
			numResults = pkb.getFromResultTable(RelationCalls, 0, rhsIntValue);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			numResults = pkb.getFromResultTable(RelationCalls, lhsIntValue, rhsIntValue);
		}

	} else if (clauseRelation == CallsT) {
		if (synConstCase = 1) { // Syn, Syn
			numResults = pkb.getFromResultTable(RelationCalls, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			numResults = pkb.getFromResultTable(RelationCalls, lhsIntValue, 0);
		}
		if (synConstCase = 3) { // Concrete, Syn
			numResults = pkb.getFromResultTable(RelationCalls, 0, rhsIntValue);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			numResults = pkb.getFromResultTable(RelationCalls, lhsIntValue, rhsIntValue);
		}
	} else if (clauseRelation == Affects) {
		if (synConstCase = 1) { // Syn, Syn
			numResults = pkb.getFromResultTable(RelationAffects, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			numResults = pkb.getFromResultTable(RelationAffects, lhsIntValue, 0);
		}
		if (synConstCase = 3) { // Concrete, Syn
			numResults = pkb.getFromResultTable(RelationAffects, 0, rhsIntValue);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			numResults = pkb.getFromResultTable(RelationAffects, lhsIntValue, rhsIntValue);
		}
	} else if (clauseRelation == AffectsT) {
		if (synConstCase = 1) { // Syn, Syn
			numResults = pkb.getFromResultTable(RelationAffectsStar, 0, 0);
		}
		if (synConstCase = 2) { // Syn, Concrete
			numResults = pkb.getFromResultTable(RelationAffectsStar, lhsIntValue, 0);
		}
		if (synConstCase = 3) { // Concrete, Syn
			numResults = pkb.getFromResultTable(RelationAffectsStar, 0, rhsIntValue);
		}
		if (synConstCase = 4) { // Concrete, Concrete
			numResults = pkb.getFromResultTable(RelationAffectsStar, lhsIntValue, rhsIntValue);
		}
	} else if (clauseRelation == With) {
		if (synConstCase = 1) { // Syn, Syn
		}
		if (synConstCase = 2) { // Syn, Concrete
		}
		if (synConstCase = 3) { // Concrete, Syn
		}
		if (synConstCase = 4) { // Concrete, Concrete
		}
	} else if (clauseRelation == None) { // Pattern
	}
};

/* Gets case of parameters
Case 1: syn, syn
Case 2: syn, concrete
Case 3: concrete, syn
Case 4: concrete, concrete
*/
int QueryOptimization::getSynConstCase(Clause &clause) {
	Param lhs = clause.getLeftParam();
	Param rhs = clause.getRightParam();
	if (Utils::isSynonym(lhs)) {
		if (Utils::isSynonym(rhs)) return 1;
		else return 2;
	} else {
		if (Utils::isSynonym(rhs)) return 3;
		else return 4;
	}
};

/* Maps param to corresponding int value or id */
int QueryOptimization::getParamIntValue(Param &p, PKB &pkb) {
	if (p.type == PROCEDURE) {
		return pkb.getProcedureId(p.value);
	} else if (p.type == VARIABLE) {
		return pkb.getVariableId(p.value);
	} else {
		return stoi(p.value);
	}
};

/* Get number of synonyms within clause */
int QueryOptimization::getNumSynsOfClause(Clause &clause) {
	Param leftParam = clause.getLeftParam();
	Param rightParam = clause.getRightParam();
	int numParams = 0;
	if (Utils::isSynonym(leftParam)) numParams++;
	if (Utils::isSynonym(rightParam)) numParams++;
	return numParams;
}

/* Get weight of each relation */
int QueryOptimization::getRelationWeightOfClause(Clause &clause) {
	RelRef clauseRelation = clause.getRelRef();
	switch (clauseRelation) {
	case With:
		return 0;
	case Follows:
		return 5;
	case FollowsT:
		return 8;
	case Parent:
		return 5;
	case ParentT:
		return 8;
	case Uses:
		return 5;
	case Modifies:
		return 5;
	case Calls:
		return 5;
	case CallsT:
		return 8;
	case None: // Pattern
		return 10;
	case Next:
		return 20;
	case NextT:
		return 50;
	case Affects:
		return 100;
	case AffectsT:
		return 500;
	}
};

/* Sorts according to total weight of each group */
vector<vector<Clause>> QueryOptimization::sortGroupsEvalOrder(map<int, vector<Clause>> &clauseGroups) {
	vector<vector<Clause>> sortedEvalGroups;
	return sortedEvalGroups;
};

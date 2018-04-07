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

/* map<int, vector<Clause>> QueryOptimization::numResultsGroupClauses(vector<Clause> groupedClauses) {

	for (Clause clause : groupedClauses) {
		RelRef relation = clause.getRelRef();
		Param leftParam = clause.getLeftParam();
		Param rightParam = clause.getRightParam();
		vector<int> size;
		vector<Clause> reGroupedClauses;
		vector<int> index;
		int i = 0;

		struct clauseValue {
			Clause originalClause;
			int size;
		};

		struct by_size {
			bool operator()(clauseValue const &a, clauseValue const &b) {
				return a.size < b.size;
			}
		};

		vector<clauseValue> clauseValues;

		if (relation == Follows) {
			if (Utils::isSynonym(leftParam)) {
				size.push_back(pkb.getFromResultTable(RelationFollows, 0, stoi(rightParam.value)));
				reGroupedClauses.push_back(clause);
				index.push_back(i);
				i++
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationFollows, stoi(leftParam.value), 0));
				reGroupedClauses.push_back(clause);
				index.push_back(i);
				i++
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationFollows, 0, 0));
				clauseValues.originalClause.push_back(clause);
				//clauseValues.size.push_back(i);
			}
		}

		if (relation == FollowsT) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationFollowsStar, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationFollowsStar, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationFollowsStar, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == Calls) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationCalls, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationCalls, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationCalls, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == CallsT) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationCallsStar, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationCallsStar, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationCallsStar, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == Uses) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationUses, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationUses, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationUses, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == Modifies) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationModifies, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationModifies, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationModifies, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == Affects) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationAffects, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationAffects, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationAffects, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == AffectsT) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationAffectsStar, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationAffectsStar, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationAffectsStar, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == Next) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationNext, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationNext, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationNext, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == NextT) {
			if (Utils::isSynonym(leftParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationNextStar, 0, stoi(rightParam.value)));
				clauseValues.originalClause.push_back(clause);
			}
			else if (Utils::isSynonym(rightParam)) {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationNextStar, stoi(leftParam.value), 0));
				clauseValues.originalClause.push_back(clause);
			}
			else {
				clauseValues.size.push_back(pkb.getFromResultTable(RelationNextStar, 0, 0));
				clauseValues.originalClause.push_back(clause);
			}
		}

		if (relation == With) {
			if (leftParam.type == PROCEDURE) {
				if (rightParam.type == CALL) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
				else if (rightParam.type == VARIABLE) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
				else if (rightParam.type == PROCEDURE) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
			}
			else if (leftParam.type == VARIABLE) {
				if (rightParam.type == CALL) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
				else if (rightParam.type == VARIABLE) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
				else if (rightParam.type == PROCEDURE) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
			}
			else if (leftParam.type == CALL) {
				if (rightParam.type == CALL) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
				else if (rightParam.type == VARIABLE) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
				else if (rightParam.type == PROCEDURE) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
					clauseValues.originalClause.push_back(clause);
				}
			}
			else if (Utils::isSynonym(leftParam)) {
				if (leftParam.type == STMT) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, 0, 0);
					clauseValues.originalClause.push_back(clause);
				}
				else if (leftParam.type == ASSIGN) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, ASSIGNMENT_TYPE, 0);
					clauseValues.originalClause.push_back(clause);
				}
				else if (leftParam.type == WHILE) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, WHILE_TYPE, 0);
					clauseValues.originalClause.push_back(clause);
				}
				else if (leftParam.type == IF) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, IF_TYPE, 0);
					clauseValues.originalClause.push_back(clause);
				}
				else if (leftParam.type == CALL) {
					clauseValues.size.push_back(pkb.getFromResultTable(RelationWithName, CALL_TYPE, 0);
					clauseValues.originalClause.push_back(clause);
				}
			}
		}
	}

	sort(clauseValues.begin(), clauseValues.end(), by_size()); 
	return clauseValues.originalClause;
} */

/* map<int, vector<Clause>> QueryOptimization::numConstantsGroupClauses(vector<Clause> groupedClauses) {

	map<int, vector<Clause>> finalGroupedClauses; 

	for (Clause clause : groupedClauses) {
		Param leftParam = clause.getLeftParam();
		Param rightParam = clause.getRightParam();

		if (Utils::isSynonym(leftParam)) { 
			if (Utils::isSynonym(rightParam)) { //2 synonyms
				finalGroupedClauses[1].push_back(clause);
				continue;
			}
			else { //only leftParam is synonym
				finalGroupedClauses[0].push_back(clause);
			}
		}
		else if (Utils::isSynonym(rightParam)) {
			if (Utils::isSynonym(leftParam)) { //2 synonyms
				finalGroupedClauses[1].push_back(clause);
				continue;
			}
			else { //only rightParam is synonym
				finalGroupedClauses[0].push_back(clause);
			}
		}
		return finalGroupedClauses;
} */

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

		if (Utils::isSynonym(lhs) && lhs.type != ALL) { // RHS
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
	return n.parent;
}

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

vector<Clause> QueryOptimization::numResultsGroupClauses(vector<Clause> groupedClauses) {

	vector<int> size;
	vector<int> newSize;
	vector<Clause> reGroupedClauses;
	vector<Clause> newReGroupedClauses;

	for (Clause clause : groupedClauses) {
		RelRef relation = clause.getRelRef();
		Param leftParam = clause.getLeftParam();
		Param rightParam = clause.getRightParam();
		//vector<int> index;
		//int i = 0;

		if (relation == Follows) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationFollows, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationFollows, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationFollows, 0, stoi(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationFollows, 0, stoi(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationFollows, stoi(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationFollows, stoi(leftParam.value), 0));
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationFollows, stoi(rightParam.value), stoi(leftParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationFollows, stoi(rightParam.value), stoi(leftParam.value)));
			}
		}

		if (relation == FollowsT) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationFollowsStar, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationFollowsStar, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationFollowsStar, 0, stoi(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationFollowsStar, 0, stoi(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationFollowsStar, stoi(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationFollowsStar, stoi(leftParam.value), 0));
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationFollowsStar, stoi(leftParam.value), stoi(rightParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationFollowsStar, stoi(leftParam.value), stoi(rightParam.value)));
			}
		}

		if (relation == Parent) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationParent, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationParent, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationParent, 0, stoi(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationParent, 0, stoi(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationParent, stoi(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationParent, stoi(leftParam.value), 0));
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationParent, stoi(rightParam.value), stoi(leftParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationParent, stoi(rightParam.value), stoi(leftParam.value)));
			}
		}

		if (relation == ParentT) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationParentStar, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationParentStar, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationParentStar, 0, stoi(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationParentStar, 0, stoi(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationParentStar, stoi(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationParentStar, stoi(leftParam.value), 0));
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationParentStar, stoi(leftParam.value), stoi(rightParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationParentStar, stoi(leftParam.value), stoi(rightParam.value)));
			}
		}

		if (relation == Calls) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationCalls, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationCalls, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationCalls, 0, pkb.getProcedureId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationCalls, 0, pkb.getProcedureId(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationCalls, pkb.getProcedureId(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationCalls, pkb.getProcedureId(leftParam.value), 0));
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationCalls, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationCalls, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
			}
		}

		if (relation == CallsT) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationCallsStar, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationCallsStar, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationCallsStar, 0, pkb.getProcedureId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationCallsStar, 0, pkb.getProcedureId(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationCallsStar, pkb.getProcedureId(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationCallsStar, pkb.getProcedureId(leftParam.value), 0));
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationCallsStar, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationCallsStar, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
			}
		}

		if (relation == Uses) {
			if (leftParam.type == PROCEDURE) {
				if (Utils::isSynonym(leftParam)) {
					if (Utils::isSynonym(rightParam)) { //both syn
						size.push_back(pkb.getFromResultTable(RelationUsesProcedure, 0, 0));
						newSize.push_back(pkb.getFromResultTable(RelationUsesProcedure, 0, 0));
					}
					else { //syn, concrete
						size.push_back(pkb.getFromResultTable(RelationUsesProcedure, 0, pkb.getVariableId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationUsesProcedure, 0, pkb.getVariableId(rightParam.value)));
					}
				}
				else if (Utils::isSynonym(rightParam)) { //concrete, syn
					size.push_back(pkb.getFromResultTable(RelationUsesProcedure, pkb.getProcedureId(leftParam.value), 0));
					newSize.push_back(pkb.getFromResultTable(RelationUsesProcedure, pkb.getProcedureId(leftParam.value), 0));
				}
				else { //concrete, concrete
					size.push_back(pkb.getFromResultTable(RelationUsesProcedure, pkb.getProcedureId(leftParam.value), pkb.getVariableId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationUsesProcedure, pkb.getProcedureId(leftParam.value), pkb.getVariableId(rightParam.value)));
				}
			}
			else if (leftParam.type == STMT) {
				if (Utils::isSynonym(leftParam)) {
					if (Utils::isSynonym(rightParam)) { //syn, syn
						size.push_back(pkb.getFromResultTable(RelationUsesStatement, 0, 0));
						newSize.push_back(pkb.getFromResultTable(RelationUsesStatement, 0, 0));
					}
					else { //syn, concrete
						size.push_back(pkb.getFromResultTable(RelationUsesStatement, 0, pkb.getVariableId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationUsesStatement, 0, pkb.getVariableId(rightParam.value)));
					}
				}
				else if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationUsesStatement, stoi(leftParam.value), 0));
					newSize.push_back(pkb.getFromResultTable(RelationUsesStatement, stoi(leftParam.value), 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationUsesStatement, stoi(leftParam.value), pkb.getVariableId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationUsesStatement, stoi(leftParam.value), pkb.getVariableId(rightParam.value)));
				}
			}
		}

		if (relation == Modifies) {
			if (leftParam.type == PROCEDURE) {
				if (Utils::isSynonym(leftParam)) {
					if (Utils::isSynonym(rightParam)) { //syn, syn
						size.push_back(pkb.getFromResultTable(RelationModifiesProcedure, 0, 0));
						newSize.push_back(pkb.getFromResultTable(RelationModifiesProcedure, 0, 0));
					}
					else { //syn, concrete
						size.push_back(pkb.getFromResultTable(RelationModifiesProcedure, 0, pkb.getVariableId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationModifiesProcedure, 0, pkb.getVariableId(rightParam.value)));
					}
				}
				else if (Utils::isSynonym(rightParam)) { //concrete, syn
					size.push_back(pkb.getFromResultTable(RelationModifiesProcedure, pkb.getProcedureId(leftParam.value), 0));
					newSize.push_back(pkb.getFromResultTable(RelationModifiesProcedure, pkb.getProcedureId(leftParam.value), 0));
				}
				else { //concrete, concrete
					size.push_back(pkb.getFromResultTable(RelationModifiesProcedure, pkb.getProcedureId(leftParam.value), pkb.getVariableId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationModifiesProcedure, pkb.getProcedureId(leftParam.value), pkb.getVariableId(rightParam.value)));
				}
			}
			else if (leftParam.type == STMT) {
				if (Utils::isSynonym(leftParam)) {
					if (Utils::isSynonym(rightParam)) { //syn, syn
						size.push_back(pkb.getFromResultTable(RelationModifiesStatement, 0, 0));
						newSize.push_back(pkb.getFromResultTable(RelationModifiesStatement, 0, 0));
					}
					else { //syn, concrete
						size.push_back(pkb.getFromResultTable(RelationModifiesStatement, 0, pkb.getVariableId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationModifiesStatement, 0, pkb.getVariableId(rightParam.value)));
					}
				}
				else if (Utils::isSynonym(rightParam)) { //concrete, syn
					size.push_back(pkb.getFromResultTable(RelationModifiesStatement, stoi(leftParam.value), 0));
					newSize.push_back(pkb.getFromResultTable(RelationModifiesStatement, stoi(leftParam.value), 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationModifiesStatement, stoi(leftParam.value), pkb.getVariableId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationModifiesStatement, stoi(leftParam.value), pkb.getVariableId(rightParam.value)));
				}
			}
		}

		if (relation == Affects) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) { //syn, syn
					size.push_back(pkb.getFromResultTable(RelationAffects, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationAffects, 0, 0));
				}
				else { //syn, concrete
					size.push_back(pkb.getFromResultTable(RelationAffects, 0, stoi(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationAffects, 0, stoi(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) { //concrete, syn
				size.push_back(pkb.getFromResultTable(RelationAffects, stoi(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationAffects, stoi(leftParam.value), 0));
			}
			else { //concrete, concrete
				size.push_back(pkb.getFromResultTable(RelationAffects, stoi(leftParam.value), stoi(rightParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationAffects, stoi(leftParam.value), stoi(rightParam.value)));
			}
		}

		if (relation == AffectsT) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationAffectsStar, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationAffectsStar, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationAffectsStar, 0, stoi(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationAffectsStar, 0, stoi(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationAffectsStar, stoi(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationAffectsStar, stoi(leftParam.value), 0));;
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationAffectsStar, stoi(leftParam.value), stoi(rightParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationAffectsStar, stoi(leftParam.value), stoi(rightParam.value)));
			}
		}

		if (relation == Next) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationNext, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationNext, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationNext, 0, stoi(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationNext, 0, stoi(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationNext, stoi(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationNext, stoi(leftParam.value), 0));
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationNext, stoi(leftParam.value), stoi(rightParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationNext, stoi(leftParam.value), stoi(rightParam.value)));
			}
		}

		if (relation == NextT) {
			if (Utils::isSynonym(leftParam)) {
				if (Utils::isSynonym(rightParam)) {
					size.push_back(pkb.getFromResultTable(RelationNextStar, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationNextStar, 0, 0));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationNextStar, 0, stoi(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationNextStar, 0, stoi(rightParam.value)));
				}
			}
			else if (Utils::isSynonym(rightParam)) {
				size.push_back(pkb.getFromResultTable(RelationNextStar, stoi(leftParam.value), 0));
				newSize.push_back(pkb.getFromResultTable(RelationNextStar, stoi(leftParam.value), 0));
			}
			else {
				size.push_back(pkb.getFromResultTable(RelationNextStar, stoi(leftParam.value), stoi(rightParam.value)));
				newSize.push_back(pkb.getFromResultTable(RelationNextStar, stoi(leftParam.value), stoi(rightParam.value)));
			}
		}

		if (relation == With) {
			if (rightParam.type == CONSTANT) {
				if (leftParam.type == STMT) {
					size.push_back(pkb.getFromResultTable(RelationWithConst, 0, 0));
					newSize.push_back(pkb.getFromResultTable(RelationWithConst, 0, 0));
				}
				else if (leftParam.type == ASSIGN) {
					size.push_back(pkb.getFromResultTable(RelationWithConst, ASSIGNMENT_TYPE, 0));
					newSize.push_back(pkb.getFromResultTable(RelationWithConst, ASSIGNMENT_TYPE, 0));
				}
				else if (leftParam.type == WHILE) {
					size.push_back(pkb.getFromResultTable(RelationWithConst, WHILE_TYPE, 0));
					newSize.push_back(pkb.getFromResultTable(RelationWithConst, WHILE_TYPE, 0));
				}
				else if (leftParam.type == IF) {
					size.push_back(pkb.getFromResultTable(RelationWithConst, IF_TYPE, 0));
					newSize.push_back(pkb.getFromResultTable(RelationWithConst, IF_TYPE, 0));
				}
				else if (leftParam.type == CALL) {
					size.push_back(pkb.getFromResultTable(RelationWithConst, CALL_TYPE, 0));
					newSize.push_back(pkb.getFromResultTable(RelationWithConst, CALL_TYPE, 0));
				}
			}
			else if (leftParam.type == PROCEDURE) {
				if (rightParam.type == CALL) {
					if (rightParam.attribute == PROCNAME) {
						size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
					}
					else { //proc, stmt
						size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), stoi(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), stoi(rightParam.value)));
					}
				}
				else if (rightParam.type == VARIABLE) {
					size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getVariableId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getVariableId(rightParam.value)));
				}
				else if (rightParam.type == PROCEDURE) {
					size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
				}
			}
			else if (leftParam.type == VARIABLE) {
				if (rightParam.type == CALL) {
					if (rightParam.attribute == PROCNAME) {
						size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getVariableId(leftParam.value), pkb.getProcedureId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getVariableId(leftParam.value), pkb.getProcedureId(rightParam.value)));
					}
					else {
						size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getVariableId(leftParam.value), stoi(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getVariableId(leftParam.value), stoi(rightParam.value)));
					}
				}
				else if (rightParam.type == VARIABLE) {
					size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getVariableId(leftParam.value), pkb.getVariableId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getVariableId(leftParam.value), pkb.getVariableId(rightParam.value)));
				}
				else if (rightParam.type == PROCEDURE) {
					size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getVariableId(leftParam.value), pkb.getProcedureId(rightParam.value)));
					newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getVariableId(leftParam.value), pkb.getProcedureId(rightParam.value)));
				}
			}
			else if (leftParam.type == CALL) {
				if (leftParam.attribute == PROCNAME) { //call proc, 
					if (rightParam.type == CALL) { //call proc, call
						if (rightParam.attribute == PROCNAME) { //call proc, call proc
							size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
							newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
						}
						else { //call proc, call stmt
							size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), stoi(rightParam.value)));
							newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), stoi(rightParam.value)));
						}
					}
					else if (rightParam.type == VARIABLE) {
						size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getVariableId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getVariableId(rightParam.value)));
					}
					else if (rightParam.type == PROCEDURE) {
						size.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationWithName, pkb.getProcedureId(leftParam.value), pkb.getProcedureId(rightParam.value)));
					}
				}
				else { //call stmt
					if (rightParam.type == CALL) { //call stmt, call
						if (rightParam.attribute == PROCNAME) { //call stmt, call proc
							size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), pkb.getProcedureId(rightParam.value)));
							newSize.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), pkb.getProcedureId(rightParam.value)));
						}
						else { //call stmt, call stmt
							size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
							newSize.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), stoi(rightParam.value)));
						}
					}
					else if (rightParam.type == VARIABLE) { //call stmt, var
						size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), pkb.getVariableId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), pkb.getVariableId(rightParam.value)));
					}
					else if (rightParam.type == PROCEDURE) { //call stmt, proc
						size.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), pkb.getProcedureId(rightParam.value)));
						newSize.push_back(pkb.getFromResultTable(RelationWithName, stoi(leftParam.value), pkb.getProcedureId(rightParam.value)));
					}
				}
			}
		}

		if (relation == NONE) {
			if (leftParam.type == ASSIGNMENT_TYPE) {
				if (rightParam.type == VARIABLE) {
					size.push_back(pkb.getFromResultTable(RelationPattern, ASSIGNMENT_TYPE, VARIABLE));
					newSize.push_back(pkb.getFromResultTable(RelationPattern, ASSIGNMENT_TYPE, VARIABLE));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationPattern, ASSIGNMENT_TYPE, 0));
					newSize.push_back(pkb.getFromResultTable(RelationPattern, ASSIGNMENT_TYPE, 0));
				}
			}
			else if (leftParam.type == WHILE_TYPE) {
				if (rightParam.type == VARIABLE) {
					size.push_back(pkb.getFromResultTable(RelationPattern, WHILE_TYPE, VARIABLE));
					newSize.push_back(pkb.getFromResultTable(RelationPattern, WHILE_TYPE, VARIABLE));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationPattern, WHILE_TYPE, 0));
					newSize.push_back(pkb.getFromResultTable(RelationPattern, WHILE_TYPE, 0));
				}
			}
			else if (leftParam.type == IF_TYPE) {
				if (rightParam.type == VARIABLE) {
					size.push_back(pkb.getFromResultTable(RelationPattern, IF_TYPE, VARIABLE));
					newSize.push_back(pkb.getFromResultTable(RelationPattern, IF_TYPE, VARIABLE));
				}
				else {
					size.push_back(pkb.getFromResultTable(RelationPattern, IF_TYPE, 0));
					newSize.push_back(pkb.getFromResultTable(RelationPattern, IF_TYPE, 0));
				}
			}
		}

		reGroupedClauses.push_back(clause);
		newReGroupedClauses.push_back(clause);
	}

	sort(size.begin(), size.end());
	for (int i = 0; i < size.size(); i++) {
		for (int j = 0; j < size.size(); j++) {
			if (size[j] == newSize[i]) {
				newReGroupedClauses[j] = reGroupedClauses[i];
				continue;
			}
		}
	}

	return newReGroupedClauses;
}

map<int, vector<Clause>> QueryOptimization::numConstantsGroupClauses(vector<Clause> groupedClauses) {

	map<int, vector<Clause>> finalGroupedClauses; 

	for (Clause clause : groupedClauses) {
		Param leftParam = clause.getLeftParam();
		Param rightParam = clause.getRightParam();

		if (Utils::isSynonym(leftParam)) {
			if (Utils::isSynonym(rightParam)) { //2 synonyms
				finalGroupedClauses[2].push_back(clause);
			}
			else { //only leftParam is synonym
				finalGroupedClauses[1].push_back(clause);
			}
		}
		else if (Utils::isSynonym(rightParam)) {
			if (Utils::isSynonym(leftParam)) { //2 synonyms
				finalGroupedClauses[2].push_back(clause);
			}
			else { //only rightParam is synonym
				finalGroupedClauses[1].push_back(clause);
			}
		}
		else {
			finalGroupedClauses[0].push_back(clause);
		}
	}
		return finalGroupedClauses;
}

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

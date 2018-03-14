#include "EvaluatorHelper.h"

using namespace std;

/* Merges the clauseResults into the intermediate table */
void EvaluatorHelper::mergeClauseTable(ClauseResults &clauseResults, IntermediateTable &iTable) {
  if (paramInTable(clauseResults, iTable)) {
    mergeWithOverlap(clauseResults, iTable);
  } else {
    mergeWithoutOverlap(clauseResults, iTable);
  }
};

/* Cross Product when there are no overlaps */
void EvaluatorHelper::mergeWithoutOverlap(ClauseResults clauseResults, IntermediateTable &iTable) {
  vector<vector<int>> newTable;
  for (vector<int> tableRow : iTable.resultsTable) {
    for (vector<int> resultsRow : clauseResults.results) {
      vector<int> newTableRow = resultsRow;
      if ()
    }
  }
};

/* With overlapping synonyms*/
void EvaluatorHelper::mergeWithOverlap(ClauseResults clauseResults, IntermediateTable &iTable) {
};

/* Returns true if param in clause result is in table */
bool paramInTable(ClauseResults clauseResults, IntermediateTable &iTable) {
  for (Param p : iTable.tableColumns) {
    if (Utils::isSameParam(p, clauseResults.entRef)) return true;
    if (Utils::isSameParam(p, clauseResults.lhs)) return true;
    if (Utils::isSameParam(p, clauseResults.rhs)) return true;
  }
  return false;
}

/* Returns the selected params from the intermediate table */
list<string> extractParams(vector<Param> selectedParams, IntermediateTable iTable) {
  if (selectedParams.size() == 1) {
    Param selected = selectedParams[0];
    if (selected.type == BOOLEAN) { // Boolean
      
    } else { // Synonym
      
    }
  } else { // Tuple

  }
  return {};
};

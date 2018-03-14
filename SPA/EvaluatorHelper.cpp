#include "EvaluatorHelper.h"

using namespace std;

/* Merges the clauseResults into the intermediate table */
void EvaluatorHelper::mergeClauseTable(ClauseResults clauseResults, IntermediateTable iTable) {
};

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

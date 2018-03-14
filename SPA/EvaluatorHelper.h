#pragma once
#include "ClauseResults.h"
#include <list>

class EvaluatorHelper {

 public:
  void mergeClauseTable(ClauseResults &clauseResults, IntermediateTable &iTable);
  list<string> extractParams(vector<Param> selectedParams, IntermediateTable &iTable);

}

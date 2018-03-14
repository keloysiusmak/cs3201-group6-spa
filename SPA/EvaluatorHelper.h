#pragma once
#include "ClauseResults.h"
#include "Utils.h"
#include <list>

class EvaluatorHelper {

 public:
  void mergeClauseTable(ClauseResults &clauseResults, IntermediateTable &iTable);
  void mergeWithoutOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);
  void mergeWithOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);
  bool paramInTable(ClauseResults &clauseResults, IntermediateTable &iTable);
  list<string> extractParams(vector<Param> selectedParams, IntermediateTable &iTable);

};
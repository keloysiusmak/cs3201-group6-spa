#pragma once
#include "ClauseResults.h"
#include "Utils.h"
#include <list>

class EvaluatorHelper {

 public:
  static void mergeClauseTable(ClauseResults &clauseResults, IntermediateTable &iTable);
  static void mergeWithoutOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);
  static void mergeWithOverlap(ClauseResults &clauseResults, IntermediateTable &iTable);

  static int getParamInt(Param p, IntermediateTable &iTable);
  static bool paramInTable(ClauseResults &clauseResults, IntermediateTable &iTable);
};
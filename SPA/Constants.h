#pragma once

enum CLAUSE_NODE_TYPE {
	CLAUSE,
	PATTERN,
	WITH_CLAUSE,
	OPERATOR
};
enum OPERATORS {
	OR,
	AND
};

enum TABLES {
	BLANK_TABLE,
	STATEMENT_TABLE,
	STATEMENT_LIST_TABLE,
	PROC_INFO_TABLE,
	USES_TABLE,
	MODIFIES_TABLE,
	CONST_TABLE,
	CALLS_TABLE,
	CALLS_STAR_TABLE,
	CALLS_INVERSE_TABLE,
	CALLS_STAR_INVERSE_TABLE,
	CALL_STATEMENT_TABLE,
	NEXT_TABLE,
	NEXT_INVERSE_TABLE,
	PROC_NAME_VAR_NAME_TABLE,
	PROC_NAME_CALL_NAME_TABLE,
	VAR_NAME_CALL_NAME_TABLE,
	STMT_NO_CONST_VALUE_TABLE,
	PATTERN_ASSIGN_VARIABLE_TABLE,
	PATTERN_WHILE_VARIABLE_TABLE,
	PATTERN_IF_VARIABLE_TABLE,
	PATTERN_TABLE,
	PROC_TABLE,
	VAR_TABLE
};

enum TYPES {
	PROCEDURE_PARENT_ID,
	ASSIGNMENT_TYPE,
	WHILE_TYPE,
	IF_TYPE,
	CALL_TYPE
};

enum ParamType {
	ASSIGN, STMT, STMTLST, WHILE, PROG_LINE, IF, CALL, //Synonym with statement number
	VARIABLE, CONSTANT, //Synonym
	PROCEDURE, //Synonym
	INTEGER,  //Statement Number
	EXPR, EXPR_EXACT, //For pattern assign (RHS)
	IDENT, //IDENT refers to "x"
	SYNONYM, //Synonym is the general term
	BOOLEAN, //return true/false
	PROC_IDENT, VAR_IDENT, //differentiate IDENT
	ALL
}; // ALL represents _

/* Attribute type to account for with statements */
enum AttrType {
	PROCNAME, VARNAME, VALUE, STMT_NO, NONE //attrName for with clause
};

enum RelRef {
	Modifies, Uses, Parent, ParentT, Follows, FollowsT,
	Next, NextT, Calls, CallsT, Affects, AffectsT, With, None
};

enum Relations {
	RelationFollows,
	RelationFollowsStar,
	RelationUsesProcedure,
	RelationUsesStatement,
	RelationModifiesProcedure,
	RelationModifiesStatement,
	RelationParent,
	RelationParentStar,
	RelationCalls,
	RelationCallsStar,
	RelationAffects,
	RelationAffectsStar,
	RelationNext,
	RelationNextStar,
	RelationWithName,
	RelationWithConst,
	RelationPattern,
	RELATIONS_SIZE
};
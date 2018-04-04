#pragma once

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
	PROC_NAME_VAR_NAME_COUNT_TABLE,
	PROC_NAME_CALL_NAME_COUNT_TABLE,
	VAR_NAME_CALL_NAME_COUNT_TABLE,
	STMT_NO_CONST_VALUE_COUNT_TABLE,
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
	RELATIONS_SIZE
};
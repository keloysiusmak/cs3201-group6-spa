#pragma once

const int STATEMENT_TABLE = 1;
const int STATEMENT_LIST_TABLE = 2;
const int PROC_INFO_TABLE = 3;
const int USES_TABLE = 4;
const int MODIFIES_TABLE = 5;
const int CONST_TABLE = 6;
const int CALLS_TABLE = 7;
const int CALLS_STAR_TABLE = 8;
const int CALLS_INVERSE_TABLE = 9;
const int CALLS_STAR_INVERSE_TABLE = 10;
const int CALL_STATEMENT_TABLE = 11;
const int NEXT_TABLE = 12;
const int NEXT_INVERSE_TABLE = 13;
const int PATTERN_TABLE = 14;
const int PROC_TABLE = 15;
const int VAR_TABLE = 16;

const int ASSIGNMENT_TYPE = 1;
const int WHILE_TYPE = 2;
const int IF_TYPE = 3;
const int CALL_TYPE = 4;
const int PROCEDURE_PARENT_ID = 0;

enum Relations {
	RelationFollows,
	RelationFollowsStar,
	RelationUses,
	RelationModifies,
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
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/RelationshipTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace RelationshipTableTest {
	TEST_CLASS(RelationshipTableTest) {

	RelationshipTable relTable;
public:
	TEST_METHOD(RelationshipTableIsValidRelationship) {
		Assert::AreEqual(true, relTable.isValidRelationship("Uses"));
		Assert::AreNotEqual(true, relTable.isValidRelationship("WrongValue"));
	}

	TEST_METHOD(RelationshipTableIsValidLeftArg) {
		Assert::AreEqual(true, relTable.isValidLeftArg("Modifies", IF));
		Assert::AreNotEqual(true, relTable.isValidLeftArg("Modifies", VARIABLE));
	}

	TEST_METHOD(RelationshipTableIsValidRightArg) {
		Assert::AreEqual(true, relTable.isValidLeftArg("NextT", PROG_LINE));
		Assert::AreNotEqual(true, relTable.isValidLeftArg("NextT", VARIABLE));
	}

	TEST_METHOD(RelationshipTableIsValidArg) {
		Assert::AreEqual(true, relTable.isValidArg("AffectsT", PROG_LINE, STMT));
		Assert::AreNotEqual(true, relTable.isValidArg("AffectsT", PROG_LINE, VARIABLE));
		Assert::AreNotEqual(true, relTable.isValidArg("AffectsT", VARIABLE, STMT));
	}

	};
}
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Preprocessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PreprocessorTesting {
	Preprocessor preprocessor;

	TEST_CLASS(PreprocessorTest) {
public:

	TEST_METHOD(PreprocessorIsValidDeclaration) {

		Preprocessor preprocessor;

		string declaration1 = "assign a";
		string declaration2 = "variable v1";
		string declaration3 = "assign a1, a2";
		string declaration4 = "stmt s1,s2";
		string declaration5 = "while w1         ,            w2";

		string invalidDeclaration1 = "assigna"; //declaration and synonym in between must have space 
		string invalidDeclaration2 = "varia v2"; // declaration must be a valid spelling
		string invalidDeclaration3 = "while 1w"; //synonym must follow the grammar rules
		string invalidDeclaration4 = "ASSIGN a"; //case-sensitive

		//Valid
		Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration1));
		Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration2));
		Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration3));
		Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration4));
		Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration5));

		//Invalid 
		//Cannot use repeated synonym
		Assert::AreNotEqual(true, preprocessor.isValidDeclaration(declaration1));
		Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration1));
		Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration2));
		Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration3));
		Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration4));
	}

	TEST_METHOD(PreprocessorIsValidQuery) {

		Preprocessor preprocessor;
		QueryEvaluator evaluatorStub;

		preprocessor.setEvaluator(evaluatorStub);

		//Populate the declarationMap
		preprocessor.insertDeclarationToMap("a", "assign");
		preprocessor.insertDeclarationToMap("w", "while");
		preprocessor.insertDeclarationToMap("v", "variable");
		preprocessor.insertDeclarationToMap("c", "constant");
		preprocessor.insertDeclarationToMap("p", "procedure");
		preprocessor.insertDeclarationToMap("c1", "call");
		preprocessor.insertDeclarationToMap("n", "prog_line");
		preprocessor.insertDeclarationToMap("ifs", "if");

		//Query without clause and pattern
		string query1 = "Select a";
		string query2 = "Select <a,w,v>";
		string query3 = "Select BOOLEAN";

		//Query for Uses and Modifies clause
		string query4 = "Select a such that Modifies(a, \"x\")";
		string query5 = "Select a such that Modifies(a, \"       x       \")";
		string query6 = "Select a such that Modifies(a, _)";
		string query7 = "Select a such that Modifies(a, v)";
		string query8 = "Select a such that Modifies(3, _)";
		string query9 = "Select a such that Modifies(3, \"       x       \")";
		string query10 = "Select a such that Modifies(3, v)";

		//Query for Follows and Parent clause
		string query11 = "Select a such that Parent*(w, 5)";
		string query12 = "Select a such that Parent*(w, a)";
		string query13 = "Select a such that Parent*(w, _)";
		string query14 = "Select a such that Parent*   (  5  , w            )";
		string query15 = "Select a such that Parent*   (5  , 7     )";
		string query16 = "Select a such that Parent*   (5  , _     )";
		string query17 = "Select a such that Parent*   (_  , 5     )";
		string query18 = "Select a such that Parent*   (_  , w     )";
		string query19 = "Select a such that Parent*   (_  , _     )";

		//Query with pattern
		string query20 = "Select a pattern a(v, _)";
		string query21 = "Select a pattern a(v, _\"y\"_)";
		string query22 = "Select a pattern a(v, _\"2\"_)";
		string query23 = "Select a pattern a(\"    x    \", _\"y\"_)";
		string query24 = "Select a pattern a(\"    x    \", _\"2	\"_)";
		string query25 = "Select a pattern a(\"    x    \", _)";
		string query26 = "Select a pattern a(   _    , _\"y\"_)";
		string query27 = "Select a pattern a(   _    , _\"2\"_)";
		string query28 = "Select a pattern a(   _    , _)";

		//Query with clause and pattern
		string query29 = "Select a such that Modifies(a, \"x\") pattern a(v, _)";
		string query30 = "Select a such that Parent*(w, _) pattern a(v, _\"y\"_)";
		string query31 = "Select a pattern a(v, _\"y\"_) such that Parent*(w, _)";
		string query32 = "Select a such that Parent*(w, _) pattern a(v, _\"y\"_) such that Modifies(a, \"x\")";

		//Query with attrRef
		string query33 = "Select a.stmt#";
		string query34 = "Select <a.stmt#, w.stmt#,v.varName>";
		string query35 = "Select <a.stmt#,w,v.varName>";

		//Query with 'with' clause
		string query36 = "Select a with c.value =	7";
		string query37 = "Select a with n = c.value";
		string query38 = "Select p.procName with p.procName = c1.procName";

		//Query with whilept
		string query39 = "Select a pattern w(v, _)";
		string query40 = "Select a pattern ifs(v,_,_)";

		//Query with and
		string query41 = "Select a such that Parent*(w, a) and Follows(a, n)";
		string query42 = "Select a pattern a(v, \"x	+     y - z\") and w(v, _)";
		string query43 = "Select v with v.varName = c1.procName and c1.procName = \"haha\"";

		//Query with tuple, BOOLEAN, such that, pattern, and, with
		string query44 = "Select <a.stmt#,n, c1.procName> such that Parent*(w, a) pattern a(v, _\"(x+y)*z\"_) and w(v,_) with c1.stmt# = 5";
		string query45 = "Select BOOLEAN with c1.stmt# = 5		 pattern a(v, _\"(x+y)*z\"_) and w(v,_) such that Parent*(w, a)";

		string invalidQuery1 = "Selecta"; //Must have space in between select and a
		string invalidQuery2 = "Select a pattern (\"x\", _\"y\"_)"; //pattern must have pattern type
		string invalidQuery3 = "Select a such Follows*(a, 2)"; //Must have "that" word
		string invalidQuery4 = "Select a such that Follows(w, a) a(\"x\", _)"; //Missing pattern
		string invalidQuery5 = "Select a such that Follows(w, \"x\")"; //Follows 2nd param must be stmtRef 
		string invalidQuery6 = "Select a such that Modifies(a, 1)"; //Modifies 2nd param must be entRef
		string invalidQuery7 = "Select a pattern a(2, _)"; // pattern 1st param must be entRef
		string invalidQuery8 = "Select a pattern a(\"x\", a)"; // pattern 2nd param must be expression-spec
		string invalidQuery9 = "Select a such  that Parent*(w, 5)"; //such that keyword can only have one white space in between
		string invalidQuery10 = "Select a such that Parent*(w, _) pattern a(v, _\"y\"_) such   that Modifies(a, \"x\")";
		//the 2nd such that keyword can only have one white space in between
		string invalidQuery11 = "Select a such that Uses()";
		string invalidQuery12 = "Select a such that Uses(a,a,a)";
		string invalidQuery13 = "Select a with";
		string invalidQuery14 = "Select a with prog_line.stmt# = 3";
		string invalidQuery15 = "Select a with c1.procName = n";
		string invalidQuery16 = "Select a such that Parent*(a, 2) and a(v, _)";

		//Valid
		Assert::AreEqual(true, preprocessor.isValidQuery(query1));
		Assert::AreEqual(true, preprocessor.isValidQuery(query2));
		Assert::AreEqual(true, preprocessor.isValidQuery(query3));
		Assert::AreEqual(true, preprocessor.isValidQuery(query4));
		Assert::AreEqual(true, preprocessor.isValidQuery(query5));
		Assert::AreEqual(true, preprocessor.isValidQuery(query6));
		Assert::AreEqual(true, preprocessor.isValidQuery(query7));
		Assert::AreEqual(true, preprocessor.isValidQuery(query8));
		Assert::AreEqual(true, preprocessor.isValidQuery(query9));
		Assert::AreEqual(true, preprocessor.isValidQuery(query10));
		Assert::AreEqual(true, preprocessor.isValidQuery(query11));
		Assert::AreEqual(true, preprocessor.isValidQuery(query12));
		Assert::AreEqual(true, preprocessor.isValidQuery(query13));
		Assert::AreEqual(true, preprocessor.isValidQuery(query14));
		Assert::AreEqual(true, preprocessor.isValidQuery(query15));
		Assert::AreEqual(true, preprocessor.isValidQuery(query16));
		Assert::AreEqual(true, preprocessor.isValidQuery(query17));
		Assert::AreEqual(true, preprocessor.isValidQuery(query18));
		Assert::AreEqual(true, preprocessor.isValidQuery(query19));
		Assert::AreEqual(true, preprocessor.isValidQuery(query20));
		Assert::AreEqual(true, preprocessor.isValidQuery(query21));
		Assert::AreEqual(true, preprocessor.isValidQuery(query22));
		Assert::AreEqual(true, preprocessor.isValidQuery(query23));
		Assert::AreEqual(true, preprocessor.isValidQuery(query24));
		Assert::AreEqual(true, preprocessor.isValidQuery(query25));
		Assert::AreEqual(true, preprocessor.isValidQuery(query26));
		Assert::AreEqual(true, preprocessor.isValidQuery(query27));
		Assert::AreEqual(true, preprocessor.isValidQuery(query28));
		Assert::AreEqual(true, preprocessor.isValidQuery(query29));
		Assert::AreEqual(true, preprocessor.isValidQuery(query30));
		Assert::AreEqual(true, preprocessor.isValidQuery(query31));
		Assert::AreEqual(true, preprocessor.isValidQuery(query32));
		Assert::AreEqual(true, preprocessor.isValidQuery(query33));
		Assert::AreEqual(true, preprocessor.isValidQuery(query34));
		Assert::AreEqual(true, preprocessor.isValidQuery(query35));
		Assert::AreEqual(true, preprocessor.isValidQuery(query36));
		Assert::AreEqual(true, preprocessor.isValidQuery(query37));
		Assert::AreEqual(true, preprocessor.isValidQuery(query38));
		Assert::AreEqual(true, preprocessor.isValidQuery(query39));
		Assert::AreEqual(true, preprocessor.isValidQuery(query40));
		Assert::AreEqual(true, preprocessor.isValidQuery(query41));
		Assert::AreEqual(true, preprocessor.isValidQuery(query42));
		Assert::AreEqual(true, preprocessor.isValidQuery(query43));
		Assert::AreEqual(true, preprocessor.isValidQuery(query44));
		Assert::AreEqual(true, preprocessor.isValidQuery(query45));

		//Invalid
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery1));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery2));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery3));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery4));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery5));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery6));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery7));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery8));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery9));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery10));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery11));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery12));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery13));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery14));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery15));
		Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery16));
	}

	TEST_METHOD(PreprocessorIsValidSynonym) {

		Assert::AreEqual(true, preprocessor.isValidSynonym("a"));
		Assert::AreEqual(true, preprocessor.isValidSynonym("ab"));
		Assert::AreEqual(true, preprocessor.isValidSynonym("a1"));
		Assert::AreEqual(true, preprocessor.isValidSynonym("a#"));

		Assert::AreNotEqual(true, preprocessor.isValidSynonym(""));
		Assert::AreNotEqual(true, preprocessor.isValidSynonym("123"));
		Assert::AreNotEqual(true, preprocessor.isValidSynonym("1a"));
		Assert::AreNotEqual(true, preprocessor.isValidSynonym("#a"));
	}

	TEST_METHOD(PreprocessorIsValidStmtRef) {

		Assert::AreEqual(true, preprocessor.isValidStmtRef("a"));
		Assert::AreEqual(true, preprocessor.isValidStmtRef("ab"));
		Assert::AreEqual(true, preprocessor.isValidStmtRef("a1"));
		Assert::AreEqual(true, preprocessor.isValidStmtRef("a#"));
		Assert::AreEqual(true, preprocessor.isValidStmtRef("_"));
		Assert::AreEqual(true, preprocessor.isValidStmtRef("1"));
		Assert::AreEqual(true, preprocessor.isValidStmtRef("123"));

		Assert::AreNotEqual(true, preprocessor.isValidStmtRef(""));
		Assert::AreNotEqual(true, preprocessor.isValidStmtRef("1a"));
		Assert::AreNotEqual(true, preprocessor.isValidStmtRef("#a"));
	}

	TEST_METHOD(PreprocessorIsValidEntRef) {

		Assert::AreEqual(true, preprocessor.isValidEntRef("a"));
		Assert::AreEqual(true, preprocessor.isValidEntRef("ab"));
		Assert::AreEqual(true, preprocessor.isValidEntRef("a1"));
		Assert::AreEqual(true, preprocessor.isValidEntRef("a#"));
		Assert::AreEqual(true, preprocessor.isValidEntRef("_"));
		Assert::AreEqual(true, preprocessor.isValidEntRef("\"x\""));
		Assert::AreEqual(true, preprocessor.isValidEntRef("\"xy\""));
		Assert::AreEqual(true, preprocessor.isValidEntRef("\"x1\""));
		Assert::AreEqual(true, preprocessor.isValidEntRef("\"x#\""));
		Assert::AreEqual(true, preprocessor.isValidEntRef("123"));

		Assert::AreNotEqual(true, preprocessor.isValidEntRef(""));
		Assert::AreNotEqual(true, preprocessor.isValidEntRef("1a"));
		Assert::AreNotEqual(true, preprocessor.isValidEntRef("#a"));
		Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"\""));
		Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"321\""));
		Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"1x\""));
		Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"#x\""));
	}

	TEST_METHOD(PreprocessorIsDeclarationSynonymExist) {

		preprocessor.insertDeclarationToMap("a", "assign");
		preprocessor.insertDeclarationToMap("s", "stmt");

		//valid
		Assert::AreEqual(true, preprocessor.isDeclarationSynonymExist("a"));

		//invalid
		Assert::AreEqual(false, preprocessor.isDeclarationSynonymExist("b"));
	}

	TEST_METHOD(PreprocessorParseClauseArg1) {

		Preprocessor preprocessor;

		//Populate the declarationMap
		preprocessor.insertDeclarationToMap("w", "while");
		preprocessor.insertDeclarationToMap("v", "variable");

		QueryObject qo;

		string relType = "Modifies";
		string arg1 = "w";
		string arg2 = "\"x\"";

		string firstParamValue;
		firstParamValue += arg1.at(0);

		string secondParamValue;
		secondParamValue += arg2.at(1);

		//Valid
		Assert::AreEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, arg2));
		Assert::AreEqual(static_cast<int>(Modifies), static_cast<int>(qo.getClauses().at(0).getRelRef()));
		Assert::AreEqual(static_cast<int>(WHILE), static_cast<int>(qo.getClauses().at(0).getFirstParam().type));
		Assert::AreEqual(firstParamValue, qo.getClauses().at(0).getFirstParam().value);
		Assert::AreEqual(static_cast<int>(IDENT), static_cast<int>(qo.getClauses().at(0).getSecondParam().type));
		Assert::AreEqual(secondParamValue, qo.getClauses().at(0).getSecondParam().value);

		//Invalid
		string invalidArg1Empty = "";
		string invalidArg1StmtRef1 = "v";
		string invalidArg1StmtRefNotExist = "a";

		string invalidArg2Empty = "";
		string invalidArg2EntRef1 = "3";
		string invalidArg2EntRef2 = "w";
		string invalidArg2EntRefNotExist = "b";

		string underScore = "_";
		string integerZero = "0";

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, invalidArg1Empty, arg2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, invalidArg1StmtRef1, arg2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, invalidArg1StmtRefNotExist, arg2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, integerZero, arg2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, underScore, arg2));

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, invalidArg2Empty));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, invalidArg2EntRef1));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, invalidArg2EntRef2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, invalidArg2EntRefNotExist));

	}

	TEST_METHOD(PreprocessorParseClauseArg2) {

		Preprocessor preprocessor;

		//Populate the declarationMap
		preprocessor.insertDeclarationToMap("s", "stmt");
		preprocessor.insertDeclarationToMap("v", "variable");

		QueryObject qo;

		string relType = "ParentT";
		string arg1 = "5";
		string arg2 = "s";

		string firstParamValue;
		firstParamValue += arg1.at(0);

		string secondParamValue;
		secondParamValue += arg2.at(0);

		//Valid
		Assert::AreEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, arg2));
		Assert::AreEqual(static_cast<int>(ParentT), static_cast<int>(qo.getClauses().at(0).getRelRef()));
		Assert::AreEqual(static_cast<int>(INTEGER), static_cast<int>(qo.getClauses().at(0).getFirstParam().type));
		Assert::AreEqual(firstParamValue, qo.getClauses().at(0).getFirstParam().value);
		Assert::AreEqual(static_cast<int>(STMT), static_cast<int>(qo.getClauses().at(0).getSecondParam().type));
		Assert::AreEqual(secondParamValue, qo.getClauses().at(0).getSecondParam().value);

		//Invalid
		string invalidArg1Empty = "";
		string invalidArg1StmtRef1 = "\"x\"";
		string invalidArg1StmtRef2 = "v";
		string invalidArg1StmtRefNotExist = "a";

		string invalidArg2Empty = "";
		string invalidArg2StmtRef1 = "\"x\"";
		string invalidArg2StmtRef2 = "v";
		string invalidArg2EntRefNotExist = "b";

		string underScore = "_";
		string integerZero = "0";
		string invalidSameStmtRef = "s";

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, invalidArg1Empty, arg2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, invalidArg1StmtRef1, arg2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, invalidArg1StmtRef2, arg2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, invalidArg1StmtRefNotExist, arg2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, integerZero, arg2));

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, invalidArg2Empty));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, invalidArg2StmtRef1));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, invalidArg2StmtRef2));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, invalidArg2EntRefNotExist));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, arg1, integerZero));

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qo, relType, invalidSameStmtRef, invalidSameStmtRef));
	}

	TEST_METHOD(PreprocessorParsePattern) {

		Preprocessor preprocessor;

		//Populate the declarationMap
		preprocessor.insertDeclarationToMap("s", "stmt");
		preprocessor.insertDeclarationToMap("v", "variable");

		QueryObject qo;

		string entity = "a";
		string arg1 = "\"x\"";
		string arg2 = "_\"3\"_";

		string firstParamValue = "x";
		string secondParamValue = "3|";

		//valid
		Assert::AreEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, arg2));
		Assert::AreEqual(static_cast<int>(ASSIGN), static_cast<int>(qo.getPatterns().at(0).getEntity().type));
		Assert::AreEqual(entity, qo.getPatterns().at(0).getEntity().value);
		Assert::AreEqual(static_cast<int>(IDENT), static_cast<int>(qo.getPatterns().at(0).getLeftParam().type));
		Assert::AreEqual(firstParamValue, qo.getPatterns().at(0).getLeftParam().value);
		Assert::AreEqual(static_cast<int>(EXPR), static_cast<int>(qo.getPatterns().at(0).getRightParam().type));
		Assert::AreEqual(secondParamValue, qo.getPatterns().at(0).getRightParam().value);

		arg1 = "v";
		arg2 = "_\"(x+y)*z\"_";

		firstParamValue = "v";
		secondParamValue = "x|y|+|z|*|";

		//valid
		Assert::AreEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, arg2));
		Assert::AreEqual(static_cast<int>(ASSIGN), static_cast<int>(qo.getPatterns().at(1).getEntity().type));
		Assert::AreEqual(entity, qo.getPatterns().at(1).getEntity().value);
		Assert::AreEqual(static_cast<int>(VARIABLE), static_cast<int>(qo.getPatterns().at(1).getLeftParam().type));
		Assert::AreEqual(firstParamValue, qo.getPatterns().at(1).getLeftParam().value);
		Assert::AreEqual(static_cast<int>(EXPR), static_cast<int>(qo.getPatterns().at(1).getRightParam().type));
		Assert::AreEqual(secondParamValue, qo.getPatterns().at(1).getRightParam().value);

		arg1 = "v";
		arg2 = "\"x	+ y - 1\"";

		firstParamValue = "v";
		secondParamValue = "x|y|+|1|-|";

		//valid
		Assert::AreEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, arg2));
		Assert::AreEqual(static_cast<int>(ASSIGN), static_cast<int>(qo.getPatterns().at(2).getEntity().type));
		Assert::AreEqual(entity, qo.getPatterns().at(2).getEntity().value);
		Assert::AreEqual(static_cast<int>(VARIABLE), static_cast<int>(qo.getPatterns().at(2).getLeftParam().type));
		Assert::AreEqual(firstParamValue, qo.getPatterns().at(2).getLeftParam().value);
		Assert::AreEqual(static_cast<int>(EXPR_EXACT), static_cast<int>(qo.getPatterns().at(2).getRightParam().type));
		Assert::AreEqual(secondParamValue, qo.getPatterns().at(2).getRightParam().value);

		arg1 = "v";
		arg2 = "_";

		firstParamValue = "v";
		secondParamValue = "_";

		//valid
		Assert::AreEqual(true, preprocessor.parsePattern(qo, WHILE, entity, arg1, arg2));
		Assert::AreEqual(static_cast<int>(WHILE), static_cast<int>(qo.getPatterns().at(3).getEntity().type));
		Assert::AreEqual(entity, qo.getPatterns().at(3).getEntity().value);
		Assert::AreEqual(static_cast<int>(VARIABLE), static_cast<int>(qo.getPatterns().at(3).getLeftParam().type));
		Assert::AreEqual(firstParamValue, qo.getPatterns().at(3).getLeftParam().value);
		Assert::AreEqual(static_cast<int>(ALL), static_cast<int>(qo.getPatterns().at(3).getRightParam().type));
		Assert::AreEqual(secondParamValue, qo.getPatterns().at(3).getRightParam().value);

		//Invalid
		string invalidArg1Empty = "";
		string invalidArg1EntRef1 = "2";
		string invalidArg1EntRef2 = "s";
		string invalidArg1StmtRefNotExist = "b";

		string invalidArg2Empty = "";
		string invalidArg2ExpressSpec = "_x_"; //without ""
		string invalidExpr1 = "\"-1+x\"";
		string invalidExpr2 = "\"(-x+y)*z\"";
		string invalidExpr3 = "\"(x+y)*\"";
		string invalidExpr4 = "\"z(x+y)\"";
		string invalidExpr5 = "\"(x+y)z\"";

		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, invalidArg1Empty, arg2));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, invalidArg1EntRef1, arg2));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, invalidArg1EntRef2, arg2));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, invalidArg1StmtRefNotExist, arg2));

		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidArg2Empty));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidArg2ExpressSpec));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidExpr1));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidExpr2));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidExpr3));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidExpr4));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidExpr5));
	}

	TEST_METHOD(PreprocessorParseWithClause) {

		Preprocessor preprocessor;

		//Populate the declarationMap
		preprocessor.insertDeclarationToMap("w", "while");
		preprocessor.insertDeclarationToMap("v", "variable");
		preprocessor.insertDeclarationToMap("s", "stmt");
		preprocessor.insertDeclarationToMap("n", "prog_line");
		preprocessor.insertDeclarationToMap("c", "call");
		preprocessor.insertDeclarationToMap("p", "procedure");

		QueryObject qo;

		string arg1 = "w.stmt#";
		string arg2 = "3";

		string firstParamValue = "w";
		string secondParamValue = "3";

		//Valid
		Assert::AreEqual(true, preprocessor.parseWithClause(qo, arg1, arg2));
		Assert::AreEqual(static_cast<int>(With), static_cast<int>(qo.getWithClauses().at(0).getRelRef()));
		Assert::AreEqual(static_cast<int>(WHILE), static_cast<int>(qo.getWithClauses().at(0).getFirstParam().type));
		Assert::AreEqual(static_cast<int>(STMT_NO), static_cast<int>(qo.getWithClauses().at(0).getFirstParam().attribute));
		Assert::AreEqual(firstParamValue, qo.getWithClauses().at(0).getFirstParam().value);
		Assert::AreEqual(static_cast<int>(INTEGER), static_cast<int>(qo.getWithClauses().at(0).getSecondParam().type));
		Assert::AreEqual(static_cast<int>(NONE), static_cast<int>(qo.getWithClauses().at(0).getSecondParam().attribute));
		Assert::AreEqual(secondParamValue, qo.getWithClauses().at(0).getSecondParam().value);

		arg1 = "n";
		arg2 = "c.stmt#";

		firstParamValue = "n";
		secondParamValue = "c";

		//Valid
		Assert::AreEqual(true, preprocessor.parseWithClause(qo, arg1, arg2));
		Assert::AreEqual(static_cast<int>(With), static_cast<int>(qo.getWithClauses().at(1).getRelRef()));
		Assert::AreEqual(static_cast<int>(PROG_LINE), static_cast<int>(qo.getWithClauses().at(1).getFirstParam().type));
		Assert::AreEqual(static_cast<int>(NONE), static_cast<int>(qo.getWithClauses().at(1).getFirstParam().attribute));
		Assert::AreEqual(firstParamValue, qo.getWithClauses().at(1).getFirstParam().value);
		Assert::AreEqual(static_cast<int>(CALL), static_cast<int>(qo.getWithClauses().at(1).getSecondParam().type));
		Assert::AreEqual(static_cast<int>(STMT_NO), static_cast<int>(qo.getWithClauses().at(1).getSecondParam().attribute));
		Assert::AreEqual(secondParamValue, qo.getWithClauses().at(1).getSecondParam().value);

		arg1 = "p.procName";
		arg2 = "\"first\"";

		firstParamValue = "p";
		secondParamValue = "first";

		//Valid
		Assert::AreEqual(true, preprocessor.parseWithClause(qo, arg1, arg2));
		Assert::AreEqual(static_cast<int>(With), static_cast<int>(qo.getWithClauses().at(2).getRelRef()));
		Assert::AreEqual(static_cast<int>(PROCEDURE), static_cast<int>(qo.getWithClauses().at(2).getFirstParam().type));
		Assert::AreEqual(static_cast<int>(PROCNAME), static_cast<int>(qo.getWithClauses().at(2).getFirstParam().attribute));
		Assert::AreEqual(firstParamValue, qo.getWithClauses().at(2).getFirstParam().value);
		Assert::AreEqual(static_cast<int>(IDENT), static_cast<int>(qo.getWithClauses().at(2).getSecondParam().type));
		Assert::AreEqual(static_cast<int>(NONE), static_cast<int>(qo.getWithClauses().at(2).getSecondParam().attribute));
		Assert::AreEqual(secondParamValue, qo.getWithClauses().at(2).getSecondParam().value);

		arg1 = "p.procName";
		arg2 = "c.procName";

		firstParamValue = "p";
		secondParamValue = "c";

		//Valid
		Assert::AreEqual(true, preprocessor.parseWithClause(qo, arg1, arg2));
		Assert::AreEqual(static_cast<int>(With), static_cast<int>(qo.getWithClauses().at(3).getRelRef()));
		Assert::AreEqual(static_cast<int>(PROCEDURE), static_cast<int>(qo.getWithClauses().at(3).getFirstParam().type));
		Assert::AreEqual(static_cast<int>(PROCNAME), static_cast<int>(qo.getWithClauses().at(3).getFirstParam().attribute));
		Assert::AreEqual(firstParamValue, qo.getWithClauses().at(3).getFirstParam().value);
		Assert::AreEqual(static_cast<int>(CALL), static_cast<int>(qo.getWithClauses().at(3).getSecondParam().type));
		Assert::AreEqual(static_cast<int>(PROCNAME), static_cast<int>(qo.getWithClauses().at(3).getSecondParam().attribute));
		Assert::AreEqual(secondParamValue, qo.getWithClauses().at(3).getSecondParam().value);
	}

	TEST_METHOD(PreprocessorIsValidSuchThatKeyword) {

		string query1 = "Select a such that Parent*(a, 5)";
		string query2 = "Select a such that Parent*(a, _) pattern a(v, _\"y\"_) such that Modifies(a, \"x\")";

		string invalidQuery1 = "Select a such   that Parent*(a, 5)";
		string invalidQuery2 = "Select a such that Parent*(a, _) pattern a(v, _\"y\"_) such   that Modifies(a, \"x\")";

		//Valid
		Assert::AreEqual(true, preprocessor.isValidSuchThatKeyword(query1));
		Assert::AreEqual(true, preprocessor.isValidSuchThatKeyword(query2));

		//Invalid
		Assert::AreNotEqual(true, preprocessor.isValidSuchThatKeyword(invalidQuery1));
		Assert::AreNotEqual(true, preprocessor.isValidSuchThatKeyword(invalidQuery2));
	}

	};
}
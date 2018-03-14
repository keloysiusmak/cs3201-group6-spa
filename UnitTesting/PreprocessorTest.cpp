#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Preprocessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

//namespace UnitTesting {
//
//	Evaluator evaluatorStub;
//	Preprocessor preprocessor;
//
//	TEST_CLASS(PreprocessorTest) {
//		public:
//
//			TEST_CLASS_INITIALIZE(setup) {
//				preprocessor.setEvaluator(evaluatorStub);
//			}
//
//			TEST_METHOD(PreprocessorIsValidDeclaration) {
//
//				Evaluator evaluatorStub;
//				Preprocessor preprocessor;
//
//				preprocessor.setEvaluator(evaluatorStub);
//
//				string declaration1 = "assign a";
//				string declaration2 = "variable v1";
//				string declaration3 = "assign a1, a2";
//				string declaration4 = "stmt s1,s2";
//				string declaration5 = "while w1         ,            w2";
//
//				string invalidDeclaration1 = "assigna"; //declaration and synonym in between must have space 
//				string invalidDeclaration2 = "varia v2"; // declaration must be a valid spelling
//				string invalidDeclaration3 = "while 1w"; //synonym must follow the grammar rules
//				string invalidDeclaration4 = "ASSIGN a"; //case-sensitive
//
//				//Valid
//				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration1));
//				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration2));
//				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration3));
//				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration4));
//				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration5));
//
//				//Invalid 
//				//Cannot use repeated synonym
//				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(declaration1));
//				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration1));
//				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration2));
//				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration3));
//				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration4));
//			}
//
//			TEST_METHOD(PreprocessorIsValidQuery) {
//
//				//Populate the declarationMap
//				preprocessor.insertDeclarationToMap("a", "assign");
//				preprocessor.insertDeclarationToMap("w", "while");
//				preprocessor.insertDeclarationToMap("v", "variable");
//
//				//Query without clause and pattern
//				string query1 = "Select a";
//
//				//Query for Uses and Modifies clause
//				string query2 = "Select a such that Modifies(a, \"x\")";
//				string query3 = "Select a such that Modifies(a, \"       x       \")";
//				string query4 = "Select a such that Modifies(a, _)";
//				string query5 = "Select a such that Modifies(a, v)";
//				string query6 = "Select a such that Modifies(3, _)";
//				string query7 = "Select a such that Modifies(3, \"       x       \")";
//				string query8 = "Select a such that Modifies(3, v)";
//
//				//Query for Follows and Parent clause
//				string query9 = "Select a such that Parent*(a, 5)";
//				string query10 = "Select a such that Parent*(a, w)";
//				string query11 = "Select a such that Parent*(a, _)";
//				string query12 = "Select a such that Parent*   (  5  , w            )";
//				string query13 = "Select a such that Parent*   (5  , 7     )";
//				string query14 = "Select a such that Parent*   (5  , _     )";
//				string query15 = "Select a such that Parent*   (_  , 5     )";
//				string query16 = "Select a such that Parent*   (_  , w     )";
//				string query17 = "Select a such that Parent*   (_  , _     )";
//
//				//Query with pattern
//				string query18 = "Select a pattern a(v, _)";
//				string query19 = "Select a pattern a(v, _\"y\"_)";
//				string query20 = "Select a pattern a(v, _\"2\"_)";
//				string query21 = "Select a pattern a(\"    x    \", _\"y\"_)";
//				string query22 = "Select a pattern a(\"    x    \", _\"2	\"_)";
//				string query23 = "Select a pattern a(\"    x    \", _)";
//				string query24 = "Select a pattern a(   _    , _\"y\"_)";
//				string query25 = "Select a pattern a(   _    , _\"2\"_)";
//				string query26 = "Select a pattern a(   _    , _)";
//
//				//Query with clause and pattern
//				string query27 = "Select a such that Modifies(a, \"x\") pattern a(v, _)";
//				string query28 = "Select a such that Parent*(a, _) pattern a(v, _\"y\"_)";
//				string query29 = "Select a pattern a(v, _\"y\"_) such that Parent*(a, _)";
//				string query30 = "Select a such that Parent*(a, _) pattern a(v, _\"y\"_) such that Modifies(a, \"x\")";
//
//
//				string invalidQuery1 = "Selecta"; //Must have space in between select and a
//				string invalidQuery2 = "Select a pattern (\"x\", _\"y\"_)"; //pattern must have pattern type
//				string invalidQuery3 = "Select a such Follows*(a, 2)"; //Must have "that" word
//				string invalidQuery4= "Select a such that Follows(w, a) a(\"x\", _)"; //Missing pattern
//				string invalidQuery5 = "Select a such that Follows(w, \"x\")"; //Follows 2nd param must be stmtRef 
//				string invalidQuery6 = "Select a such that Modifies(a, 1)"; //Modifies 2nd param must be entRef
//				string invalidQuery7 = "Select a pattern a(2, _)"; // pattern 1st param must be entRef
//				string invalidQuery8 = "Select a pattern a(\"x\", a)"; // pattern 2nd param must be expression-spec
//				string invalidQuery9 = "Select a such  that Parent*(a, 5)"; //such that keyword can only have one white space in between
//				string invalidQuery10 = "Select a such that Parent*(a, _) pattern a(v, _\"y\"_) such   that Modifies(a, \"x\")";
//				//the 2nd such that keyword can only have one white space in between
//				string invalidQuery11 = "Select a such that Uses()";
//				string invalidQuery12 = "Select a such that Uses(a,a,a)";
//
//				//Valid
//				Assert::AreEqual(true, preprocessor.isValidQuery(query1));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query2));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query3));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query4));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query5));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query6));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query7));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query8));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query9));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query10));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query11));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query12));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query13));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query14));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query15));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query16));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query17));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query18));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query19));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query20));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query21));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query22));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query23));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query24));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query25));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query26));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query27));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query28));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query29));
//				Assert::AreEqual(true, preprocessor.isValidQuery(query30));
//
//				//Invalid
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery1));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery2));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery3));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery4));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery5));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery6));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery7));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery8));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery9));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery10));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery11));
//				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery12));
//			}
//
//			TEST_METHOD(PreprocessorIsValidSynonym) {
//
//				Assert::AreEqual(true, preprocessor.isValidSynonym("a"));
//				Assert::AreEqual(true, preprocessor.isValidSynonym("ab"));
//				Assert::AreEqual(true, preprocessor.isValidSynonym("a1"));
//				Assert::AreEqual(true, preprocessor.isValidSynonym("a#"));
//
//				Assert::AreNotEqual(true, preprocessor.isValidSynonym(""));
//				Assert::AreNotEqual(true, preprocessor.isValidSynonym("123"));
//				Assert::AreNotEqual(true, preprocessor.isValidSynonym("1a"));
//				Assert::AreNotEqual(true, preprocessor.isValidSynonym("#a"));
//			}
//
//			TEST_METHOD(PreprocessorIsValidStmtRef) {
//
//				Assert::AreEqual(true, preprocessor.isValidStmtRef("a"));
//				Assert::AreEqual(true, preprocessor.isValidStmtRef("ab"));
//				Assert::AreEqual(true, preprocessor.isValidStmtRef("a1"));
//				Assert::AreEqual(true, preprocessor.isValidStmtRef("a#"));
//				Assert::AreEqual(true, preprocessor.isValidStmtRef("_"));
//				Assert::AreEqual(true, preprocessor.isValidStmtRef("1"));
//				Assert::AreEqual(true, preprocessor.isValidStmtRef("123"));
//
//				Assert::AreNotEqual(true, preprocessor.isValidStmtRef(""));
//				Assert::AreNotEqual(true, preprocessor.isValidStmtRef("1a"));
//				Assert::AreNotEqual(true, preprocessor.isValidStmtRef("#a"));
//			}
//
//			TEST_METHOD(PreprocessorIsValidEntRef) {
//
//				Assert::AreEqual(true, preprocessor.isValidEntRef("a"));
//				Assert::AreEqual(true, preprocessor.isValidEntRef("ab"));
//				Assert::AreEqual(true, preprocessor.isValidEntRef("a1"));
//				Assert::AreEqual(true, preprocessor.isValidEntRef("a#"));
//				Assert::AreEqual(true, preprocessor.isValidEntRef("_"));
//				Assert::AreEqual(true, preprocessor.isValidEntRef("\"x\""));
//				Assert::AreEqual(true, preprocessor.isValidEntRef("\"xy\""));
//				Assert::AreEqual(true, preprocessor.isValidEntRef("\"x1\""));
//				Assert::AreEqual(true, preprocessor.isValidEntRef("\"x#\""));
//
//				Assert::AreNotEqual(true, preprocessor.isValidEntRef(""));
//				Assert::AreNotEqual(true, preprocessor.isValidEntRef("123"));
//				Assert::AreNotEqual(true, preprocessor.isValidEntRef("1a"));
//				Assert::AreNotEqual(true, preprocessor.isValidEntRef("#a"));
//				Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"\""));
//				Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"321\""));
//				Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"1x\""));
//				Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"#x\""));
//			}
//
//			TEST_METHOD(PreprocessorIsValidExpressSpec) {
//
//				Assert::AreEqual(true, preprocessor.isValidExpressSpec("_"));
//				Assert::AreEqual(true, preprocessor.isValidExpressSpec("_\"x\"_"));
//				Assert::AreEqual(true, preprocessor.isValidExpressSpec("_\"x1\"_"));
//				Assert::AreEqual(true, preprocessor.isValidExpressSpec("_\"5\"_"));
//
//				Assert::AreNotEqual(true, preprocessor.isValidExpressSpec(""));
//				Assert::AreNotEqual(true, preprocessor.isValidExpressSpec("123"));
//				Assert::AreNotEqual(true, preprocessor.isValidExpressSpec("1a"));
//				Assert::AreNotEqual(true, preprocessor.isValidExpressSpec("#a"));
//			}
//
//			TEST_METHOD(PreprocessorIsDeclarationSynonymExist) {
//
//				preprocessor.insertDeclarationToMap("a", "assign");
//				preprocessor.insertDeclarationToMap("s", "stmt");
//
//				//valid
//				Assert::AreEqual(true, preprocessor.isDeclarationSynonymExist("a"));
//
//				//invalid
//				Assert::AreEqual(false, preprocessor.isDeclarationSynonymExist("b"));
//			}
//
//			TEST_METHOD(PreprocessorParseClauseArg1) {
//
//				Evaluator evaluatorStub;
//				Preprocessor preprocessor;
//
//				preprocessor.setEvaluator(evaluatorStub);
//
//				//Populate the declarationMap
//				preprocessor.insertDeclarationToMap("w", "while");
//				preprocessor.insertDeclarationToMap("v", "variable");
//
//				QueryObject qo;
//
//				string relType = "Modifies";
//				string arg1 = "w";
//				string arg2 = "\"x\"";
//
//				string firstParamValue;
//				firstParamValue += arg1.at(0);
//
//				string secondParamValue;
//				secondParamValue += arg2.at(1);
//				
//				//Valid
//				Assert::AreEqual(true, preprocessor.parseClauseArg1(qo, relType, arg1, arg2));
//				Assert::AreEqual(static_cast<int>(Modifies), static_cast<int>(qo.getClauses().at(0).getRelRef()));
//				Assert::AreEqual(static_cast<int>(WHILE), static_cast<int>(qo.getClauses().at(0).getFirstParam().type));
//				Assert::AreEqual(firstParamValue, qo.getClauses().at(0).getFirstParam().value);
//				Assert::AreEqual(static_cast<int>(IDENT), static_cast<int>(qo.getClauses().at(0).getSecondParam().type));
//				Assert::AreEqual(secondParamValue, qo.getClauses().at(0).getSecondParam().value);
//
//				//Invalid
//				string invalidArg1Empty = "";
//				string invalidArg1StmtRef1 = "\"x\"";
//				string invalidArg1StmtRef2 = "v";	
//				string invalidArg1StmtRefNotExist = "a";
//
//				string invalidArg2Empty = "";
//				string invalidArg2EntRef1 = "3";
//				string invalidArg2EntRef2 = "w";			
//				string invalidArg2EntRefNotExist = "b";
//
//				string underScore = "_";
//				string integerZero = "0";
//
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, invalidArg1Empty, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, invalidArg1StmtRef1, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, invalidArg1StmtRef2, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, invalidArg1StmtRefNotExist, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, integerZero, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, underScore, arg2));
//
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, arg1, invalidArg2Empty));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, arg1, invalidArg2EntRef1));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, arg1, invalidArg2EntRef2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg1(qo, relType, arg1, invalidArg2EntRefNotExist));
//				
//			}
//
//			TEST_METHOD(PreprocessorParseClauseArg2) {
//
//				Evaluator evaluatorStub;
//				Preprocessor preprocessor;
//
//				preprocessor.setEvaluator(evaluatorStub);
//
//				//Populate the declarationMap
//				preprocessor.insertDeclarationToMap("s", "stmt");
//				preprocessor.insertDeclarationToMap("v", "variable");
//
//				QueryObject qo;
//
//				string relType = "Parent*";
//				string arg1 = "5";
//				string arg2 = "s";
//
//				string firstParamValue;
//				firstParamValue += arg1.at(0);
//
//				string secondParamValue;
//				secondParamValue += arg2.at(0);
//
//				//Valid
//				Assert::AreEqual(true, preprocessor.parseClauseArg2(qo, relType, arg1, arg2));
//				Assert::AreEqual(static_cast<int>(ParentT), static_cast<int>(qo.getClauses().at(0).getRelRef()));
//				Assert::AreEqual(static_cast<int>(INTEGER), static_cast<int>(qo.getClauses().at(0).getFirstParam().type));
//				Assert::AreEqual(firstParamValue, qo.getClauses().at(0).getFirstParam().value);
//				Assert::AreEqual(static_cast<int>(STMT), static_cast<int>(qo.getClauses().at(0).getSecondParam().type));
//				Assert::AreEqual(secondParamValue, qo.getClauses().at(0).getSecondParam().value);
//
//				//Invalid
//				string invalidArg1Empty = "";
//				string invalidArg1StmtRef1 = "\"x\"";
//				string invalidArg1StmtRef2 = "v";
//				string invalidArg1StmtRefNotExist = "a";
//
//				string invalidArg2Empty = "";
//				string invalidArg2StmtRef1 = "\"x\"";
//				string invalidArg2StmtRef2 = "v";
//				string invalidArg2EntRefNotExist = "b";
//
//				string underScore = "_";
//				string integerZero = "0";
//				string invalidSameStmtRef = "s";
//
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, invalidArg1Empty, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, invalidArg1StmtRef1, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, invalidArg1StmtRef2, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, invalidArg1StmtRefNotExist, arg2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, integerZero, arg2));
//
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, arg1, invalidArg2Empty));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, arg1, invalidArg2StmtRef1));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, arg1, invalidArg2StmtRef2));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, arg1, invalidArg2EntRefNotExist));
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, arg1, integerZero));
//
//				Assert::AreNotEqual(true, preprocessor.parseClauseArg2(qo, relType, invalidSameStmtRef, invalidSameStmtRef));
//			}
//
//			TEST_METHOD(PreprocessorParsePattern) {
//				Evaluator evaluatorStub;
//				Preprocessor preprocessor(evaluatorStub);
//
//				//Populate the declarationMap
//				preprocessor.insertDeclarationToMap("s", "stmt");
//
//				QueryObject qo;
//
//				string entity = "a";
//				string arg1 = "\"x\"";
//				string arg2 = "_\"3\"_";
//
//				string firstParamValue;
//				firstParamValue += arg1.at(1);
//
//				string secondParamValue;
//				secondParamValue += arg2.at(2);
//
//				//valid
//				Assert::AreEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, arg2));
//				Assert::AreEqual(static_cast<int>(ASSIGN), static_cast<int>(qo.getPatterns().at(0).getEntity().type));
//				Assert::AreEqual(entity, qo.getPatterns().at(0).getEntity().value);
//				Assert::AreEqual(static_cast<int>(IDENT), static_cast<int>(qo.getPatterns().at(0).getLeftParam().type));
//				Assert::AreEqual(firstParamValue, qo.getPatterns().at(0).getLeftParam().value);
//				Assert::AreEqual(static_cast<int>(CONSTANT), static_cast<int>(qo.getPatterns().at(0).getRightParam().type));
//				Assert::AreEqual(secondParamValue, qo.getPatterns().at(0).getRightParam().value);
//
//				//Invalid
//				string invalidArg1Empty = "";
//				string invalidArg1EntRef1 = "2";
//				string invalidArg1EntRef2 = "s";
//				string invalidArg1StmtRefNotExist = "b";
//
//				string invalidArg2Empty = "";
//				string invalidArg2ExpressSpec= "_x_"; //without ""
//
//				Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, invalidArg1Empty, arg2));
//				Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, invalidArg1EntRef1, arg2));
//				Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, invalidArg1EntRef2, arg2));
//				Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, invalidArg1StmtRefNotExist, arg2));
//
//				Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidArg2Empty));
//				Assert::AreNotEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, invalidArg2ExpressSpec));
//
//			}
//
//			TEST_METHOD(PreprocessorIsValidSuchThatKeyword) {
//
//				string query1 = "Select a such that Parent*(a, 5)";
//				string query2 = "Select a such that Parent*(a, _) pattern a(v, _\"y\"_) such that Modifies(a, \"x\")";
//
//				string invalidQuery1 = "Select a such   that Parent*(a, 5)";
//				string invalidQuery2 = "Select a such that Parent*(a, _) pattern a(v, _\"y\"_) such   that Modifies(a, \"x\")";
//
//				//Valid
//				Assert::AreEqual(true, preprocessor.isValidSuchThatKeyword(query1));
//				Assert::AreEqual(true, preprocessor.isValidSuchThatKeyword(query2));
//
//				//Invalid
//				Assert::AreNotEqual(true, preprocessor.isValidSuchThatKeyword(invalidQuery1));
//				Assert::AreNotEqual(true, preprocessor.isValidSuchThatKeyword(invalidQuery2));
//			}
//
//	};
//}
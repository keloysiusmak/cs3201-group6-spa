#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/Preprocessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
	TEST_CLASS(PreprocessorTest) {
		public:

			TEST_METHOD(isValidDeclarationTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				string declaration1 = "assign a";
				string declaration2 = "variable v1";
				string declaration3 = "assign a1, a2";
				string declaration4 = "while w";
				string declaration5 = "prog_line n";

				string invalidDeclaration1 = "assigna";
				string invalidDeclaration2 = "varia v2";
				string invalidDeclaration3 = "while 1w";

				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration1));
				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration2));
				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration3));
				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration4));
				Assert::AreEqual(true, preprocessor.isValidDeclaration(declaration5));

				//Cannot use repeated synonym
				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(declaration1));
				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration1));
				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration2));
				Assert::AreNotEqual(true, preprocessor.isValidDeclaration(invalidDeclaration3));
			}

			TEST_METHOD(isValidQueryTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				//Populate the declarationMap
				preprocessor.insertDeclarationToMap("a", "assign");
				preprocessor.insertDeclarationToMap("w", "while");

				string query1 = "Select a";
				string query2 = "Select a pattern a(\"x\", _\"y\"_)";
				string query3 = "Select a pattern a(_, _\"5\"_)";
				string query4 = "Select a such that Follows*(a, 2)";
				string query5 = "Select a such that Follows(w, a) pattern a(\"x\", _)";
				string query6 = "Select a such that Uses(a, \"x\")";

				string invalidQuery1 = "Selecta";
				string invalidQuery2 = "Select a pattern (\"x\", _\"y\"_)";
				string invalidQuery3 = "Select a such Follows*(a, 2)";
				string invalidQuery4= "Select a such that Follows(w, a) a(\"x\", _)";

				Assert::AreEqual(true, preprocessor.isValidQuery(query1));
				Assert::AreEqual(true, preprocessor.isValidQuery(query2));
				Assert::AreEqual(true, preprocessor.isValidQuery(query3));
				Assert::AreEqual(true, preprocessor.isValidQuery(query4));
				Assert::AreEqual(true, preprocessor.isValidQuery(query5));
				Assert::AreEqual(true, preprocessor.isValidQuery(query6));

				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery1));
				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery2));
				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery3));
				Assert::AreNotEqual(true, preprocessor.isValidQuery(invalidQuery4));
			}

			TEST_METHOD(isValidSynonymTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				Assert::AreEqual(true, preprocessor.isValidSynonym("a"));
				Assert::AreEqual(true, preprocessor.isValidSynonym("ab"));
				Assert::AreEqual(true, preprocessor.isValidSynonym("a1"));
				Assert::AreEqual(true, preprocessor.isValidSynonym("a#"));

				Assert::AreNotEqual(true, preprocessor.isValidSynonym(""));
				Assert::AreNotEqual(true, preprocessor.isValidSynonym("123"));
				Assert::AreNotEqual(true, preprocessor.isValidSynonym("1a"));
				Assert::AreNotEqual(true, preprocessor.isValidSynonym("#a"));
			}

			TEST_METHOD(isValidStmtRefTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

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

			TEST_METHOD(isValidEntRefTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				Assert::AreEqual(true, preprocessor.isValidEntRef("a"));
				Assert::AreEqual(true, preprocessor.isValidEntRef("ab"));
				Assert::AreEqual(true, preprocessor.isValidEntRef("a1"));
				Assert::AreEqual(true, preprocessor.isValidEntRef("a#"));
				Assert::AreEqual(true, preprocessor.isValidEntRef("_"));
				Assert::AreEqual(true, preprocessor.isValidEntRef("\"x\""));
				Assert::AreEqual(true, preprocessor.isValidEntRef("\"xy\""));
				Assert::AreEqual(true, preprocessor.isValidEntRef("\"x1\""));
				Assert::AreEqual(true, preprocessor.isValidEntRef("\"x#\""));

				Assert::AreNotEqual(true, preprocessor.isValidEntRef(""));
				Assert::AreNotEqual(true, preprocessor.isValidEntRef("123"));
				Assert::AreNotEqual(true, preprocessor.isValidEntRef("1a"));
				Assert::AreNotEqual(true, preprocessor.isValidEntRef("#a"));
				Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"\""));
				Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"321\""));
				Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"1x\""));
				Assert::AreNotEqual(true, preprocessor.isValidEntRef("\"#x\""));
			}

			TEST_METHOD(isValidExpressSpecTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				Assert::AreEqual(true, preprocessor.isValidExpressSpec("_"));
				Assert::AreEqual(true, preprocessor.isValidExpressSpec("_\"x\"_"));
				Assert::AreEqual(true, preprocessor.isValidExpressSpec("_\"x1\"_"));
				Assert::AreEqual(true, preprocessor.isValidExpressSpec("_\"5\"_"));

				Assert::AreNotEqual(true, preprocessor.isValidExpressSpec(""));
				Assert::AreNotEqual(true, preprocessor.isValidExpressSpec("123"));
				Assert::AreNotEqual(true, preprocessor.isValidExpressSpec("1a"));
				Assert::AreNotEqual(true, preprocessor.isValidExpressSpec("#a"));
			}

			TEST_METHOD(isDeclarationSynonymExistTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				preprocessor.insertDeclarationToMap("a", "assign");
				preprocessor.insertDeclarationToMap("s", "stmt");

				Assert::AreEqual(true, preprocessor.isDeclarationSynonymExist("a"));
				Assert::AreEqual(false, preprocessor.isDeclarationSynonymExist("b"));
			}

			TEST_METHOD(parseClauseArg1Test) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				//Populate the declarationMap
				preprocessor.insertDeclarationToMap("w", "while");

				QueryObject qo;

				string relType = "Modifies";
				string arg1 = "w,";
				string arg2 = "\"x\")";

				string firstParamValue;
				firstParamValue += arg1.at(0);

				string secondParamValue;
				secondParamValue += arg2.at(1);
				
				Assert::AreEqual(true, preprocessor.parseClauseArg1(qo, relType, arg1, arg2));
				Assert::AreEqual(relType, qo.getClauses().at(0).getRelRef());
				Assert::AreEqual(static_cast<int>(SYNONYM), static_cast<int>(qo.getClauses().at(0).getFirstParam().type));
				Assert::AreEqual(firstParamValue, qo.getClauses().at(0).getFirstParam().value);
				Assert::AreEqual(static_cast<int>(IDENT), static_cast<int>(qo.getClauses().at(0).getSecondParam().type));
				Assert::AreEqual(secondParamValue, qo.getClauses().at(0).getSecondParam().value);
			}

			TEST_METHOD(parseClauseArg2Test) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				//Populate the declarationMap
				preprocessor.insertDeclarationToMap("v", "variable");

				QueryObject qo;

				string relType = "Parent*";
				string arg1 = "5,";
				string arg2 = "v)";

				string firstParamValue;
				firstParamValue += arg1.at(0);

				string secondParamValue;
				secondParamValue += arg2.at(0);

				Assert::AreEqual(true, preprocessor.parseClauseArg2(qo, relType, arg1, arg2));
				Assert::AreEqual(relType, qo.getClauses().at(0).getRelRef());
				Assert::AreEqual(static_cast<int>(INTEGER), static_cast<int>(qo.getClauses().at(0).getFirstParam().type));
				Assert::AreEqual(firstParamValue, qo.getClauses().at(0).getFirstParam().value);
				Assert::AreEqual(static_cast<int>(SYNONYM), static_cast<int>(qo.getClauses().at(0).getSecondParam().type));
				Assert::AreEqual(secondParamValue, qo.getClauses().at(0).getSecondParam().value);
			}

			TEST_METHOD(parsePatternTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				QueryObject qo;

				string entity = "a";
				string arg1 = "\"x\",";
				string arg2 = "_\"3\"_)";

				string firstParamValue;
				firstParamValue += arg1.at(1);

				string secondParamValue;
				secondParamValue += arg2.at(2);

				Assert::AreEqual(true, preprocessor.parsePattern(qo, ASSIGN, entity, arg1, arg2));
				Assert::AreEqual(static_cast<int>(ASSIGN), static_cast<int>(qo.getPatterns().at(0).getEntity().type));
				Assert::AreEqual(entity, qo.getPatterns().at(0).getEntity().value);
				Assert::AreEqual(static_cast<int>(IDENT), static_cast<int>(qo.getPatterns().at(0).getLeftParam().type));
				Assert::AreEqual(firstParamValue, qo.getPatterns().at(0).getLeftParam().value);
				Assert::AreEqual(static_cast<int>(CONSTANT), static_cast<int>(qo.getPatterns().at(0).getRightParam().type));
				Assert::AreEqual(secondParamValue, qo.getPatterns().at(0).getRightParam().value);
			}

			TEST_METHOD(retrieveArgTypeTest) {
				Evaluator evaluatorStub;
				Preprocessor preprocessor(evaluatorStub);

				string refInteger = "5";
				string refConstValue = "_\"2\"_";
				string refVarName = "_\"x\"_";		
				string refIdent = "\"y\"";
				string refSynonym = "a";
				string refAll = "_";

				Assert::AreEqual(1, preprocessor.retrieveArgType(refInteger));
				Assert::AreEqual(2, preprocessor.retrieveArgType(refConstValue));
				Assert::AreEqual(3, preprocessor.retrieveArgType(refVarName));
				Assert::AreEqual(4, preprocessor.retrieveArgType(refIdent));
				Assert::AreEqual(5, preprocessor.retrieveArgType(refSynonym));
				Assert::AreEqual(6, preprocessor.retrieveArgType(refAll));

			}

	};
}
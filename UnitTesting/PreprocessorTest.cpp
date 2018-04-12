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

		//Populate the declarationMap
		preprocessor.insertDeclarationToMap("a", "assign");
		preprocessor.insertDeclarationToMap("s", "stmt");
		preprocessor.insertDeclarationToMap("s1", "stmt");
		preprocessor.insertDeclarationToMap("s2", "stmt");
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
		string query39 = "Select a with 3=3";
		string query40 = "Select a with \"x\"=\"y\"";
		string query41 = "Select a with 3=a.stmt#";

		//Query with whilept
		string query42 = "Select a pattern w(v, _)";
		string query43 = "Select a pattern ifs(v,_,_)";

		//Query with and
		string query44 = "Select a such that Parent*(w, a) and Follows(a, n)";
		string query45 = "Select a pattern a(v, \"x	+     y - z\") and w(v, _)";
		string query46 = "Select v with v.varName = c1.procName and c1.procName = \"haha\"";

		//Query with tuple, BOOLEAN, such that, pattern, and, with
		string query47 = "Select <a.stmt#,n, c1.procName> such that Parent*(w, a) pattern a(v, _\"(x+y)*z\"_) and w(v,_) with c1.stmt# = 5";
		string query48 = "Select BOOLEAN with c1.stmt# = 5		 pattern a(v, _\"(x+y)*z\"_) and w(v,_) such that Parent*(w, a)";
		string query49 = "Select BOOLEAN with 3=3";
		string query50 = "Select BOOLEAN with c5.procName=\"haha\"";
		string query51 = "Select BOOLEAN";
		
		//Extension for AND, OR, NOT
		string query52 = "Select BOOLEAN such that (Parent*(w, a) and Follows(a, n))";
		string query53 = "Select a such that Parent*(w, a) such that Follows(a, n)";

		//Query with or
		string query54 = "Select a such that Parent*(w, a) or Follows(a, n)";
		string query55 = "Select a pattern a(v, \"x	+     y - z\") or w(v, _)";
		string query56 = "Select v with v.varName = c1.procName or c1.procName = \"haha\"";

		//Query with and and or
		string query57 = "Select a such that Affects*(5,5) or Next*(8,1) and Calls(p, \"first\") or Follows(s1,s2)";

		//Query with parenthesis
		string query58 = "Select ifs such that (Parent(5,6) or Parent(5,7)) and (Parent(3,5) or Parent(5,6))";

		string query59 = "Select a with (v.varName = c1.procName or c1.procName = \"haha\") with (p.procName = c1.procName or c1.stmt# = 5)";
		string query60 = "Select BOOLEAN such that Affects*(5,5) or Next*(8,1) such that (Calls(p, \"first\") or Follows(s1,s2))";
		string query61 = "Select a such that ((Affects*(5,5) or Next*(8,1)) or Follows(s1,s2)) and Calls(p, \"first\")";
		
		//Query with not
		string query62 = "Select a such that not Parent(5,6)";
		string query63 = "Select a such that not (Parent(5,6) or Affects*(5,5))";
		string query64 = "Select a such that not ((Affects*(5,5) and Next*(8,1)) or (Calls(p, \"first\") and Follows(s1,s2)))";
		string query65 = "Select a such that not (Affects*(5,5) or Next*(8,1) and Parent(5,6))";
		string query66 = "Select a such that not (((Affects*(5,5) and Next*(8,1)) or Parent(5,6)) or Calls(p, \"first\"))";
		string query67 = "Select a such that not (( not (Affects*(5,5) and Next*(8,1)) or Parent(5,6)) or Calls(p, \"first\"))";
		
		//Query with pattern and or not
		string query68 = "Select a pattern (a(v, \"x	+     y - z\") or w(v, _)) and not (a(v, \"x	+     y - z\") and w(v, _))";

		//Query with SubQuery
		string query69 = "Select a such that Uses((Select s such that Parent(5,6)), \"x\")";
		string query70 = "Select a such that Uses(3, (Select v such that Parent(5,6)))";
		string query71 = "Select a such that Uses((Select s such that Parent(5,6)), (Select v such that Parent(5,6)))";
		string query72 = "Select a pattern a((Select v such that Parent(5,6)), \"x	+     y - z\")";
		string query73 = "Select a pattern a(_, (Select v such that Parent(5,6)))";
		string query74 = "Select a pattern a((Select v such that Parent(5,6)), (Select v such that Parent(5,6)))";
		string query75 = "Select a with (Select v such that Parent(5,6)) = \"x\" ";
		string query76 = "Select a with 3 = (Select c.value such that Parent(5,6)) ";
		string query77 = "Select a with (Select v.varName such that Affects*(5,5)) = (Select c1.procName such that Parent(5,6)) ";
		string query78 = "Select s such that Follows(s, (Select a such that Uses(a, (Select v such that Modifies((Select s with s.stmt# = c.value), v)))))";

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
		Assert::AreEqual(true, preprocessor.isValidQuery(query46));
		Assert::AreEqual(true, preprocessor.isValidQuery(query47));
		Assert::AreEqual(true, preprocessor.isValidQuery(query48));
		Assert::AreEqual(true, preprocessor.isValidQuery(query49));
		Assert::AreEqual(false, preprocessor.isValidQuery(query50));
		Assert::AreEqual(true, preprocessor.isValidQuery(query51));
		Assert::AreEqual(true, preprocessor.isValidQuery(query52));
		Assert::AreEqual(true, preprocessor.isValidQuery(query53));
		Assert::AreEqual(true, preprocessor.isValidQuery(query54));
		Assert::AreEqual(true, preprocessor.isValidQuery(query55));
		Assert::AreEqual(true, preprocessor.isValidQuery(query56));
		Assert::AreEqual(true, preprocessor.isValidQuery(query57));
		Assert::AreEqual(true, preprocessor.isValidQuery(query58));
		Assert::AreEqual(true, preprocessor.isValidQuery(query59));
		Assert::AreEqual(true, preprocessor.isValidQuery(query60));
		Assert::AreEqual(true, preprocessor.isValidQuery(query61));
		Assert::AreEqual(true, preprocessor.isValidQuery(query62));
		Assert::AreEqual(true, preprocessor.isValidQuery(query63));
		Assert::AreEqual(true, preprocessor.isValidQuery(query64));
		Assert::AreEqual(true, preprocessor.isValidQuery(query65));
		Assert::AreEqual(true, preprocessor.isValidQuery(query66));
		Assert::AreEqual(true, preprocessor.isValidQuery(query67));
		Assert::AreEqual(true, preprocessor.isValidQuery(query68));
		Assert::AreEqual(true, preprocessor.isValidQuery(query69));
		Assert::AreEqual(true, preprocessor.isValidQuery(query70));
		Assert::AreEqual(true, preprocessor.isValidQuery(query71));
		Assert::AreEqual(true, preprocessor.isValidQuery(query72));
		Assert::AreEqual(true, preprocessor.isValidQuery(query73));
		Assert::AreEqual(true, preprocessor.isValidQuery(query74));
		Assert::AreEqual(true, preprocessor.isValidQuery(query75));
		Assert::AreEqual(true, preprocessor.isValidQuery(query76));
		Assert::AreEqual(true, preprocessor.isValidQuery(query77));
		Assert::AreEqual(true, preprocessor.isValidQuery(query78));

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

	TEST_METHOD(PreprocessorIsValidIdent) {

		Assert::AreEqual(true, preprocessor.isValidIdent("\"a\""));
		Assert::AreEqual(true, preprocessor.isValidIdent("\"ab\""));
		Assert::AreEqual(true, preprocessor.isValidIdent("\"a1\""));
		Assert::AreEqual(true, preprocessor.isValidIdent("\"a#\""));

		Assert::AreNotEqual(true, preprocessor.isValidIdent(""));
		Assert::AreNotEqual(true, preprocessor.isValidIdent("1"));
		Assert::AreNotEqual(true, preprocessor.isValidIdent("1a"));
		Assert::AreNotEqual(true, preprocessor.isValidIdent("#a"));
		Assert::AreNotEqual(true, preprocessor.isValidIdent("\"1\""));
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

	TEST_METHOD(PreprocssorIsValidAttrRef) {
		Assert::AreEqual(true, preprocessor.isValidAttrRef("s.stmt#"));
		Assert::AreEqual(true, preprocessor.isValidAttrRef("v.varName"));
		Assert::AreEqual(true, preprocessor.isValidAttrRef("p.procName"));
		Assert::AreEqual(true, preprocessor.isValidAttrRef("c.value"));

		Assert::AreNotEqual(true, preprocessor.isValidAttrRef(""));
		Assert::AreNotEqual(true, preprocessor.isValidAttrRef("1a"));
		Assert::AreNotEqual(true, preprocessor.isValidAttrRef("#a"));
		Assert::AreNotEqual(true, preprocessor.isValidAttrRef("\"\""));
		Assert::AreNotEqual(true, preprocessor.isValidAttrRef("\"321\""));
		Assert::AreNotEqual(true, preprocessor.isValidAttrRef("\"1x\""));
		Assert::AreNotEqual(true, preprocessor.isValidAttrRef("\"#x\""));
		Assert::AreNotEqual(true, preprocessor.isValidAttrRef("c.consta"));
		Assert::AreNotEqual(true, preprocessor.isValidAttrRef("s.stmt"));
	}

	TEST_METHOD(PreprocssorIsValidAttrName) {
		Assert::AreEqual(true, preprocessor.isValidAttrName(ASSIGN, "stmt#"));
		Assert::AreEqual(true, preprocessor.isValidAttrName(CALL, "stmt#"));
		Assert::AreEqual(true, preprocessor.isValidAttrName(CALL, "procName"));
		Assert::AreEqual(true, preprocessor.isValidAttrName(CONSTANT, "value"));
		Assert::AreEqual(true, preprocessor.isValidAttrName(VARIABLE, "varName"));

		Assert::AreNotEqual(true, preprocessor.isValidAttrName(PROG_LINE, "stmt#"));
		Assert::AreNotEqual(true, preprocessor.isValidAttrName(PROCEDURE, "value"));
		Assert::AreNotEqual(true, preprocessor.isValidAttrName(CALL, "varName"));
		Assert::AreNotEqual(true, preprocessor.isValidAttrName(ASSIGN, "procName"));
	}

	TEST_METHOD(PreprocessorIsValidRef) {

		Assert::AreEqual(true, preprocessor.isValidRef("a"));
		Assert::AreEqual(true, preprocessor.isValidRef("ab"));
		Assert::AreEqual(true, preprocessor.isValidRef("a1"));
		Assert::AreEqual(true, preprocessor.isValidRef("a#"));
		Assert::AreEqual(true, preprocessor.isValidRef("1"));
		Assert::AreEqual(true, preprocessor.isValidRef("123"));
		Assert::AreEqual(true, preprocessor.isValidRef("\"x\""));
		Assert::AreEqual(true, preprocessor.isValidRef("\"xy\""));
		Assert::AreEqual(true, preprocessor.isValidRef("\"x1\""));
		Assert::AreEqual(true, preprocessor.isValidRef("\"x#\""));
		Assert::AreEqual(true, preprocessor.isValidRef("s.stmt#"));
		Assert::AreEqual(true, preprocessor.isValidRef("v.varName"));
		Assert::AreEqual(true, preprocessor.isValidRef("p.procName"));
		Assert::AreEqual(true, preprocessor.isValidRef("c.value"));

		Assert::AreNotEqual(true, preprocessor.isValidRef(""));
		Assert::AreNotEqual(true, preprocessor.isValidRef("1a"));
		Assert::AreNotEqual(true, preprocessor.isValidRef("#a"));
		Assert::AreNotEqual(true, preprocessor.isValidRef("\"\""));
		Assert::AreNotEqual(true, preprocessor.isValidRef("\"321\""));
		Assert::AreNotEqual(true, preprocessor.isValidRef("\"1x\""));
		Assert::AreNotEqual(true, preprocessor.isValidRef("\"#x\""));
		Assert::AreNotEqual(true, preprocessor.isValidRef("c.consta"));
		Assert::AreNotEqual(true, preprocessor.isValidRef("s.stmt"));
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

		QueryContent qc;

		string relType = "Modifies";
		string arg1 = "w";
		string arg2 = "\"x\"";

		string firstParamValue;
		firstParamValue += arg1.at(0);

		string secondParamValue;
		secondParamValue += arg2.at(1);

		//Valid
		Assert::AreEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(Modifies), static_cast<int>(qc.getClauses().at(0).getClause().getRelRef()));
		Assert::AreEqual(static_cast<int>(WHILE), static_cast<int>(qc.getClauses().at(0).getClause().getLeftParam().type));
		Assert::AreEqual(firstParamValue, qc.getClauses().at(0).getClause().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(VAR_IDENT), static_cast<int>(qc.getClauses().at(0).getClause().getRightParam().type));
		Assert::AreEqual(secondParamValue, qc.getClauses().at(0).getClause().getRightParam().value);

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

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, invalidArg1Empty, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, invalidArg1StmtRef1, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, invalidArg1StmtRefNotExist, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, integerZero, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, underScore, arg2, false));

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, invalidArg2Empty, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, invalidArg2EntRef1, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, invalidArg2EntRef2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, invalidArg2EntRefNotExist, false));

	}

	TEST_METHOD(PreprocessorParseClauseArg2) {

		Preprocessor preprocessor;

		//Populate the declarationMap
		preprocessor.insertDeclarationToMap("s", "stmt");
		preprocessor.insertDeclarationToMap("v", "variable");

		QueryContent qc;

		string relType = "ParentT";
		string arg1 = "5";
		string arg2 = "s";

		string firstParamValue;
		firstParamValue += arg1.at(0);

		string secondParamValue;
		secondParamValue += arg2.at(0);

		//Valid
		Assert::AreEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(ParentT), static_cast<int>(qc.getClauses().at(0).getClause().getRelRef()));
		Assert::AreEqual(static_cast<int>(INTEGER), static_cast<int>(qc.getClauses().at(0).getClause().getLeftParam().type));
		Assert::AreEqual(firstParamValue, qc.getClauses().at(0).getClause().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(STMT), static_cast<int>(qc.getClauses().at(0).getClause().getRightParam().type));
		Assert::AreEqual(secondParamValue, qc.getClauses().at(0).getClause().getRightParam().value);

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

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, invalidArg1Empty, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, invalidArg1StmtRef1, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, invalidArg1StmtRef2, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, invalidArg1StmtRefNotExist, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, integerZero, arg2, false));

		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, invalidArg2Empty, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, invalidArg2StmtRef1, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, invalidArg2StmtRef2, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, invalidArg2EntRefNotExist, false));
		Assert::AreNotEqual(true, preprocessor.parseClauseArg(qc, relType, arg1, integerZero, false));
	}

	TEST_METHOD(PreprocessorParsePattern) {

		Preprocessor preprocessor;

		//Populate the declarationMap
		preprocessor.insertDeclarationToMap("s", "stmt");
		preprocessor.insertDeclarationToMap("v", "variable");

		QueryContent qc;

		string entity = "a";
		string arg1 = "\"x\"";
		string arg2 = "_\"3\"_";

		string firstParamValue = "x";
		string secondParamValue = "3|";

		//valid
		Assert::AreEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(ASSIGN), static_cast<int>(qc.getPattern().at(0).getPattern().getEntity().type));
		Assert::AreEqual(entity, qc.getPattern().at(0).getPattern().getEntity().value);
		Assert::AreEqual(static_cast<int>(VAR_IDENT), static_cast<int>(qc.getPattern().at(0).getPattern().getLeftParam().type));
		Assert::AreEqual(firstParamValue, qc.getPattern().at(0).getPattern().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(EXPR), static_cast<int>(qc.getPattern().at(0).getPattern().getRightParam().type));
		Assert::AreEqual(secondParamValue, qc.getPattern().at(0).getPattern().getRightParam().value);

		arg1 = "v";
		arg2 = "_\"(x+y)*z\"_";

		firstParamValue = "v";
		secondParamValue = "x|y|+|z|*|";

		//valid
		Assert::AreEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(ASSIGN), static_cast<int>(qc.getPattern().at(1).getPattern().getEntity().type));
		Assert::AreEqual(entity, qc.getPattern().at(1).getPattern().getEntity().value);
		Assert::AreEqual(static_cast<int>(VARIABLE), static_cast<int>(qc.getPattern().at(1).getPattern().getLeftParam().type));
		Assert::AreEqual(firstParamValue, qc.getPattern().at(1).getPattern().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(EXPR), static_cast<int>(qc.getPattern().at(1).getPattern().getRightParam().type));
		Assert::AreEqual(secondParamValue, qc.getPattern().at(1).getPattern().getRightParam().value);

		arg1 = "v";
		arg2 = "\"x	+ y - 1\"";

		firstParamValue = "v";
		secondParamValue = "x|y|+|1|-|";

		//valid
		Assert::AreEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(ASSIGN), static_cast<int>(qc.getPattern().at(2).getPattern().getEntity().type));
		Assert::AreEqual(entity, qc.getPattern().at(2).getPattern().getEntity().value);
		Assert::AreEqual(static_cast<int>(VARIABLE), static_cast<int>(qc.getPattern().at(2).getPattern().getLeftParam().type));
		Assert::AreEqual(firstParamValue, qc.getPattern().at(2).getPattern().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(EXPR_EXACT), static_cast<int>(qc.getPattern().at(2).getPattern().getRightParam().type));
		Assert::AreEqual(secondParamValue, qc.getPattern().at(2).getPattern().getRightParam().value);

		arg1 = "v";
		arg2 = "_";

		firstParamValue = "v";
		secondParamValue = "_";

		//valid
		Assert::AreEqual(true, preprocessor.parsePattern(qc, WHILE, entity, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(WHILE), static_cast<int>(qc.getPattern().at(3).getPattern().getEntity().type));
		Assert::AreEqual(entity, qc.getPattern().at(3).getPattern().getEntity().value);
		Assert::AreEqual(static_cast<int>(VARIABLE), static_cast<int>(qc.getPattern().at(3).getPattern().getLeftParam().type));
		Assert::AreEqual(firstParamValue, qc.getPattern().at(3).getPattern().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(ALL), static_cast<int>(qc.getPattern().at(3).getPattern().getRightParam().type));
		Assert::AreEqual(secondParamValue, qc.getPattern().at(3).getPattern().getRightParam().value);

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

		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, invalidArg1Empty, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, invalidArg1EntRef1, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, invalidArg1EntRef2, arg2, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, invalidArg1StmtRefNotExist, arg2, false));

		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, invalidArg2Empty, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, invalidArg2ExpressSpec, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, invalidExpr1, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, invalidExpr2, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, invalidExpr3, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, invalidExpr4, false));
		Assert::AreNotEqual(true, preprocessor.parsePattern(qc, ASSIGN, entity, arg1, invalidExpr5, false));
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

		QueryContent qc;

		string arg1 = "w.stmt#";
		string arg2 = "3";

		string firstParamValue = "w";
		string secondParamValue = "3";

		//Valid
		Assert::AreEqual(true, preprocessor.parseWithClause(qc, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(With), static_cast<int>(qc.getWithClauses().at(0).getWithClause().getRelRef()));
		Assert::AreEqual(static_cast<int>(WHILE), static_cast<int>(qc.getWithClauses().at(0).getWithClause().getLeftParam().type));
		Assert::AreEqual(static_cast<int>(STMT_NO), static_cast<int>(qc.getWithClauses().at(0).getWithClause().getLeftParam().attribute));
		Assert::AreEqual(firstParamValue, qc.getWithClauses().at(0).getWithClause().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(INTEGER), static_cast<int>(qc.getWithClauses().at(0).getWithClause().getRightParam().type));
		Assert::AreEqual(static_cast<int>(NONE), static_cast<int>(qc.getWithClauses().at(0).getWithClause().getRightParam().attribute));
		Assert::AreEqual(secondParamValue, qc.getWithClauses().at(0).getWithClause().getRightParam().value);

		arg1 = "n";
		arg2 = "c.stmt#";

		firstParamValue = "n";
		secondParamValue = "c";

		//Valid
		Assert::AreEqual(true, preprocessor.parseWithClause(qc, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(With), static_cast<int>(qc.getWithClauses().at(1).getWithClause().getRelRef()));
		Assert::AreEqual(static_cast<int>(PROG_LINE), static_cast<int>(qc.getWithClauses().at(1).getWithClause().getLeftParam().type));
		Assert::AreEqual(static_cast<int>(NONE), static_cast<int>(qc.getWithClauses().at(1).getWithClause().getLeftParam().attribute));
		Assert::AreEqual(firstParamValue, qc.getWithClauses().at(1).getWithClause().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(CALL), static_cast<int>(qc.getWithClauses().at(1).getWithClause().getRightParam().type));
		Assert::AreEqual(static_cast<int>(STMT_NO), static_cast<int>(qc.getWithClauses().at(1).getWithClause().getRightParam().attribute));
		Assert::AreEqual(secondParamValue, qc.getWithClauses().at(1).getWithClause().getRightParam().value);

		arg1 = "p.procName";
		arg2 = "\"first\"";

		firstParamValue = "p";
		secondParamValue = "first";

		//Valid
		Assert::AreEqual(true, preprocessor.parseWithClause(qc, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(With), static_cast<int>(qc.getWithClauses().at(2).getWithClause().getRelRef()));
		Assert::AreEqual(static_cast<int>(PROCEDURE), static_cast<int>(qc.getWithClauses().at(2).getWithClause().getLeftParam().type));
		Assert::AreEqual(static_cast<int>(PROCNAME), static_cast<int>(qc.getWithClauses().at(2).getWithClause().getLeftParam().attribute));
		Assert::AreEqual(firstParamValue, qc.getWithClauses().at(2).getWithClause().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(PROC_IDENT), static_cast<int>(qc.getWithClauses().at(2).getWithClause().getRightParam().type));
		Assert::AreEqual(static_cast<int>(NONE), static_cast<int>(qc.getWithClauses().at(2).getWithClause().getRightParam().attribute));
		Assert::AreEqual(secondParamValue, qc.getWithClauses().at(2).getWithClause().getRightParam().value);

		arg1 = "p.procName";
		arg2 = "c.procName";

		firstParamValue = "p";
		secondParamValue = "c";

		//Valid
		Assert::AreEqual(true, preprocessor.parseWithClause(qc, arg1, arg2, false));
		Assert::AreEqual(static_cast<int>(With), static_cast<int>(qc.getWithClauses().at(3).getWithClause().getRelRef()));
		Assert::AreEqual(static_cast<int>(PROCEDURE), static_cast<int>(qc.getWithClauses().at(3).getWithClause().getLeftParam().type));
		Assert::AreEqual(static_cast<int>(PROCNAME), static_cast<int>(qc.getWithClauses().at(3).getWithClause().getLeftParam().attribute));
		Assert::AreEqual(firstParamValue, qc.getWithClauses().at(3).getWithClause().getLeftParam().value);
		Assert::AreEqual(static_cast<int>(CALL), static_cast<int>(qc.getWithClauses().at(3).getWithClause().getRightParam().type));
		Assert::AreEqual(static_cast<int>(PROCNAME), static_cast<int>(qc.getWithClauses().at(3).getWithClause().getRightParam().attribute));
		Assert::AreEqual(secondParamValue, qc.getWithClauses().at(3).getWithClause().getRightParam().value);
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
#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/PKB.h";
#include "../SPA/Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace PKBParserIntegrationTesting
{
	PKB pkb;
	Parser parser;
	string simpleSource;
	string testString;

	TEST_CLASS(PKBParserIntegrationFollowsParent)
	{
	public:

		TEST_CLASS_INITIALIZE(setup)
		{
			parser = Parser();
			simpleSource = "";
			/* 
				procedure a{ 
				1.	a = b; 
				2.	while c { 
				3.		b = a;
				4.		d = 7; } 
				5.	if a then { 
				6.		while e { 
				7.			c = 4; }} 
					else { 
				8.		d = 1; } 
				9.	e = 1; }

				procedure b{
				10.	a = b;
				11.	while a {
				12.		while b {
				13.			a = 1; }}
			
			*/
			testString = "procedure a {a = 7; while c { b = a; d = 7; } if a then { while e { c = 4; } } else { d = 1; } e = 1; } procedure b{ a = b; while a { while b { a = 1; }} }";
			pkb = parser.Parse(simpleSource, pkb, true, testString);
		}
		/*

		TEST_METHOD(PKBParserGetFollowsBeforeInvalid)
		{
			Assert::AreEqual(0, pkb.checkFollowsAfter(1));
		}

		TEST_METHOD(PKBParserGetFollowsBefore)
		{
			Assert::AreEqual(1, pkb.checkFollowsAfter(2));
		}

		TEST_METHOD(PKBParserGetFollowsAfter)
		{
			Assert::AreEqual(2, pkb.checkFollowsAfter(1));
		}

		TEST_METHOD(PKBParserGetFollowsAfterInvalid)
		{
			Assert::AreEqual(0, pkb.checkFollowsAfter(13));
		}

		TEST_METHOD(PKBParserCheckFollows)
		{
			Assert::AreEqual(true, pkb.checkFollows(1, 2));
		}

		TEST_METHOD(PKBParserCheckFollowsInValid)
		{
			Assert::AreEqual(false, pkb.checkFollows(2, 3));
		}*/

		TEST_METHOD(PKBParserGetAllFollows)
		{
			std::vector <std::vector<int>> output = pkb.getAllFollows();
			std::vector <std::vector<int>> data = { {3,4},{ 1,2 },{ 2,5 },{ 5,9 } ,{10,11} };
			for (int i = 0; i < data.size(); i++) {
				Assert::AreEqual(true, (output[i] == data[i]));
			}
		}

		TEST_METHOD(PKBParserGetAllFollowsStar)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 1, {2, 5, 9} });
			data.insert({ 2,{ 5, 9 } });
			data.insert({ 3,{ 4 } });
			data.insert({ 5,{ 9 } });
			data.insert({ 10,{ 11 } });
			unordered_map<int, std::vector<int>> output = pkb.getAllFollowsStar();
			for (auto it = output.begin(); it != output.end(); ++it) {
				Assert::AreEqual(true, (data[it->first] == it->second));
			}
		}

		TEST_METHOD(PKBParserGetAllParent)
		{
			std::vector <std::vector<int>> output = pkb.getAllParent();
			std::vector <std::vector<int>> data = { { 2, 3 },{ 2, 4 },{ 5, 6 },{ 6, 7 } ,{ 5, 8 } , {11,12}, {12,13} };
			for (int i = 0; i < data.size(); i++) {
				Assert::AreEqual(true, (output[i] == data[i]));
			}
		}

		TEST_METHOD(PKBParserGetAllParentStar)
		{
			unordered_map<int, std::vector<int>> data;
			data.insert({ 2,{ 3, 4 } });
			data.insert({ 5,{ 6, 7, 8 } });
			data.insert({ 6,{ 7 } });
			data.insert({ 11,{ 12,13 } });
			data.insert({ 12,{ 13 } });
			unordered_map<int, std::vector<int>> output = pkb.getAllParentStar();
			for (auto it = output.begin(); it != output.end(); ++it) {
				Assert::AreEqual(true, (data[it->first] == it->second));
			}
		}

	};
}
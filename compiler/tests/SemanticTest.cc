/*
 * LexTest.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: eshinig
 */
#include <LexicalAnalyzer.hh>
#include <Syntatic.hh>
#include <Semantic.h>
#include "gtest/gtest.h"
#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>
#include<fstream>
#include<list>
#include <algorithm>

class SemanticTest: public ::testing::Test
{
public:
    Lex l;

    SemanticTest()
    {
    }

    void SetUp()
    {
    }

    void TearDown()
    {
        Semantic::currentTable.clear();
        Semantic::nonTerminalSymValue.clear();
        Semantic::symbolTables.clear();
    }

};

TEST_F(SemanticTest,SymbolTableFull)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{ "
            "int mc1v1[2][4]; float mc1v2; MyClass2 mc1v3[3]; "
            "int mc1f1(int p1, MyClass2 p2[3]){ MyClass2 fv1[3];};"
            "int f2(MyClass1 f2p1[3]){int mc1v1;};"
            "};"
            "class MyClass2 "
            "{"
            "int mc1v1[2][4];"
            "float fp1;"
            "MyClass2 m2[3];"
            "};"
            "program { int m1; float m2[3][2]; }; "
            "float f1(int fp1[2][2], float fp2){ MyClass fv1[3]; int fv2;}; "
            "int f2(){};";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

//    /*
//     * Check the Lexical Analyzer, token list should have 132 tokens
//     */
    EXPECT_EQ(l.tokenList.size(), 132);

    Parser p(l.getTokenDSList());

    SyntaticTokenValue tv;
       tv.tds.lineNum = 0;
       tv.tds.type = NONE;
       tv.tds.value = "$";
       tv.syntacticValue="$";
    p.inputSemanticValue.push_back(tv);
//
//   //Parser Input contains 132 tokens + '$' as 133th token

    EXPECT_EQ(p.productions.size(), 93);// based on grammer
//
//
//    //Start parsing
    p.tableDrivenParserAlgorithm();
//
//    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");
//
//    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),132);

    //8 tables are created i.e. global, f1, f2, MyClass1,MyClass2,program ,mc1f1 and f2
    EXPECT_EQ(Semantic::symbolTables.size(),8);

    //Table global contains 5 symbols--> f1 , f2, MyClass1, MyClass2, program
    EXPECT_EQ(Semantic::symbolTables.find("global")->second.size(),5);

    //Table program contains 3
    EXPECT_EQ(Semantic::symbolTables.find("program")->second.size(),3);

    //Table program contains 4
    EXPECT_EQ(Semantic::symbolTables.find("f1")->second.size(),4);
}

TEST_F(SemanticTest,SymbolTableGlobalFunctions)
{

    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{ "
            "};"
            "class MyClass2 "
            "{"
            "};"
            "program { }; "
            "float f1(){}; "
            "int f2(){};";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    /*
     * Check the Lexical Analyzer, token list should have 49 tokens
     */
    EXPECT_EQ(l.tokenList.size(), 28);

    /*
     * Send the tokens from Lex to Parser
     */
    Parser p(l.getTokenDSList());

    SyntaticTokenValue tv;
       tv.tds.lineNum = 0;
       tv.tds.type = NONE;
       tv.tds.value = "$";
       tv.syntacticValue="$";
    p.inputSemanticValue.push_back(tv);

   //Parser Input contains 28 tokens + '$' as 29th token
    EXPECT_EQ(p.inputSemanticValue.size(),29);

    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start parsing
    p.tableDrivenParserAlgorithm();

//    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),28);

//    8 tables are created i.e. global, f1, f2, MyClass1,MyClass2,program
    EXPECT_EQ(Semantic::symbolTables.size(),6);
//
//    Table contains 5 symbols--> f1 , f2, MyClass1, MyClass2, program
    EXPECT_EQ(Semantic::symbolTables.find("global")->second.size(),5);

}


TEST_F(SemanticTest,SymbolTableCreateGlobalAndFunctionTable)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{ "
            "};"
            "class MyClass2 "
            "{"
            "};"
            "program { }; "
            "float f1(int fp1[2][4], float fp2){ int v1[10]; float v2; }; "
            "int f2(){};";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    /*
     * Check the Lexical Analyzer, token list should have 39 tokens
     */
    EXPECT_EQ(l.tokenList.size(), 48);

    /*
     * Send the tokens from Lex to Parser
     */
    Parser p(l.getTokenDSList());

    SyntaticTokenValue tv;
       tv.tds.lineNum = 0;
       tv.tds.type = NONE;
       tv.tds.value = "$";
       tv.syntacticValue="$";
    p.inputSemanticValue.push_back(tv);

   //Parser Input contains 48 tokens + '$' as 49th token
    EXPECT_EQ(p.inputSemanticValue.size(),49);

    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start parsing
    p.tableDrivenParserAlgorithm();

//    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),48);

    //8 tables are created i.e. global, f1, f2, MyClass1,MyClass2,program
    EXPECT_EQ(Semantic::symbolTables.size(),6);

    //Table contains 5 symbols--> fp1 fp2 v1 v2;
    EXPECT_EQ(Semantic::symbolTables.find("f1")->second.size(),4);
}

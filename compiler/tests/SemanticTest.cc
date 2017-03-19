/*
 * LexTest.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: eshinig
 */
#include <LexicalAnalyzer.hh>
#include <Syntatic.hh>
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
    }

};

TEST_F(SemanticTest,SymbolTable)
{
    //Input list consist of following 49 tokens
    /*
     * class id { id id [ integer ] ; } ;  //11 tokens
     * program { int id [ integer ] ; float id [ integer ] ; } ; //16 tokens
     * float id ( int id [ integer ] ) { float id ; return ( num * num ) ; } //22 tokens;
     */
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
//     * Check the Lexical Analyzer, token list should have 49 tokens
//     */
//    EXPECT_EQ(l.tokenList.size(), 49);
//
//    l.printTokenDataStruct();
//
    Parser p(l.getTokenDSList());
//
//
    SyntaticTokenValue tv;
       tv.tds.lineNum = 0;
       tv.tds.type = NONE;
       tv.tds.value = "$";
       tv.syntacticValue="$";
    p.inputSemanticValue.push_back(tv);
//
//   //Parser Input contains 49 tokens + '$' as 50th token
//
//    EXPECT_EQ(p.input.size(),50);
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
//    EXPECT_EQ(p.derivation.size(),49);
}


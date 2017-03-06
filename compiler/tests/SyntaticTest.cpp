/*
 * SyntaticTest.cpp
 *
 *  Created on: Feb 25, 2017
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
#include "LexicalAnalyzer.hh"

class SyntaticTest: public ::testing::Test
{
public:



    SyntaticTest()
{
}

    void SetUp()
    {

    }

    void TearDown()
    {
    }
};


TEST_F(SyntaticTest,classProgramFunction)
{
    Parser p;
    Lex l;
    l.currentCharIndex = 0;
    //INput list consist of following 50 tokens: NOTE:Added '$' as 50th token
    /*
     * class id { id id [ integer ] ; } ;
     * program { int id [ integer ] ; float id [ integer ] ; } ;
     * float id ( int id [ integer ] ) { float id ; return ( num * num ) ; } $
     */
    EXPECT_EQ(p.input.size(),50);
    ASSERT_EQ(p.productions.size(), 86);// based on grammer

    //Start parsing
    p.tableDrivenParserAlgorithm();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.inverseDerivation.size(),1);
    EXPECT_EQ(p.inverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input -1
    EXPECT_EQ(p.derivation.size(),49);
}

TEST_F(SyntaticTest,classProgramFunctionLexical)
{
    //Input list consist of following 49 tokens
    /*
     * class id { id id [ integer ] ; } ;  //11 tokens
     * program { int id [ integer ] ; float id [ integer ] ; } ; //16 tokens
     * float id ( int id [ integer ] ) { float id ; return ( num * num ) ; } //22 tokens;
     */
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken = "class utility "
            "{ utility utility [ 5 ] ; } ;"
            "program "
            "{ int x [ 10 ] ; "
            "float y [ 10 ] ; "
            "} ;"
            "float randomize ( int x [ 10 ] ) "
            "{ float y ; "
            "return ( x x ) ; "
            "} ;";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 49 tokens
     */
    EXPECT_EQ(l.tokenList.size(), 49);

    Parser p(l.getTokenDSList());

    /*
     * Parser Input contains 49 tokens + '$' as 50th token
     */
    EXPECT_EQ(p.input.size(),50);
    EXPECT_EQ(p.productions.size(), 86);// based on grammer

    //Start parsing
    p.tableDrivenParserAlgorithm();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.inverseDerivation.size(),1);
    EXPECT_EQ(p.inverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),49);
}

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
    ASSERT_EQ(p.productions.size(), 93);// based on grammar

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
            "return ( 10 * 9.3 ) ; "
            "} ;";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 49 tokens
     */
    EXPECT_EQ(l.tokenList.size(), 49);

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList());


    p.input.push_back("$");

   //Parser Input contains 49 tokens + '$' as 50th token

    EXPECT_EQ(p.input.size(),50);
    EXPECT_EQ(p.productions.size(), 93);// based on grammer


    //Start parsing
    p.tableDrivenParserAlgorithm();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.inverseDerivation.size(),1);
    EXPECT_EQ(p.inverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),49);
}


TEST_F(SyntaticTest,classProgramFunctionForLexical)
{
    //Input list consist of following 69 tokens
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken = "class xyz { int x [ 11 ] ; } ; "
            "program { int a [ 110 ] ; float b [ 120 ] ; } ; "
            "float multiplication ( int c [ 140 ] ) "
            "{ "
            "float x ; "
            "for ( int i = 20 ; i < 40 ; i = ( k ) + 10 ) ; "
            "return ( 12 * 12.01 ) ; "
            "} ;";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 49 tokens
     */
    EXPECT_EQ(l.tokenList.size(), 69);

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList());


    p.input.push_back("$");

   //Parser Input contains 69 tokens + '$' as 70th token

    EXPECT_EQ(p.input.size(),70);
    EXPECT_EQ(p.productions.size(), 93);// based on grammer


    //Start parsing
    p.tableDrivenParserAlgorithm();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.inverseDerivation.size(),1);
    EXPECT_EQ(p.inverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),69);
}




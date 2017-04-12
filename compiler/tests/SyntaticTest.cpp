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
#include "Exceptions.hh"

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
    Parser p("classProgramFunction");
    Lex l("classProgramFunction");
    l.currentCharIndex = 0;
    //INput list consist of following 50 tokens: NOTE:Added '$' as 50th token
    /*
     * class id { id id [ integer ] ; } ;
     * program { int id [ integer ] ; float id [ integer ] ; } ;
     * float id ( int id [ integer ] ) { float id ; return ( num * num ) ; } $
     */
    EXPECT_EQ(p.inputSemanticValue.size(),50);
    ASSERT_EQ(p.productions.size(), 93);// based on grammar

    //Start parsing
    p.twoPassParser();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

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
    Lex l("classProgramFunctionLexical");
    l.currentCharIndex = 0;
    l.rawToken = "class utility "
            "{ utility utility [ 5 ] ; } ;"
            "program "
            "{ int x [ 10 ] ; "
            "float y [ 10 ] ; "
            "} ;"
            "float randomize ( int x [ 10 ] ) "
            "{ float y ; "
            "return ( y * y ) ; "
            "} ;";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 49 tokens
     */
    EXPECT_EQ(l.tokenList.size(), 49);

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList(),"classProgramFunctionLexical");

    //Parser Input contains 49 tokens + '$' as 50th token

    EXPECT_EQ(p.inputSemanticValue.size(),50);
    EXPECT_EQ(p.productions.size(), 93);// based on grammer


    //Start parsing
    p.twoPassParser();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),49);
}


TEST_F(SyntaticTest,ComplexExpression)
{
    //Input list consist of following 69 tokens
    Lex l("ComplexExpression");
    l.currentCharIndex = 0;
    l.rawToken = "class xyz { int x [ 11 ] ; } ; "
            "program { int a [ 110 ] ; float b [ 120 ] ; } ; "
            "float multiplication ( int c [ 140 ] ) "
            "{ "
            "float x ; "
            "int i;"
            "k = 10;"
            "for ( int i = 20 ; i < 40 ; i =  i + 10 ) "
            " i = 10 ;;"
            " "
            "return ( x * x ) ; "
            "} "
            ""
            ""
            ""
            ";";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 49 tokens
     */
    EXPECT_EQ(78,l.tokenList.size());

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList(),"classProgramFunctionForLexical");


    //Parser Input contains 74 tokens + '$' as 75th token

    EXPECT_EQ(79,p.inputSemanticValue.size());
    EXPECT_EQ(p.productions.size(), 93);// based on grammer


    //Start parsing
    p.twoPassParser();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(78,p.derivation.size());
}

TEST_F(SyntaticTest,ConditionalStatement)
{
    //Input list consist of following 69 tokens
    Lex l("ConditionalStatement");
    l.currentCharIndex = 0;
    l.rawToken = "class xyz { int x [ 11 ] ; } ; "
            "program { int a ;"
            "          int b;"
            "          a = 10;"
            "          b = 20;"
            "          if ( a < b ) "
            "           then "
            "           a = b ;"
            "           else"
            "           b = a ; ; } ; ";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 46 tokens
     */
    EXPECT_EQ(46,l.tokenList.size());

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList(),"ConditionalStatement");

    //Parser Input contains 46 tokens + '$' as 47th token

    EXPECT_EQ(47,p.inputSemanticValue.size());
    EXPECT_EQ(p.productions.size(), 93);// based on grammer


    //Start parsing
    p.twoPassParser();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(46,p.derivation.size());
}


TEST_F(SyntaticTest,ClassDeclarationDataMembers)
{
    //Input list consist of following 69 tokens
    Lex l("ClassDeclarationDataMembers");
    l.currentCharIndex = 0;
    l.rawToken = "class xyz { "
            "                 int a [ 10 ] ;"
            "                 int b; "
            "                 float c;"
            "                 int f1( int d )"
            "                 { return ( d*d ) ; }; "
            "                } ; "
            "program {  } ; ";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 37 tokens
     */
    EXPECT_EQ(37,l.tokenList.size());

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList(),"ClassDeclarationDataMembers");

    //Parser Input contains 37 tokens + '$' as 38th token

    EXPECT_EQ(38,p.inputSemanticValue.size());

    //Start parsing
    p.twoPassParser();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(37,p.derivation.size());
}

TEST_F(SyntaticTest,ClassDataMembersAccess)
{
    /*
     * this TC does not work due to lexical error. Show the tree in KFG
     * class id { int id ; float id ; } ; program { id id ; id . id = id ; } ;
     */

    Lex l("ClassDataMembersAccess");
    l.currentCharIndex = 0;
    l.rawToken = "class XYZ { "
            "                 int a [ 10 ] ;"
            "                 int b; "
            "                 float c;"
            "                 int f1( int d )"
            "                 { return ( d*d ) ; }; "
            "                } ; "
            "     program   {  XYZ x; "
            "                  x.b = 10 ;"
            "               } ; ";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 45 tokens
     */
    EXPECT_EQ(46,l.tokenList.size());

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList(),"ClassDataMembersAccess");

    //Parser Input contains 45 tokens + '$' as 46th token

    EXPECT_EQ(47,p.inputSemanticValue.size());

    //Start parsing
    p.twoPassParser();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(46,p.derivation.size());
}

TEST_F(SyntaticTest,ArrayAccess)
{
    Lex l("ClassDataMembersAccess");
    l.currentCharIndex = 0;
    l.rawToken = "class XYZ { "
           "                } ; "
            "     program   {  int x[100]; "
            "                  x[100] = 10 ;"
            "               } ; ";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 22 tokens
     */
    EXPECT_EQ(22,l.tokenList.size());

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList(),"ClassDataMembersAccess");

    //Parser Input contains 22 tokens + '$' as 23rd token

    EXPECT_EQ(23,p.inputSemanticValue.size());

    //Start parsing
    p.twoPassParser();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(22,p.derivation.size());
}

TEST_F(SyntaticTest,SyntacticErrorRecoverySemiColonMissing)
{
    Lex l("SyntacticErrorRecoverySemiColonMissing");
    l.currentCharIndex = 0;
    l.rawToken = "class xyz { int x [ 11 ] ; } ; "
            "program { int a [ 110 ] ; float b [ 120 ] ; } ; "
            "float function ( int c [ 140 ] ) "
            "{ "
            "int x /*No Semicolon to show error recovery*/ "
            "int k; "

            "} ;";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 44 tokens
     */
    EXPECT_EQ(44,l.tokenList.size());

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList(),"SyntacticErrorRecoverySemiColonMissing");

    //Parser Input contains 44 tokens + '$' as 45th token
    EXPECT_EQ(45,p.inputSemanticValue.size());

    //Start parsing
    EXPECT_THROW(p.twoPassParser(),SyntaticException);

    EXPECT_EQ(false,p.parsing);
    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing 2 tokens are missing because of error recovery 44-2 = 42
    EXPECT_EQ(42,p.derivation.size());

    //Parsisg should fail
    EXPECT_EQ(false,p.parsing);
}

TEST_F(SyntaticTest,SyntacticErrorRecoveryIDMissing)
{
    Lex l("SyntacticErrorRecoveryIDMissing");
    l.currentCharIndex = 0;
    l.rawToken = "class xyz { int x [ 11 ] ; } ; "
            "program { int a [ 110 ] ; float b [ 120 ] ; } ; "
            "float function (int c [ 140 ] ) "
            "{ "
            "int  ;/*No ID to show error recovery*/"
            "int k; "
            "}; ";

    l.findTokenTypeAndBuildList();

    /*
     * Check the Lexical Analyzer, token list should have 44 tokens
     */
    EXPECT_EQ(44,l.tokenList.size());

    l.printTokenDataStruct();

    Parser p(l.getTokenDSList(),"SyntacticErrorRecoveryIDMissing");

    //Parser Input contains 44 tokens + '$' as 45th token
    EXPECT_EQ(45,p.inputSemanticValue.size());

    //Start parsing
    EXPECT_THROW(p.twoPassParser(),SyntaticException);

    EXPECT_EQ(false,p.parsing);
    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing 1 token is missing because of error recovery 44-2 = 42
    EXPECT_EQ(42,p.derivation.size());

    //Parsisg should fail
    EXPECT_EQ(false,p.parsing);
}



/*
 * LexTest.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: eshinig
 */
#include <LexicalAnalyzer.hh>
#include <Exceptions.hh>
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
    SyntaticTokenValue tv;

    SemanticTest()
    {
    }

    void SetUp()
    {
        Semantic::semanticStack.clear();
    }

    void TearDown()
    {
        Semantic::currentTable.clear();
        Semantic::nonTerminalSymValue.clear();
        Semantic::symbolTables.clear();
        Semantic::semanticStack.clear();
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
            "float f1(int fp1[2][2], float fp2){ MyClass1 fv1[3]; int fv2;}; "
            "int f2(){};";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    //    /*
    //     * Check the Lexical Analyzer, token list should have 132 tokens
    //     */
    EXPECT_EQ(132,l.tokenList.size());

    Parser p(l.getTokenDSList());

    //Parser Input contains 132 tokens + '$' as 133th token
    EXPECT_EQ(p.productions.size(), 93);// based on grammer

    //Start parsing
    p.twoPassParser();

    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");
    //
    //    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),132);

    //8 tables are created i.e. global, f1, f2, MyClass1,MyClass2,program ,mc1f1 and f2
    EXPECT_EQ(Semantic::symbolTables.size(),8);

    //Table global contains 5 symbols--> f1 , f2, MyClass1, MyClass2, program
    EXPECT_EQ(Semantic::symbolTables.find("global")->second.size(),5);

    //Table program contains 3 i.e. m1, m2 and a empty symbol(due to implementation)
    EXPECT_EQ(Semantic::symbolTables.find("program")->second.size(),3);

    //Table f1 contains 5 i.e. fp1 , fp2, fv1, fv2 and a empty line
    EXPECT_EQ(5,Semantic::symbolTables.find("f1")->second.size());

    //Table f2 contains a empty line
    EXPECT_EQ(1,Semantic::symbolTables.find("f2")->second.size());

    //Table MyClass2 contains    fp1,  m2 ,mc1v1 a empty line
    EXPECT_EQ(4,Semantic::symbolTables.find("MyClass2")->second.size());

    //Table MyClass1 contains  f2, mc1f1, mc1v1, mc1v2 mc1v3 a empty line
    EXPECT_EQ(6,Semantic::symbolTables.find("MyClass1")->second.size());

    //Table MyClass1:f2 contains  f2p1, mc1v1  a empty line
    EXPECT_EQ(3,Semantic::symbolTables.find("MyClass1:f2")->second.size());

    //Table MyClass1:mc1f1 contains  fv1, p1, p2  a empty line
    EXPECT_EQ(4,Semantic::symbolTables.find("MyClass1:mc1f1")->second.size());

    //Next step is to test this table : MyClass1 for its name, kind , type and link
    //+--------------------------------MyClass1--------------------------------------------+
    //                Name                Kind                type                link
    //
    //                  f2            function                  f2                  f2
    //               mc1f1            function               mc1f1               mc1f1
    //               mc1v1            variable           int[2][4]                NONE
    //               mc1v2            variable               float                NONE
    //               mc1v3            variable         MyClass2[3]                NONE
    //+-----------------------------------------------------------------------------------+

    SymbolInfo symInfo;
    symInfo = Semantic::symbolTables["MyClass1"]["f2"];
    EXPECT_EQ("function",symInfo.kind);
    EXPECT_EQ("int",symInfo.type);
    EXPECT_EQ("MyClass1:f2",symInfo.link);

    symInfo = Semantic::symbolTables["MyClass1"]["mc1f1"];
    EXPECT_EQ("function",symInfo.kind);
    EXPECT_EQ("int",symInfo.type);
    EXPECT_EQ("MyClass1:mc1f1",symInfo.link);

    symInfo = Semantic::symbolTables["MyClass1"]["mc1v1"];
    EXPECT_EQ("variable",symInfo.kind);
    EXPECT_EQ("int[2][4]",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["MyClass1"]["mc1v2"];
    EXPECT_EQ("variable",symInfo.kind);
    EXPECT_EQ("float",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["MyClass1"]["mc1v3"];
    EXPECT_EQ("variable",symInfo.kind);
    EXPECT_EQ("MyClass2[3]",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);
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

    //Parser Input contains 28 tokens + '$' as 29th token
    EXPECT_EQ(p.inputSemanticValue.size(),29);

    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start parsing
    p.twoPassParser();

    //    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(p.stackInverseDerivation.size(),1);
    EXPECT_EQ(p.stackInverseDerivation.front(),"$");

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(p.derivation.size(),28);

    //    8 tables are created i.e. global, f1, f2, MyClass1,MyClass2,program
    EXPECT_EQ(6,Semantic::symbolTables.size());
    //
    //    Table contains 5 symbols--> f1 , f2, MyClass1, MyClass2, program
    EXPECT_EQ(5,Semantic::symbolTables.find("global")->second.size());


    //Table contains 1 symbols--> "" (empty by default);
    EXPECT_EQ(1,Semantic::symbolTables.find("program")->second.size());
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
            "float f1(int fp1[2][4], float fp2, MyClass2 mc[5]){ int v1[10]; float v2; MyClass2 mc1[5];}; "
            "int f2(){};";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    /*
     * Check the Lexical Analyzer, token list should have 39 tokens
     */
    EXPECT_EQ(60,l.tokenList.size());

    /*
     * Send the tokens from Lex to Parser
     */
    Parser p(l.getTokenDSList());


    //Parser Input contains 60 tokens + '$' as 61st token
    EXPECT_EQ(p.inputSemanticValue.size(),61);

    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start parsing
    p.twoPassParser();

    //    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(1,p.stackInverseDerivation.size());
    EXPECT_EQ("$",p.stackInverseDerivation.front());

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(60,p.derivation.size());

    //6 tables are created i.e. global, f1, f2, MyClass1,MyClass2,program
    EXPECT_EQ(Semantic::symbolTables.size(),6);

    //Table contains 5 symbols--> fp1 fp2 v1 v2 and "" (empty by default);
    EXPECT_EQ(7,Semantic::symbolTables.find("f1")->second.size());

    SymbolInfo symInfo;
    symInfo = Semantic::symbolTables["f1"]["fp1"];
    EXPECT_EQ("parameter",symInfo.kind);
    EXPECT_EQ("int[2][4]",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["f1"]["fp2"];
    EXPECT_EQ("parameter",symInfo.kind);
    EXPECT_EQ("float",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["f1"]["fp1"];
    EXPECT_EQ("parameter",symInfo.kind);
    EXPECT_EQ("int[2][4]",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["f1"]["mc"];
    EXPECT_EQ("parameter",symInfo.kind);
    EXPECT_EQ("MyClass2[5]",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["f1"]["mc1"];
    EXPECT_EQ("variable",symInfo.kind);
    EXPECT_EQ("MyClass2[5]",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["f1"]["v1"];
    EXPECT_EQ("variable",symInfo.kind);
    EXPECT_EQ("int[10]",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["f1"]["v2"];
    EXPECT_EQ("variable",symInfo.kind);
    EXPECT_EQ("float",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    //Table contains 1 symbols--> "" (empty by default);
    EXPECT_EQ(1,Semantic::symbolTables.find("f2")->second.size());


    //Table contains 1 symbols--> "" (empty by default);
    EXPECT_EQ(1,Semantic::symbolTables.find("MyClass1")->second.size());

    //Table contains 1 symbols--> "" (empty by default);
    EXPECT_EQ(1,Semantic::symbolTables.find("MyClass2")->second.size());

    //Table contains 1 symbols--> "" (empty by default);
    EXPECT_EQ(1,Semantic::symbolTables.find("program")->second.size());
}




TEST_F(SemanticTest,SymbolTableProgramSubFuntion)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{"
            "};"
            "class MyClass2 "
            "{"
            "};"
            "program { int x ; int y[10]; }; "
            "int square ( int x ){ return (x*x); };";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    //    /*
    //     * Check the Lexical Analyzer, token list should have 39 tokens
    //     */
    EXPECT_EQ(39,l.tokenList.size());
    //
    //    /*
    //     * Send the tokens from Lex to Parser
    //     */
    Parser p(l.getTokenDSList());


    //Parser Input contains 39 tokens + '$' as 40th token
    EXPECT_EQ(40,p.inputSemanticValue.size());
    //
    //    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start Parsing and no exception should be thrown
    EXPECT_NO_THROW(p.twoPassParser());
    //    //Start parsing
//    +--------------------------------MyClass1--------------------------------------------+
//                    Name                Kind                type                link
//
//    +-----------------------------------------------------------------------------------+
//    +--------------------------------MyClass2--------------------------------------------+
//                    Name                Kind                type                link
//
//    +-----------------------------------------------------------------------------------+
//    +--------------------------------global--------------------------------------------+
//                    Name                Kind                type                link
//                MyClass1               class                                MyClass1
//                MyClass2               class                                MyClass2
//                 program            function                                 program
//                  square            function                 int              square
//    +-----------------------------------------------------------------------------------+
//    +--------------------------------program--------------------------------------------+
//                    Name                Kind                type                link
//
//                       x            variable                 int                NONE
//                       y            variable             int[10]                NONE
//    +-----------------------------------------------------------------------------------+
//    +--------------------------------square--------------------------------------------+
//                    Name                Kind                type                link
//
//                       x           parameter                 int                NONE
//    +-----------------------------------------------------------------------------------+


    //    //At the end of parsing the STACK should have only one element i.e. $
    EXPECT_EQ(1,p.stackInverseDerivation.size());
    EXPECT_EQ("$",p.stackInverseDerivation.front());

    //At the end of parsing the Derivation stack should be equal to input
    EXPECT_EQ(39,p.derivation.size());

    //5 tables are created i.e. global, MyClass1,MyClass2,program, square
    EXPECT_EQ(5,Semantic::symbolTables.size());

    //Table program contains 3 symbols--> x , y and "" (empty by default);
    EXPECT_EQ(3,Semantic::symbolTables.find("program")->second.size());

    SymbolInfo symInfo;
    symInfo = Semantic::symbolTables["program"]["x"];
    EXPECT_EQ("variable",symInfo.kind);
    EXPECT_EQ("int",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    symInfo = Semantic::symbolTables["program"]["y"];
    EXPECT_EQ("variable",symInfo.kind);
    EXPECT_EQ("int[10]",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    //Table square contains 2 symbols--> x "" (empty by default);
    EXPECT_EQ(2,Semantic::symbolTables.find("square")->second.size());
    symInfo = Semantic::symbolTables["square"]["x"];
    EXPECT_EQ("parameter",symInfo.kind);
    EXPECT_EQ("int",symInfo.type);
    EXPECT_EQ("NONE",symInfo.link);

    //Table gloabl contains 4 symbols-->  MyClass1,MyClass2,program, square
    EXPECT_EQ(4,Semantic::symbolTables.find("global")->second.size());
}


TEST_F(SemanticTest,SymbolTableMultipleDeclaration)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{"
            "};"
            "class MyClass2 "
            "{"
            "};"
            "program { int x ; int x[10]; x = square ( x ) ; }; "
            "int square ( int x ){ return (x*x); };";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    //    /*
    //     * Check the Lexical Analyzer, token list should have 39 tokens
    //     */
    EXPECT_EQ(46,l.tokenList.size());
    //
    //    /*
    //     * Send the tokens from Lex to Parser
    //     */
    Parser p(l.getTokenDSList());
    //
    //    //Parser Input contains 46 tokens + '$' as 47th token
    EXPECT_EQ(47,p.inputSemanticValue.size());
    //
    //    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start Parsing and semantic EXCEPTION should be thrown as
    //variable x is defined twice in program function
    EXPECT_THROW(p.twoPassParser(),SemanticException);
}

TEST_F(SemanticTest,TypeCheckingAssignment)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{"
            "};"
            "class MyClass2 "
            "{"
            "};"
            "program { int x ; int y ; y = x ; }; ";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    //    /*
    //     * Check the Lexical Analyzer, token list should have 39 tokens
    //     */
    EXPECT_EQ(24,l.tokenList.size());
    //
    //    /*
    //     * Send the tokens from Lex to Parser
    //     */
    Parser p(l.getTokenDSList());
    //
    //
    //    //Parser Input contains 46 tokens + '$' as 47th token
    EXPECT_EQ(25,p.inputSemanticValue.size());
    //
    //    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start Parsing and semantic EXCEPTION should be thrown as
    //variable x is defined twice in program function
    EXPECT_NO_THROW(p.twoPassParser());
}


TEST_F(SemanticTest,TypeCheckingAssignmentFail)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{"
            "};"
            "class MyClass2 "
            "{"
            "};"
            "program { int x ; float y ; y = x ; }; ";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    //    /*
    //     * Check the Lexical Analyzer, token list should have 39 tokens
    //     */
    EXPECT_EQ(24,l.tokenList.size());
    //
    //    /*
    //     * Send the tokens from Lex to Parser
    //     */
    Parser p(l.getTokenDSList());
    //
    //
    //    //Parser Input contains 46 tokens + '$' as 47th token
    EXPECT_EQ(25,p.inputSemanticValue.size());
    //
    //    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start Parsing and semantic EXCEPTION should be thrown as
    //variable x is defined twice in program function
    EXPECT_THROW(p.twoPassParser(),SemanticException);
}

TEST_F(SemanticTest,TypeCheckingAssignmentNestedFunction)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{"
            "};"
            "class MyClass2 "
            "{"
            "};"
            "program { int x ; int y ;}; "
            "";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    //    /*
    //     * Check the Lexical Analyzer, token list should have 39 tokens
    //     */
    EXPECT_EQ(46,l.tokenList.size());
    //
    //    /*
    //     * Send the tokens from Lex to Parser
    //     */
    Parser p(l.getTokenDSList());
    //
    //    //insert '$' for the parse input
    p.inputSemanticValue.push_back(tv);
    //
    //    //Parser Input contains 46 tokens + '$' as 47th token
    EXPECT_EQ(47,p.inputSemanticValue.size());
    //
    //    // based on grammar
    EXPECT_EQ(p.productions.size(), 93);

    //Start Parsing and semantic EXCEPTION should be thrown as
    //variable x is defined twice in program function
    EXPECT_NO_THROW(p.twoPassParser());
}

TEST_F(SemanticTest,SymbolTableTypeNotDefined)
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
            "program { int m1; float m2[3][2]; }; "
            "float f1(int fp1[2][2], float fp2){ MyClass1 fv1[3]; int fv2;}; "
            "int f2(){};";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();

    /*
     * Check the Lexical Analyzer, token list should have 132 tokens
     */
    EXPECT_EQ(109,l.tokenList.size());

    Parser p(l.getTokenDSList());

    //Parser Input contains 132 tokens + '$' as 133th token
    EXPECT_EQ(p.productions.size(), 93);// based on grammer

    //Start parsing
    EXPECT_THROW(p.twoPassParser(),SemanticException);
}

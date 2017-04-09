/*
 * CodeGenerationTest.cpp
 *
 *  Created on: Apr 9, 2017
 *      Author: eshinig
 */

#include <LexicalAnalyzer.hh>
#include <Exceptions.hh>
#include <Syntatic.hh>
#include <Semantic.h>
#include<CodeGenerator.h>
#include "gtest/gtest.h"
#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>
#include<fstream>
#include<list>
#include <algorithm>


class CodeGenerationTest: public ::testing::Test
{
public:
    Lex l;
    SyntaticTokenValue tv;

    CodeGenerationTest()
    {
    }

    void SetUp()
    {
        Semantic::semanticStack.clear();
    }

    void TearDown()
    {
        Semantic::currentTable.clear();
        Semantic::symbolTables.clear();
        Semantic::semanticStack.clear();
    }

};

TEST_F(CodeGenerationTest,ReadWrite)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{ "
            "};"
            "program "
            "{ "
            "int value;"
            "get ( value ) ;"
            "put ( value ) ; "
            "}; ";

    /*
     * this is expected code to be generated for the above program defined in line 50
     */
    std::string expectedGeneratedCode = "org\t217\n"
            "align\n"
            "entry                  % Start here\n"
            "getc\tr1\n"
            "sw\tvalue(r0),r1\n"
            "lw\tr10,value(r0)\n"
            "putc\tr10\n"
            "hlt\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "value\tdw\t0";
    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();
    Parser p(l.getTokenDSList());
    p.twoPassParser();

    /*
     * Read the file that contains the generated code
     */
    std::ifstream ifs("ReadWrite.m");
    std::string generatedCode( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    /*
     * Assert if the expected generated code and file generated containing the code
     * are same
     */
    EXPECT_EQ(expectedGeneratedCode,generatedCode);
}

TEST_F(CodeGenerationTest,ArithmeticAdd)
{
    Lex l;
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{ "
            "};"
            "program "
            "{ "
            "int x;"
            "int y;"
            "int z;"
            "x = y+z ;"
            "put ( x ) ; "
            "}; ";
    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();
    Parser p(l.getTokenDSList());
    p.twoPassParser();
}



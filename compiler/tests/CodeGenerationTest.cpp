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
    Lex l("ReadWrite");
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{"
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
    std::string expectedGeneratedCode =
            "org\t217\n"
            "align\n"
            "entry                  % Start here\n"
            "getc\tr1\n"
            "sw\tvalue(r0),r1\n"
            "lw\tr10,value(r0)\n"
            "putc\tr10\n"
            "hlt\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "value\tdw\t0\n";
    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();
    Parser p(l.getTokenDSList(),"ReadWrite");
    p.twoPassParser();

    /*
     * Read the file that contains the generated code
     */
    std::ifstream ifs(CodeGenerator::outputFileName.c_str());
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
    Lex l("ArithmeticAdd");
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
            "y = 10 ;"
            "z = 20 ;"
            "x = y+z+z ;"
            "put ( x ) ; "
            "}; ";
    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();
    Parser p(l.getTokenDSList(),"ArithmeticAdd");
    p.twoPassParser();
    std::string expectedGeneratedCode =
            "org\t217\n"
            "align\n"
            "entry                  % Start here\n"
            "addi\tr1,r0,10\n"
            "sb\ty(r0),r1\n"
            "addi\tr10,r0,20\n"
            "sb\tz(r0),r10\n"
            "lw\tr11,z(r0)\n"
            "lw\tr12,y(r0)\n"
            "add\tr13,r11,r12\n"
            "sb\tt0(r0),r13\n"
            "lw\tr1,z(r0)\n"
            "lw\tr10,t0(r0)\n"
            "add\tr11,r1,r10\n"
            "sb\tt1(r0),r11\n"
            "lb\tr1,t1(r0)\n"
            "sb\tx(r0),r1\n"
            "lw\tr10,x(r0)\n"
            "putc\tr10\n"
            "hlt\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "x\tdw\t0\n"
            "y\tdw\t0\n"
            "z\tdw\t0\n"
            "t0\tdw\t0\n"
            "t1\tdw\t0\n";
    /*
     * Read the file that contains the generated code
     */
    std::ifstream ifs(CodeGenerator::outputFileName.c_str());
    std::string generatedCode( (std::istreambuf_iterator<char>(ifs) ),
            (std::istreambuf_iterator<char>()    ) );

    /*
     * Assert if the expected generated code and file generated containing the code
     * are same
     */
    EXPECT_EQ(expectedGeneratedCode,generatedCode);
}


TEST_F(CodeGenerationTest,MemoryAllocationArray)
{
    Lex l("MemoryAllocationArray");
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{ "
            "};"
            "program "
            "{ "
            "int x[10];"
            "float y[10];"
            "}; ";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();
    Parser p(l.getTokenDSList(),"MemoryAllocationArray");
    p.twoPassParser();
    std::string expectedGeneratedCode =
            "org\t217\n"
            "align\n"
            "entry                  % Start here\n"
            "hlt\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "x\tres\t10\n"
            "y\tres\t20\n";
            /*
             * Read the file that contains the generated code
             */
            std::ifstream ifs(CodeGenerator::outputFileName.c_str());
    std::string generatedCode( (std::istreambuf_iterator<char>(ifs) ),
            (std::istreambuf_iterator<char>()    ) );
    /*
     * Assert if the expected generated code and file generated containing the code
     * are same
     */
    EXPECT_EQ(expectedGeneratedCode,generatedCode);
}

TEST_F(CodeGenerationTest,MemoryAllocationObject)
{
    Lex l("MemoryAllocationObject");
    l.currentCharIndex = 0;
    l.rawToken =
            "class MyClass1 "
            "{ "
            "int x;"
            "float y[10];"
            "int z;"
            "};"
            "program "
            "{ "
            "MyClass1 myc1;"
            "}; ";

    l.findTokenTypeAndBuildList();
    l.printTokenDataStruct();
    Parser p(l.getTokenDSList(),"MemoryAllocationObject");
    p.twoPassParser();
    std::string expectedGeneratedCode =
            "org\t217\n"
            "align\n"
            "entry                  % Start here\n"
            "hlt\n"
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n"
            "myc1\tres\t22\n";
            /*
             * Read the file that contains the generated code
             */
            std::ifstream ifs(CodeGenerator::outputFileName.c_str());
    std::string generatedCode( (std::istreambuf_iterator<char>(ifs) ),
            (std::istreambuf_iterator<char>()    ) );
    /*
     * Assert if the expected generated code and file generated containing the code
     * are same
     */
    EXPECT_EQ(expectedGeneratedCode,generatedCode);
}



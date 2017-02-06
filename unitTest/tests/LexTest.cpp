/*
 * LexTest.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: eshinig
 */
#include <LexicalAnalyzer.hh>
#include "gtest/gtest.h"
#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>
#include<fstream>
#include<list>
#include <algorithm>

class LexTest: public ::testing::Test
{
public:
    Lex l;

    LexTest()
    {
    }

    void SetUp()
    {
        l.currentCharIndex = 0;
    }

    void TearDown()
    {
    }

};

TEST_F(LexTest,Assignment)
{

    l.rawToken = "a=10;";
    l.findTokens();

    ASSERT_EQ(l.tokenList.size(), 4);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "a") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "10") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), ";") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,LetterDigitAssignment)
{

    l.rawToken = "aaaa34=34654;";
    l.findTokens();

    ASSERT_EQ(l.tokenList.size(), 4);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "aaaa34") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "34654") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,OperatorLessEqual)
{

    l.rawToken = "a<=1";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "<=") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,OperatorLess)
{

    l.rawToken = "a<1";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "<") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,OperatorGreater)
{
    l.rawToken = "a123>b12333";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), ">") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,OperatorMoreEqualWithSpace)
{
    l.rawToken = "a >= 1";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), " ") != l.tokenList.end());
    ASSERT_FALSE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), ">=") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,AddSubOperator)
{
    l.rawToken = "a=b+20-10;";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 8);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "+") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "-") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,DivMulOperator)
{
    l.rawToken = "a=b*10/20;";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 8);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "/") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "*") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,CurlyBrackets)
{
    l.rawToken = "{ ab21==10 }";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 5);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "{") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "}") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,ParenthesisBrackets)
{
    l.rawToken = "( ab21 == 10 )";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 5);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "(") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), ")") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "==") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,KeywordIF)
{
    l.rawToken = "if( ab21 <= 10 )";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 6);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "(") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), ")") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "<=") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,KeywordINT)
{
    l.rawToken = "int x = 10 ;";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 5);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "int") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "10") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,Float_1)
{
    l.rawToken = " int x =1.0 ;";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 5);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "int") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "1.0") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,Float_2)
{
    l.rawToken = " a = .0001 ;";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 4);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), ".0001") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}


TEST_F(LexTest,SqreBracket)
{
    l.rawToken = " int a[100] ;";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 6);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "[") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "]") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,SqreBracketAssignment)
{
    l.rawToken = " a12[100]=b;";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 7);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "[") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,Comma)
{
    l.rawToken = "int function(a,b)";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 7);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), ",") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,CommentsAfter)
{
    l.rawToken = "int x =10;/*asdfasdf*/";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 5);
    l.printTokenDataStruct();
}

TEST_F(LexTest,CommentsBefore)
{
    l.rawToken = "/*asdfasdf*/int x =10; ";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(), 5);
    l.printTokenDataStruct();
}

TEST_F(LexTest,mainDriver)
{
    using namespace std;
    using namespace boost;

    string line;
    int lineNumCount = 1;
    int totalTokensExpected = 58;
    int totalTokensActual;
    ifstream myfile("source_code.txt");
    char_separator<char> sept
    { "" };
    std::list<std::string> rawTokensFromInputFile;
    std::string srcFileName = "source_code.txt";
    Lex lexAnalyser;

    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            tokenizer tokens
            { line, sept };
            for (const auto &token : tokens)
            {
                Lex lex(token, lineNumCount);
                lex.findTokens();
                lexAnalyser.tokenListDS.insert(lexAnalyser.tokenListDS.end(),
                        lex.tokenListDS.begin(), lex.tokenListDS.end());
            }
            lineNumCount++;
        }
    }
    else
    {
        cout << "File cannot be opened";
    }

    lexAnalyser.printTokenDataStruct();
    totalTokensActual = lexAnalyser.getTokenDSList().size();
    std::cout << "Total Tokens=" << totalTokensActual;
    myfile.close();
    ASSERT_EQ(totalTokensActual,totalTokensExpected);
}

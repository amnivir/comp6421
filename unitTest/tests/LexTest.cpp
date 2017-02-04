/*
 * LexTest.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: eshinig
 */
#include <LexicalAnalyzer.hh>
#include "gtest/gtest.h"
#include <algorithm>

class LexTest: public ::testing::Test
{
public:
    Lex l;
};

TEST_F(LexTest,Assignment)
{

    l.currentCharIndex = 0;
    l.rawToken = "a=10;";
    l.findTokens();


    ASSERT_EQ(l.tokenList.size(),4);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "a") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "10") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), ";") != l.tokenList.end());
}

TEST_F(LexTest,LetterDigitAssignment)
{

    l.currentCharIndex = 0;
    l.rawToken = "a1=1;";
    l.findTokens();


    ASSERT_EQ(l.tokenList.size(),4);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "a1") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "1") != l.tokenList.end());
    ASSERT_TRUE(token);
}

TEST_F(LexTest,identifier)
{

    l.currentCharIndex = 0;
    l.rawToken = "aaaa34";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(),1);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "aaaa34") != l.tokenList.end());
    ASSERT_TRUE(token);
}

TEST_F(LexTest,Number)
{

    l.currentCharIndex = 0;
    l.rawToken = "34654";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(),1);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "34654") != l.tokenList.end());
    ASSERT_TRUE(token);
}

TEST_F(LexTest,OperatorLessEqual)
{

    l.currentCharIndex = 0;
    l.rawToken = "a<=1";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(),3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "<=") != l.tokenList.end());
    ASSERT_TRUE(token);
}

TEST_F(LexTest,OperatorMoreEqualWithSpace)
{
    l.currentCharIndex = 0;
    l.rawToken = "a >= 1";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(),3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), " ") != l.tokenList.end());
    ASSERT_FALSE(token);
}

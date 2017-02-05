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

    LexTest( ) {
    }

    void SetUp( ) {
        l.currentCharIndex = 0;
    }

    void TearDown( ) {
    }

};

TEST_F(LexTest,Assignment)
{


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
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,LetterDigitAssignment)
{

    l.rawToken = "aaaa34=34654;";
    l.findTokens();


    ASSERT_EQ(l.tokenList.size(),4);
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
    ASSERT_EQ(l.tokenList.size(),3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "<=") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}


TEST_F(LexTest,OperatorMoreEqualWithSpace)
{
    l.rawToken = "a >= 1";
    l.findTokens();
    ASSERT_EQ(l.tokenList.size(),3);
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
    ASSERT_EQ(l.tokenList.size(),8);
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
    ASSERT_EQ(l.tokenList.size(),8);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "/") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "*") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}


TEST_F(LexTest,mainDriver)
{
    using namespace std;
    using namespace boost;

    string line;
    int lineNumCount = 1;
    ifstream myfile ("source_code.txt");
    char_separator<char> sept {" "};
    std::list <std::string> rawTokensFromInputFile;
    std::string srcFileName = "source_code.txt";

    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            tokenizer tokens{line,sept};
            for(const auto &token : tokens)
            {
                Lex lex(token,lineNumCount);
                lex.findTokens();
                lex.printTokenDataStruct();
            }
            lineNumCount++;
        }
    }
    else {
        cout<<"File cannot be opened";
    }
    myfile.close();
}

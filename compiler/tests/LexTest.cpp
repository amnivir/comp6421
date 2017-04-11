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


    void SetUp()
    {

    }

    void TearDown()
    {
    }

};

TEST_F(LexTest,AssignmentInteger)
{
    Lex l("AssignmentInteger");
    l.currentCharIndex = 0;
    l.rawToken = "a=10;";
    l.findTokenTypeAndBuildList();

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

    std::list<TokenDS>::iterator findIter = l.tokenListDS.begin();
    std::advance(findIter,2);
    EXPECT_EQ(findIter->value,"10");
    EXPECT_EQ(findIter->type,INT_VALUE);

}

TEST_F(LexTest,AssignmentFloat)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "a=10.001;";
    l.findTokenTypeAndBuildList();

    std::list<TokenDS>::iterator findIter = l.tokenListDS.begin();
    std::advance(findIter,2);
    EXPECT_EQ(findIter->value,"10.001");
    EXPECT_EQ(findIter->type,FLOAT_VALUE);
}

TEST_F(LexTest,LetterDigitAssignment)
{

    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "aaaa34=34654;";
    l.findTokenTypeAndBuildList();

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
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "a<=1";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "<=") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,OperatorLess)
{

    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "a<1";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "<") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,OperatorGreater)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "a123>b12333";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), ">") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,OperatorMoreEqualWithSpace)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "a >= 1";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 3);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), " ") != l.tokenList.end());
    ASSERT_FALSE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), ">=") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,AddSubOperator)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "a=b+20-10;";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 8);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "+") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "-") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,DivMulOperator)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "a=b*10/20;";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 8);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "/") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "*") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,CurlyBrackets)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "{ ab21==10 }";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 5);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "{") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "}") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,DISABLED_ParenthesisBrackets)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "( ab21 == 10 )";
    l.findTokenTypeAndBuildList();
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
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "if( ab21 <= 10 )";
    l.findTokenTypeAndBuildList();
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
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "int x = 10 ;";
    l.findTokenTypeAndBuildList();
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
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = " int x =1.0 ;";
    l.findTokenTypeAndBuildList();
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
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = " a = .0001 ;";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 4);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), ".0001") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}


TEST_F(LexTest,SqreBracket)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = " int a[100] ;";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 6);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "[") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "]") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,SqreBracketAssignment)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = " a12[100]=b;";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 7);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), "[") != l.tokenList.end());
    ASSERT_TRUE(token);
    token = (std::find(l.tokenList.begin(), l.tokenList.end(), "=") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,Comma)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "int function(a,b)";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 7);
    bool token = (std::find(l.tokenList.begin(), l.tokenList.end(), ",") != l.tokenList.end());
    ASSERT_TRUE(token);
    l.printTokenDataStruct();
}

TEST_F(LexTest,CommentsAfter)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "int x =10;/*asdfasdf*/";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 5);
    l.printTokenDataStruct();
}

TEST_F(LexTest,CommentsBefore)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "/*asdfasdf*/int x =10; ";
    l.findTokenTypeAndBuildList();
    ASSERT_EQ(l.tokenList.size(), 5);
    l.printTokenDataStruct();
}

TEST_F(LexTest,ErrorFloat)
{
    Lex l("AssignmentFloat");
    l.currentCharIndex = 0;
    l.rawToken = "int x =10.a; ";
    l.findTokenTypeAndBuildList();
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
    Lex lexAnalyser("");

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
                lex.findTokenTypeAndBuildList();
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
    std::cout<<" Writing to a file"<<std::endl;
    lexAnalyser.writeTokenDataStructToFile();
    myfile.close();
    ASSERT_EQ(totalTokensActual,totalTokensExpected);
}

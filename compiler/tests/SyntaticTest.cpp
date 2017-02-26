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

class SyntaticTest: public ::testing::Test
{
public:
    Parser p;

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


TEST_F(SyntaticTest,test)
{
p.tableDrivenParserAlgorithm();
ASSERT_EQ(p.productions.size(), 9);
}

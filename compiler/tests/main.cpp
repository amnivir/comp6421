/*
 * main.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: eshinig
 */
#include <LexicalAnalyzer.hh>
#include <Exceptions.hh>
#include <Syntatic.hh>
#include <Semantic.h>
#include<CodeGenerator.h>
#include "gtest/gtest.h"
#include<iostream>
#include<string>
#include<fstream>
#include <regex>
#include <fstream>
#include <algorithm>    // std::all_of

#include "gtest/gtest.h"


int main(int argc, char* argv[])
{
    /*
     * External file is provided ./compiler file input.txt
     */

    std::string fileName;

    if (argc == 3)
    {
        std::cout<<"Compiling the program..\n";
        Lex l(argv[2],"LexicalOutput");
        l.buildTokenDSFromFile();
        l.printTokenDataStruct();
        l.writeTokenDataStructToFile();
        std::list<TokenDS> toklist;// = l.getTokenDSList();//
        Parser p("Output");
//        p.twoPassParser();

        for(auto item : l.tokenListDS)
        {
            TokenDS tokends;
            std::cout<<item.value <<"\t"<<item.type<<"\t"<<item.lineNum<<endl;
            tokends.lineNum=item.lineNum;
            tokends.value=item.value;
            tokends.type=item.type;
            p.tokenListFromLexicalAnalyser.push_back(tokends);
        }
        p.buildInputFromLex();
        p.twoPassParser();
    }

    else if(argc == 2)
    {
        ::testing::InitGoogleTest(&argc,argv);
        RUN_ALL_TESTS();
    }

    return 0;
}

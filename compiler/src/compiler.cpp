//============================================================================
// Name        : compiler.cpp
// Author      : Shivam Nigam
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// simple_example_1.cpp
#include "Lex.hh"

#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>
#include<fstream>
#include<list>

void lex(std::string tok);

void createListOfRawTokensFromFile(std::list<std::string> &rawTokenList, std::string sourceFileName);
int main(){

    std::list<std::string> rawTokenList;
    std::string srcFileName = "source_code.txt";
    Lex l("a=1");
    createListOfRawTokensFromFile(rawTokenList, srcFileName);

    l.findTokens();



    //    for(auto &tokens:rawTokenList)
//    {
//        if((char)tokens[0] == '{')
//         std::cout<<tokens<<std::endl;
//
//    }

    return 0;
}

void lex(std:: string s)
{

}
//TODO convert to char for file name
void createListOfRawTokensFromFile(std::list<std::string>& rawTokenList, std::string sourceFileName)
{
    using namespace std;
    using namespace boost;

    string line;
    ifstream myfile ("source_code.txt");
    char_separator<char> sept {" "};
    std::list <std::string> rawTokensFromInputFile;
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            tokenizer tokens{line,sept};
            for(const auto &token : tokens)
            {
                rawTokenList.push_back(token);
            }
        }

    }
    else {
        cout<<"File cannot be opened";
    }
    cout<<"Tokens Added to the list with size ="<<rawTokenList.size()<<endl;
    myfile.close();
}

/*
 * Syntatic.hh
 *
 *  Created on: Feb 25, 2017
 *      Author: eshinig
 */
#include <iostream>
#include <map>
#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#ifndef SRC_SYNTATIC_HH_
#define SRC_SYNTATIC_HH_

class Parser
{
public:
    Parser();
    void tableDrivenParserAlgorithm();
    bool isTerminal(std::string& x);

    std::list <std::string> derivation;
    std::list <std::string> inverseDerivation; //stack
    std::vector<std::string> terminalSymbols = {"+","*","(",")","0","1","$"};
    std::map  <std::string,int> terminalSymbolsMap =
    {
            {"+", 1},
            {"*", 2},
            {"(", 3},
            {")", 4},
            {"0", 5},
            {"1", 6},
            {"$", 7},

    };
    std::map  <std::string,int> nonTerminalSymbolsMap =
    {
            {"E", 1},
            {"Ep", 2},
            {"T", 3},
            {"Tp", 4},
            {"F", 5},
    };

    int parseTable[6][8] =
    {
          //{0, "+","*","(",")","0","1","$"},
            {0,  0,  0,  0,  0,  0,  0,  0},
            {0, 11, 11,  1, 10,  1,  1, 10},   //E
            {0,  3, 11, 11,  2, 11, 11,  2},  //Ep
            {0, 10, 11,  4, 10,  4,  4, 10},  //T
            {0,  5,  6, 11,  5, 11, 11,  5},  //Tp
            {0, 10, 10,  7, 10,  8,  9, 10}   //F
    };

    std::map<int,std::list<std::string> > productions =
    {
            {1,  {"T" ,"Ep"}},        //E
            {2, {"EPSILON"}},         //Ep
            {3, {"+", "T", "Ep"}},    //Ep
            {4,  {"F", "Tp"}},        //T
            {5, {"EPSILON"}},         //Tp
            {6, {"*", "F", "Tp"}},    //Tp
            {7,  {"(", "E", ")"}},    //F
            {8,  {"0"}},              //F
            {9,  {"1"}},              //F
    };

    //Implemented as Queue
    std::list <std::string> input = {"(","0","+","1",")","*", "0","$"};

private:
    void printDerivation();
    void printInverseDerivation();
};
#endif /* SRC_SYNTATIC_HH_ */

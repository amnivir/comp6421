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
#include <fstream>
#include "LexicalAnalyzer.hh"
#ifndef SRC_SYNTATIC_HH_
#define SRC_SYNTATIC_HH_

struct SyntaticTokenValue
{
    TokenDS tds;
    std::string syntacticValue;
};

class Parser
{
public:
    Parser(std::string fileName);
    Parser(std::list<TokenDS>,std::string fileName);
    std::list<TokenDS> tokenListFromLexicalAnalyser;//TODO make this a pointer
    /*
     * To build symbol table, two pass is required. This method calls tableDrivenParserAlgorithm
     * twice. First time no exceptions are thrown and symbol table is built
     * In second run, Exceptions are thrown if the type is not defined.
     */

    void twoPassParser();

    std::list <std::string> derivation;
    std::list <std::string> stackInverseDerivation; //stack
    /*
     * Checks if the symbol is termical
     */
    bool isTerminal(std::string& x);


    const int ERROR_CODE = 93;// 94 and above is an error

    const int POP_CODE = 94;//if the token is in the FOLLOW set of current non-terminal on the top of the stack

    const int SCAN_CODE = 95;//scan the the token until we get one with we can resume the parse

    std::vector<std::string> terminalSymbols = {"class","id","{","}",";","program","(",")",".","[","]",
            "for","if","then","else","get","put","return","+","-","floatValue","intValue","not","float",
            "int",",","=","<","<=","<>","==",">",">=","or","*","/","and","$"};
    std::map  <std::string,int> terminalSymbolsMap =
    {
            // {0,"class","id","{","}",";","(",")","program","for","+","-","[","]","integer","float","int",",","=","<","<=","<>","==",">",">=","or","*","/","and","$"},
            {"class", 1},
            {"id", 2},
            {"{", 3},
            {"}", 4},
            {";", 5},
            {"program", 6},
            {"(", 7},
            {")", 8},
            {".", 9},
            {"[", 10},
            {"]", 11},
            {"for", 12},
            {"if", 13},
            {"then", 14},
            {"else", 15},
            {"get", 16},
            {"put", 17},
            {"return", 18},
            {"+", 19},
            {"-", 20},
            {"floatValue", 21},
            {"intValue", 22},
            {"not", 23},
            {"float", 24},
            {"int", 25},
            {",", 26},
            {"=", 27},
            {"<", 28},
            {"<=", 29},
            {"<>", 30},
            {"==", 31},
            {">", 32},
            {">=", 33},
            {"or", 34},
            {"*", 35},
            {"/", 36},
            {"and", 37},
            {"$", 38},
    };
    std::map  <std::string,int> nonTerminalSymbolsMap =
    {
            {"prog",1},
            {"classDecl",2},
            {"progBody",3},
            {"funcDef",4},
            {"funcBody",5},
            {"RvarDeclfuncDef",6},
            {"N_varDeclFunctDef",7},
            {"N_indiceIdnest",8},
            {"N_varDeclStatement",9},
            {"N_funcBody",10},
            {"statement",11},
            {"assignmentStatment",12},
            {"statementNoID",13},
            {"assignStat",14},
            {"statBlock",15},
            {"expr",16},
            {"expr_",17},
            {"relExpr",18},
            {"arithExpr",19},
            {"arithExprLR",20},
            {"sign",21},
            {"term",22},
            {"termLR",23},
            {"factor",24},
            {"variable",25},
            {"N_idnest",26},
            {"idnest_",27},
            {"factor_",28},
            {"indice",29},
            {"arraySize",30},
            {"type",31},
            {"FloatOrInt",32},
            {"fParams",33},
            {"aParams",34},
            {"fParamsTail",35},
            {"aParamsTail",36},
            {"N_classDecl",37},
            {"N_funcDef",38},
            {"N_statement",39},
            {"N_arraySize",40},
            {"N_indice",41},
            {"N_fParamsTail",42},
            {"N_aParamsTail",43},
            {"assignOp",44},
            {"relOp",45},
            {"addOp",46},
            {"multiOp",47},
    };





    int parseTable[48][39] =
            //{0,"class","id","{","}",";","program","(",")",".","[","]","for","if","then","else","get","put","return","+","-","floatValue","intValue","not","float","int",",","=","<","<=","<>","==",">",">=","or","*","/","and","$"},
    {
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,1,95,95,95,95,1,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,94},
            {0,2,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,95,95,3,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,94},
            {0,95,4,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,4,4,95,95,95,95,95,95,95,95,95,95,95,95,94},
            {0,95,95,5,95,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,6,95,7,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,6,6,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,94,9,95,8,95,95,9,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,95,95,95,95,95,10,11,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,12,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,13,95,94,95,95,95,95,14,14,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,14,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,16,95,18,95,95,95,95,95,95,95,17,17,95,95,17,17,17,95,95,95,95,95,15,15,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,20,95,94,94,95,95,95,95,95,95,19,19,95,94,19,19,19,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,21,95,94,94,95,95,95,95,95,95,94,94,95,94,94,94,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,94,95,94,94,95,95,95,95,95,95,22,23,95,94,24,25,26,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,27,95,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,29,28,95,30,95,95,95,95,95,95,29,29,95,30,29,29,29,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,31,95,95,94,95,31,94,95,95,95,95,95,95,95,95,95,95,31,31,31,31,31,95,95,94,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,95,33,95,95,33,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,33,95,32,32,32,32,32,32,95,95,95,95,95},
            {0,95,34,95,95,94,95,34,95,95,95,95,95,95,95,95,95,95,95,34,34,34,34,34,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,35,95,95,94,95,35,94,95,95,94,95,95,95,95,95,95,95,35,35,35,35,35,95,95,94,95,94,94,94,94,94,94,95,95,95,95,95},
            {0,95,95,95,95,37,95,95,37,95,95,37,95,95,95,95,95,95,95,36,36,95,95,95,95,95,37,95,37,37,37,37,37,37,36,95,95,95,95},
            {0,95,94,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,38,39,94,94,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,40,95,95,94,95,40,94,95,95,94,95,95,95,95,95,95,95,40,40,40,40,40,95,95,94,95,94,94,94,94,94,94,94,95,95,95,95},
            {0,95,95,95,95,42,95,95,42,95,95,42,95,95,95,95,95,95,95,42,42,95,95,95,95,95,42,95,42,42,42,42,42,42,42,41,41,41,95},
            {0,95,47,95,95,94,95,43,94,95,95,94,95,95,95,95,95,95,95,48,48,44,45,46,95,95,94,95,94,94,94,94,94,94,94,94,94,94,95},
            {0,95,49,95,95,94,95,94,94,95,95,94,95,95,95,95,95,95,95,94,94,95,95,95,95,95,94,94,94,94,94,94,94,94,94,94,94,94,95},
            {0,95,95,95,95,51,95,51,51,50,95,51,95,95,95,95,95,95,95,51,51,95,95,95,95,95,51,51,51,51,51,51,51,51,51,51,51,51,95},
            {0,95,95,95,95,94,95,94,94,52,95,94,95,95,95,95,95,95,95,94,94,95,95,95,95,95,94,94,94,94,94,94,94,94,94,94,94,94,95},
            {0,95,95,95,95,54,95,53,54,95,95,54,95,95,95,95,95,95,95,54,54,95,95,95,95,95,54,95,54,54,54,54,54,54,54,54,54,54,95},
            {0,95,95,95,95,94,95,94,94,94,55,94,95,95,95,95,95,95,95,94,94,95,95,95,95,95,94,94,94,94,94,94,94,94,94,94,94,94,95},
            {0,95,95,95,95,94,95,95,94,95,56,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,57,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,58,58,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,59,60,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,61,95,95,95,95,95,62,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,61,61,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,63,95,95,95,95,63,64,95,95,95,95,95,95,95,95,95,95,63,63,63,63,63,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,65,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,66,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,67,95,95,95,95,68,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,69,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,69,69,95,95,95,95,95,95,95,95,95,95,95,95,70},
            {0,95,71,95,72,95,95,95,95,95,95,95,71,71,95,95,71,71,71,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,95,74,95,95,74,95,73,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,74,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,95,76,95,76,76,76,75,76,95,95,95,95,95,95,95,76,76,95,95,95,95,95,76,76,76,76,76,76,76,76,76,76,76,76,95},
            {0,95,95,95,95,95,95,95,78,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,77,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,95,95,95,95,95,95,80,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,79,95,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,94,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,94,94,94,94,94,95,95,95,81,95,95,95,95,95,95,95,95,95,95,95},
            {0,95,94,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,94,94,94,94,94,95,95,95,95,82,83,84,85,86,87,95,95,95,95,95},
            {0,95,94,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,88,89,94,94,94,95,95,95,95,95,95,95,95,95,95,90,95,95,95,95},
            {0,95,94,95,95,95,95,94,95,95,95,95,95,95,95,95,95,95,95,94,94,94,94,94,95,95,95,95,95,95,95,95,95,95,95,91,92,93,95}
    };


    std::map<int,std::list<std::string> > productions =
    {
            {1,  {"CREATE_GLOBAL_TABLE","N_classDecl","progBody"}},        //prog → N_classDecl
            {2,  {"class", "id","CREATE_CLASS_ENTRY_TABLE","{" ,"RvarDeclfuncDef", "}" ,"END_FUNCTION",";"}},        //T
            {3, {"program","CREATE_PROGRAM_ENTRY", "funcBody", ";", "N_funcDef"}},    //Tp
            //Function declaration ex. int func1 (int x[10], float x[10][20])
            {4,  {"type","id","COPY_ID","CREATE_FUNCTION_ENTRY","(", "fParams",")","CREATE_PARAMETER_DIMENSIONS","funcBody",";"}},
            {5, { "{", "N_funcBody", "}","END_FUNCTION"}},         //Ep
            {6,  {"type", "id","COPY_ID", "N_varDeclFunctDef"}},    //F
            {7, {"EPSILON"}},    //Ep
            {8, {"CREATE_FUNCTION_ENTRY","(","fParams",")","CREATE_PARAMETER_DIMENSIONS","funcBody",";","RvarDeclfuncDef"}},         //Ep
            {9, {"N_arraySize","WRITE_VARIABLE_DIMENSION",";","CREATE_VARIABLE_DIMENSIONS","RvarDeclfuncDef"}},         //Tp
            {10, {".","id","N_indiceIdnest"}},    //Tp
            {11,  {"[","arithExpr","]","N_indiceIdnest"}},              //F
            {12,  {"EPSILON"}},        //T
            {13,  {"id","COPY_ID","N_arraySize","WRITE_VARIABLE_DIMENSION",";","CREATE_VARIABLE_DIMENSIONS","N_funcBody"}},              //F
            {14,  {"N_indiceIdnest", "assignOp", "COPY_ASSIGNMENT","expr", ";" ,"TYPE_CHECK", "N_statement"}},        //E
            {15, {"FloatOrInt", "id", "COPY_ID","N_arraySize","WRITE_VARIABLE_DIMENSION", ";", "CREATE_VARIABLE_DIMENSIONS","N_funcBody"}},    //Ep
            {16,  {"id","COPY_TYPE", "N_varDeclStatement"}},        //T
            {17,  {"statementNoID", "N_statement"}},              //F
            {18, {"EPSILON"}},         //Tp
            {19,  {"statementNoID"}},              //F
            {20,  {"assignmentStatment"}},
            {21,  {"variable", "assignOp", "COPY_ASSIGNMENT","expr", ";" ,"TYPE_CHECK"}},              //F
            {22,  {"for", "(", "type", "id","COPY_ID", "assignOp","COPY_ASSIGNMENT", "expr", ";", "TYPE_CHECK","relExpr", ";",
                    "assignStat", ")", "statBlock", ";"}},        //E
                    {23, {"if", "(", "expr", ")", "then", "statBlock", "else", "statBlock", ";"}},         //Ep
                    {24, {"get", "(", "variable", ")","CODE_GET",";"}},         //Ep
                    {25,  {"put", "(", "expr", ")" ,"CODE_PUT",";"}},        //T
                    {26,  {"return", "(", "expr", ")" ,";"}},        //T
                    {27,  {"variable", "assignOp" ,"expr","TYPE_CHECK"}},        //T//T
                    {28,  {"{","N_statement","}"}},        //T
                    {29,  {"statement"}},    //Tp
                    {30,  {"EPSILON"}},              //F
                    {31,  {"arithExpr", "EXPR","expr_"}},    //F
                    {32,  {"relOp", "arithExpr"}},    //F
                    {33,  {"EPSILON"}},
                    {34,  {"arithExpr", "relOp", "arithExpr"}},              //F
                    {35,  {"term", "ARITHEXPRLR","arithExprLR"}},        //E
                    {36,  {"addOp", "term","ADD_SUB_IDS" ,"arithExprLR"}},//Ep
                    {37,  {"EPSILON"}},              //F
                    {38, {"+"}},    //Ep
                    {39,  {"-"}},        //T
                    {40, {"factor", "TERM", "termLR"}},         //Tp
                    {41, {"multiOp", "factor","MUL_DIV_IDS", "termLR"}},    //Tp
                    {42,  {"TERMLR","EPSILON"}},
                    {43,  {"(", "arithExpr", ")"}},    //F
                    {44,  {"floatValue","FACTOR_VALUE"}},
                    {45,  {"intValue","FACTOR_VALUE"}},
                    {46,  {"not", "factor"}},//F
                    {47,  {"variable", "factor_"}},//F
                    {48,  {"sign", "factor"}},//F
                    {49, {"id","COPY_ID","N_indice","N_idnest"}},         //Ep
                    {50,  {"idnest_","N_idnest" }},        //E
                    {51,  {"EPSILON"}},        //T
                    {52,  {".","id","COPY_ID","MEMBER_CHECK","N_indice"}},        //T
                    {53,  {"(","TYPE_CHECK","aParams",")"}},
                    {54,  {"FACTOR_","EPSILON"}},        //T//E
                    {55, { "[", "arithExpr", "]"}},         //Tp
                    {56, {"[", "intValue","COPY_ARRAY_SIZE" ,"]"}},    //Tp
                    {57,  {"id","COPY_TYPE"}},              //F
                    {58,  {"FloatOrInt"}},              //F
                    {59,  {"float","COPY_TYPE"}},    //F
                    {60,  {"int","COPY_TYPE"}},              //
                    {61,  {"type", "id", "COPY_ID","N_arraySize","WRITE_PARAMETER_DIMENSION", "N_fParamsTail"}},        //EF
                    {62,  {"EPSILON"}},        //T
                    {63,  {"expr" ,"N_aParamsTail"}},        //E
                    {64, {"EPSILON"}},         //Ep
                    {65,  {",", "type", "id", "COPY_ID","N_arraySize","WRITE_PARAMETER_DIMENSION"}},    //Ep
                    {66, {"," "expr"}},    //Ep
                    {67,  {"classDecl", "N_classDecl"}},
                    {68,  {"EPSILON"}},        //T
                    {69,  {"funcDef", "N_funcDef"}},    //F
                    {70,  {"EPSILON"}},
                    {71,  {"statement", "N_statement"}},        //T
                    {72,  {"EPSILON"}},
                    {73,  {"arraySize", "N_arraySize"}},    //F
                    {74,  {"EPSILON"}},
                    {75,  {"indice","N_indice" }},
                    {76,  {"EPSILON"}},
                    {77,  {"fParamsTail", "N_fParamsTail"}},        //EF
                    {78,  {"EPSILON"}},
                    {79, {"aParamsTail", "N_aParamsTail"}},    //Ep
                    {80,  {"EPSILON"}},
                    {81,  {"="}},    //Tp
                    {82,  {"<"}},    //F
                    {83,  {"<="}},              //F
                    {84,  {"<>"}},              //F
                    {85,  {"=="}},        //E
                    {86, {">"}},         //Ep
                    {87, {">="}},    //Ep
                    {88,  {"+","COPY_OPERATOR"}},        //T
                    {89, {"-","COPY_OPERATOR"}},         //Tp
                    {90, {"or"}},    //Tp
                    {91,  {"*","COPY_OPERATOR"}},    //F
                    {92,  {"/","COPY_OPERATOR"}},              //F
                    {93,  {"and"}},              //F
    };

    std::list <SyntaticTokenValue> inputSemanticValue;
    std::list <SyntaticTokenValue> inputSemanticValueCopy;
    void buildInputFromLex();
    /*
     * true if the parsing was sucessful, i.e. no errors occured otherwise false.
     */
    bool parsing = true;

private:
    void printDerivation();
    void printInverseDerivation();
    /*
     * copies values from inputSemanticValueCopy to inputSemanticValue
     */
    void copySyntaticTokenValueList();
    void parseTerminalSymbol(const std::string& nonTerminal, std::string& token);
    /*
     * Reads next token from Lexical analyzer list
     */

    /*
     * This algorithm is based on Table driven design parser
     */
    void tableDrivenParserAlgorithm(bool secondPass);

    /*
     * Error recovery when rule is either pop or scan code
     */
    void errorRecovery(int code, std::string &token,std::string &previousToken,int lineNum);

    std::string fileName;

};
#endif /* SRC_SYNTATIC_HH_ */

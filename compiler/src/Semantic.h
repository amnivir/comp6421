/*
 * Semantic.h
 *
 *  Created on: Mar 22, 2017
 *      Author: eshinig
 */
#include <vector>
#include <string>
#include <map>
#include <list>
#include <Syntatic.hh>
#ifndef SRC_SEMANTIC_H_
#define SRC_SEMANTIC_H_

using namespace std;

struct SymbolInfo
{
//    std::string name; // x,program,MyClass
    std::string kind;// function, class,parameter,variable
    std::string type;//int,float,class
    std::string link="NONE"; //TODO create a stack of table;
    //std::map<std::string, SymbolTabel>* link = nullptr; // link to a new symboltable
};

class Semantic
{
public:
    Semantic();
    static void performAction(const std::string& symbolFromStack, const SyntaticTokenValue&);

    static const std::vector<std::string> semanticActions ;
    static bool isSemanticAction(const std::string& symbolFromStack);


    /**
     * This is stack of the tables created during the creation of Symbol table
     */
    static std::vector<std::string> currentTable;

    /**
     * This is the data structure for the symbol table
     * <  "TableName", < <"Symbol">, <Symbol Type Kind>   >>
     */
    static std::map < std::string, std::map<std::string, SymbolInfo>  > symbolTables ;

    /*
     * non terminal symbol values, to be used in Semantic analysis
     */
    static std::map <std::string,std::string> nonTerminalSymValue;

    /*
     * This is a semantic stack that where attributes are gathered and propagated across tree node
     * Since this is a map the insertion order is not maintained so Vector is used in conjunction
     */
    //static std::map<std::string,std::string> semanticStack;
    //static std::vector<std::string,std::string> sdemanticStack;
    static std::vector < std::pair<std::string, std::string> > semanticStack;
    //v.push_back(std::pair<std::string,std::string>("y","2"));

    /*
     * Print all the symbol tables
     */
    static void printSymbolTable();

    static void printSemanticStack();

    virtual ~Semantic();
};

#endif /* SRC_SEMANTIC_H_ */

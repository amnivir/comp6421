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
    int lineNum;
    bool initialized = false;
    //std::map<std::string, SymbolTabel>* link = nullptr; // link to a new symboltable
};

class Semantic
{
public:
    /*
     * returns true if symbol exist in the symbol table
     */
    static bool doesSymbolExist(const std::string& currentTableName, std::string& symbolName);

    /*
     * returns true if class type or function exist in the symbol table
     */
    static bool doesTypeOrFunctionExist(std::string& id_type);

    static void performAction(const std::string& symbolFromStack, const SyntaticTokenValue&, bool secondPass);

    static const std::vector<std::string> semanticActions ;
    static bool isSemanticAction(const std::string& symbolFromStack);

    /*
     * check types during assignment operation through symbol table, i.e if A = B, then check type of A is equal to B
     */
    static bool isTypesEqual(const std::string left, const std::string right);
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
     * This is a semantic stack that where attributes are gathered and propagated across tree node
     * Since this is a map the insertion order is not maintained so Vector is used in conjunction
     * This stack is used for building symbol table. TODO merge this with semanticStackExpr
     */
    static std::vector < std::pair<std::string, std::string> > semanticStack;
    //v.push_back(std::pair<std::string,std::string>("y","2"));

    static std::vector < std::pair<std::string, SyntaticTokenValue> > semanticStackExpr;

    /*
     * Copy operator : + , - , * , /
     */
    static std::string m_operator;

    /*
     * Print all the symbol tables
     */
    static void printSymbolTable();

    static void printSemanticStack();

    virtual ~Semantic();
};

#endif /* SRC_SEMANTIC_H_ */

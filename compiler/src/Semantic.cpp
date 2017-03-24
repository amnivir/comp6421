/*
 * Semantic.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: eshinig
 */

#include <Semantic.h>
#include <iomanip>
#include <iostream>

/**
 * Definition of static variables
 */
std::vector<std::string> Semantic::currentTable = {};
std::map < std::string, std::map<std::string, SymbolInfo>  > Semantic::symbolTables = {};
std::map <std::string,std::string> Semantic::nonTerminalSymValue = {};

Semantic::Semantic()
{
}
const std::vector<std::string>  Semantic::semanticActions = {
            "CREATE_GLOBAL_TABLE",
            "CREATE_CLASS_ENTRY_TABLE",
            "CREATE_PROGRAM_ENTRY",
            "CREATE_FUNCTION_ENTRY",
            "CREATE_VARIABLE_ENTRY",
            "COPY_TYPE", // Funtion type or variable type, int or float
            "COPY_ARRAY_SIZE",
            "COPY_ID",
            "CREATE_PARAMETER_ENTRY",
            "WRITE_PARAMETER_DIMENSION"
        };

bool Semantic::isSemanticAction(const std::string& symbolFromStack)
{
        for (auto symbol : semanticActions)
        {
            if(symbol == symbolFromStack)
            {
                return true;
            }
        }
        return false;
 }

void Semantic::performAction(const std::string& symbolFromStack, const SyntaticTokenValue& stv)
{
    SymbolInfo symInfo;
    std::string name; //name of class ,func ,x,a
    std::string kind; //class , function, program
    if(symbolFromStack == "CREATE_GLOBAL_TABLE")
    {
        std::cout<<"GLOBAL TABLE CREATED\n";
        currentTable.push_back("global");
    }

    else if(symbolFromStack == "CREATE_CLASS_ENTRY_TABLE")
    {
        name = stv.tds.value;
        symInfo.kind = "class";
        symInfo.type = "";
        symbolTables["global"][name] = symInfo;
    }

    else if(symbolFromStack == "CREATE_PROGRAM_ENTRY")
    {
        name = "program";
        symInfo.kind = "function";
        symInfo.type = "";
        Semantic::symbolTables["global"][name] = symInfo;
    }

    else if(symbolFromStack == "COPY_TYPE")
    {
        std::cout<<"TYPE="<<stv.syntacticValue<<"  "<<stv.tds.value<<std::endl;
        nonTerminalSymValue["type"] = stv.syntacticValue;
    }

    /*
     * THis rule will have return types
     * once the entry to table is done, new table is created
     *
     */
    else if(symbolFromStack == "CREATE_FUNCTION_ENTRY")
    {
        name = stv.tds.value;
        symInfo.kind = "function";
        symInfo.type = nonTerminalSymValue["FloatOrInt"];
        symInfo.link = name;
        Semantic::symbolTables["global"][name] = symInfo;
        std::cout<<"CREATE_FUNCTION_ENTRY"<<nonTerminalSymValue.find("FloatOrInt")->second<<"\n";
        //now the entries are going to be in new function table
        currentTable.push_back(name);
    }

    else if(symbolFromStack == "COPY_ARRAY_SIZE")
    {
        name = stv.tds.value;
        std::cout<<"COPY_ARRAY_SIZE="<<name;
        nonTerminalSymValue["arraySize"] = nonTerminalSymValue["arraySize"] +
                "[" + name + "]";
        std::cout<<"ARRAT_SIZE"<<nonTerminalSymValue["arraySize"]<<std::endl;
    }

    else if(symbolFromStack == "COPY_ID")//creates new table with parameter
    {
        //name = stv.tds.value;
        std::cout<<"COPY_ID\n";
        name = stv.tds.value;//id
        nonTerminalSymValue["id_value"]=name;

    }

    else if(symbolFromStack == "WRITE_PARAMETER_DIMENSION")
    {
        std::cout<<"WRITE_PARAMETER_DIMENSION"<<std::endl;
        symInfo.kind = "parameter";
        symInfo.type = nonTerminalSymValue["type"].append(nonTerminalSymValue["arraySize"]);
        Semantic::symbolTables[currentTable.back()][nonTerminalSymValue["id_value"]] = symInfo;
        nonTerminalSymValue.clear();

    }
}
Semantic::~Semantic()
{
    // TODO Auto-generated destructor stub
}

void Semantic::printSymbolTable(const std::string& tableName)
{
    for (std::map < std::string, std::map<std::string, SymbolInfo>  >::iterator iter = symbolTables.begin();
            iter != symbolTables.end();
            ++iter)
    {
        std::cout<<"+--------------------------------"<<iter->first<<"--------------------------------------------+"<<std::endl;
        std::cout<<std::setw(20)<<"Name"<<std::setw(20)<<"Kind"<<std::setw(20)<<"type"<<std::setw(20)<<"link"<<std::endl;
        for (auto symRecord : symbolTables.find(iter->first)->second)
        {
            std::cout<<std::setw(20)<<symRecord.first<<std::setw(20)<<symRecord.second.kind<<std::setw(20)
            <<symRecord.second.type<<std::setw(20)<<symRecord.second.link<<std::endl;
        }
        std::cout<<"+-----------------------------------------------------------------------------------+"<<std::endl;
    }
}

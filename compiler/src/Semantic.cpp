/*
 * Semantic.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: eshinig
 */

#include <Semantic.h>
#include <iomanip>
#include <iostream>
#include <algorithm>

/**
 * Definition of static variables
 */
std::vector<std::string> Semantic::currentTable = {};
std::map < std::string, std::map<std::string, SymbolInfo>  > Semantic::symbolTables = {};
std::map <std::string,std::string> Semantic::nonTerminalSymValue = {};
std::vector < std::pair<std::string, std::string> > Semantic::semanticStack = {};

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
        "CREATE_PARAMETER_DIMENSIONS",
        "WRITE_PARAMETER_DIMENSION", // Function paramaters i.e float randomoze( int x [10] ){}
        "WRITE_VARIABLE_DIMENSION", // Function variable i.e. float randomize() {int x[10]}
        "CREATE_VARIABLE_DIMENSIONS",
        "END_FUNCTION"
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
    SymbolInfo emptySymInfo;
    emptySymInfo.kind = "";
    emptySymInfo.link = "" ;
    emptySymInfo.type = "" ;
    std::string name; //name of class ,func ,x,a
    std::string kind; //class , function, program3
    std::string parameter;
    std::string id_value;
    std::string id_type;
    std::string id_arraysize = "";
    std::string EMPTY="";

    if(symbolFromStack == "CREATE_GLOBAL_TABLE")
    {
        std::cout<<"GLOBAL TABLE CREATED\n";
        currentTable.clear();
        currentTable.push_back("global");
    }

    /*
     * writes class name in Global Symbol table
     */
    else if(symbolFromStack == "CREATE_CLASS_ENTRY_TABLE")
    {
        name = stv.tds.value;
        symInfo.kind = "class";
        symInfo.type = "";
        symInfo.link = name;
        //Enter the class name in global table
        symbolTables["global"][name] = symInfo;
        //create the empty class table
        symbolTables[name][EMPTY] = emptySymInfo;
        //GO into table
        currentTable.push_back(name);
    }


    /*
     * writes 'program' in Global Symbol table
     */

    else if(symbolFromStack == "CREATE_PROGRAM_ENTRY")
    {
        name = "program";
        symInfo.kind = "function";
        symInfo.type = "";
        symInfo.link = name;
        Semantic::symbolTables["global"][name] = symInfo;
        //create the empty class table
        symbolTables[name][EMPTY] = emptySymInfo;
        //GO into program table
        currentTable.push_back(name);
    }

    /*
     * THis rule will have return types
     * once the entry to table is done, new table is created
     *
     */
    else if(symbolFromStack == "CREATE_FUNCTION_ENTRY")
    {
        std::string scopedFunctionName ="";
        name = stv.tds.value;
        symInfo.kind = "function";
        //Function type
        //semantic stack contains the type of the function name so pop it as it is already written
        Semantic::semanticStack.pop_back();
        symInfo.type = Semantic::semanticStack.back().second;
        //Semantic::semanticStack.pop_back();
        //Name of the function

        scopedFunctionName = name;

        if(currentTable.size() >= 2)
        {
            scopedFunctionName = currentTable.back() + ":" + name;
        }

        symInfo.link = scopedFunctionName;
        Semantic::symbolTables[currentTable.back()][name] = symInfo;
        //create the empty class table
        symbolTables[scopedFunctionName][EMPTY] = emptySymInfo;
        currentTable.push_back(scopedFunctionName);


    }

    else if(symbolFromStack == "COPY_ARRAY_SIZE")
    {
        std::string tmp;
        name = stv.tds.value;
        tmp = name;
        std::cout<<"COPY_ARRAY_SIZE="<<name;
        nonTerminalSymValue["arraySize"] = nonTerminalSymValue["arraySize"] +
                "[" + name + "]";
        std::cout<<"ARRAY_SIZE"<<nonTerminalSymValue["arraySize"]<<std::endl;

        if(Semantic::semanticStack.front().first == "arraySize")
        {
            tmp = Semantic::semanticStack.front().second;
            Semantic::semanticStack.pop_back();
            tmp = tmp +" "+ name;
        }
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("arraySize",tmp));
    }

    else if(symbolFromStack == "COPY_TYPE")
    {
        std::cout<<"TYPE="<<stv.syntacticValue<<"  "<<stv.tds.value<<std::endl;
        nonTerminalSymValue["type"] = stv.syntacticValue;
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("type",stv.tds.value));
    }

    else if(symbolFromStack == "COPY_ID")//creates new table with parameter
    {
        //name = stv.tds.value;
        std::cout<<"COPY_ID\n";
        name = stv.tds.value;//id
        nonTerminalSymValue["id_value"]=name;
        /*
         * type--> FloatOrInt
         * FloatOrInt--> Float / Int, so bypass FloatOrInt i.e. type -->Float/Int
         */
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("id",name));
    }

    else if(symbolFromStack == "WRITE_PARAMETER_DIMENSION")
    {
        std::cout<<"WRITE_PARAMETER_DIMENSION"<<std::endl;
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("parameter",name));
        nonTerminalSymValue.clear();
    }
    else if(symbolFromStack == "WRITE_VARIABLE_DIMENSION")
    {
        std::cout<<"WRITE_VARIABLE_DIMENSION"<<std::endl;
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("variable",name));
    }

    else if(symbolFromStack == "CREATE_PARAMETER_DIMENSIONS")
    {
        if(Semantic::semanticStack.size() == 0)
            return;
        /*
         * parameter
         * id  v2
         * type  float
         * parameter
         * arraySize  10
         * id  v1
         * type  int
         *
         */
        while(Semantic::semanticStack.back().first == "parameter")
        {
            Semantic::semanticStack.pop_back(); //pop "parameter"
            id_arraysize = "";
            while(Semantic::semanticStack.back().first == "arraySize")
            {
                // int id [intValue1][intValue2]
                id_arraysize = "[" + Semantic::semanticStack.back().second +"]"+ id_arraysize ;
                Semantic::semanticStack.pop_back(); //pop "arraysize"
            }
            if(Semantic::semanticStack.back().first == "id") //TODO  use map instead
            {
                id_value = Semantic::semanticStack.back().second;
                Semantic::semanticStack.pop_back(); //pop "id"
            }
            if(Semantic::semanticStack.back().first == "type")
            {
                id_type = Semantic::semanticStack.back().second;
                Semantic::semanticStack.pop_back(); //pop "id"
            }

            symInfo.kind = "parameter";
            symInfo.type = id_type.append(id_arraysize);
            Semantic::symbolTables[currentTable.back()][id_value] = symInfo;

            if(Semantic::semanticStack.size() == 0)
                break;
        }

        std::cout<<"CREATE_PARAMETER_DIMENSIONS \n Printing Stack inside the function\n";
        Semantic::printSemanticStack();
    }

    else if(symbolFromStack == "CREATE_VARIABLE_DIMENSIONS")
    {
        if(Semantic::semanticStack.size() == 0)
            return;
        /*
         * id  v1
         * type  int
         *
         */
        while(Semantic::semanticStack.back().first == "variable")
        {
            Semantic::semanticStack.pop_back(); //pop "parameter"

            while(Semantic::semanticStack.back().first == "arraySize")
            {
                // int id [intValue1][intValue2]
                id_arraysize = "[" + Semantic::semanticStack.back().second +"]"+ id_arraysize ;
                Semantic::semanticStack.pop_back(); //pop "arraysize"
            }
            if(Semantic::semanticStack.back().first == "id") //TODO  use map instead
            {
                id_value = Semantic::semanticStack.back().second;
                Semantic::semanticStack.pop_back(); //pop "id"
            }
            if(Semantic::semanticStack.back().first == "type")
            {
                id_type = Semantic::semanticStack.back().second;
                Semantic::semanticStack.pop_back(); //pop "id"
            }

            symInfo.kind = "variable";
            symInfo.type = id_type.append(id_arraysize);
            Semantic::symbolTables[currentTable.back()][id_value] = symInfo;

            if(Semantic::semanticStack.size() == 0)
                break;
        }

        std::cout<<"CREATE_VARIABLE_DIMENSIONS \n Printing Stack inside the function\n";
        Semantic::printSemanticStack();
    }

    /*
     * THis function is called when the funciton is ended i.e '}' is found after N_funcBody in production 5
     */
    else if(symbolFromStack == "END_FUNCTION")
    {
        if(currentTable.size() > 0)
        {
            cout<<"Function Ended="<<currentTable.back()<<std::endl;
            currentTable.pop_back();
        }
    }
}
Semantic::~Semantic()
{
}

void Semantic::printSymbolTable()
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

void Semantic::printSemanticStack()
{
    for(std::vector<std::pair<std::string, std::string>>::reverse_iterator it = semanticStack.rbegin(); it != semanticStack.rend(); ++it)
    {
        std::cout << (*it).first << "  " <<(*it).second<<"\n";
    }
}

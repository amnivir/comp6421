/*
 * Semantic.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: eshinig
 */
#include <CodeGenerator.h>
#include <Exceptions.hh> // for Semantic exceptions
#include <Semantic.h>
#include <iomanip>
#include <iostream>
#include <algorithm>


/**
 * Definition of static variables
 */
std::vector<std::string> Semantic::currentTable = {};
std::map < std::string, std::map<std::string, SymbolInfo>  > Semantic::symbolTables = {};
std::vector < std::pair<std::string, std::string> > Semantic::semanticStack = {};
std::string Semantic::m_operator = "";
std::string Semantic::outputFileName="";


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
        "END_FUNCTION",
        "COPY_ASSIGNMENT",// '='
        "TYPE_CHECK", // performs type checking through symbol table, only valid in sexond pass
        "ADD_SUB_IDS",
        "MUL_DIV_IDS",
        "FACTOR_",
        "FACTOR_VALUE",
        "TERM",
        "TERMLR",
        "ARITHEXPRLR",
        "EXPR",
        "COPY_OPERATOR",
        "CODE_GET",
        "CODE_PUT"
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


void Semantic::performAction(const std::string& symbolFromStack, const SyntaticTokenValue& stv, bool secondPass)
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
        //std::cout<<"GLOBAL TABLE CREATED\n";
        currentTable.clear();
        currentTable.push_back("global");
        if(secondPass)
        {
            CodeGenerator::createSrcCodeFile("ReadWrite");
            CodeGenerator::addDirectives();
        }
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
        symInfo.lineNum = stv.tds.lineNum;
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
        symInfo.lineNum = stv.tds.lineNum;
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
        symInfo.lineNum = stv.tds.lineNum;
        //Function type
        //semantic stack contains the type of the function name so pop it as it is already written
        Semantic::semanticStack.pop_back();
        symInfo.type = Semantic::semanticStack.back().second;
        //Semantic::semanticStack.pop_back();
        //Name of the function

        scopedFunctionName = name;

        if(!secondPass)
        {
            if(doesSymbolExist(currentTable.back(),scopedFunctionName))
            {
                throw SemanticException("Function already exist:"+scopedFunctionName);
            }
        }
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
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("type",stv.tds.value));
    }

    else if(symbolFromStack == "COPY_ID")//creates new table with parameter
    {
        name = stv.tds.value;//id
        /*
         * type--> FloatOrInt
         * FloatOrInt--> Float / Int, so bypass FloatOrInt i.e. type -->Float/Int
         */
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("id",name));
        //std::cout<<"Variable = id\n";
    }

    else if(symbolFromStack == "WRITE_PARAMETER_DIMENSION")
    {
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("parameter",name));
    }

    else if(symbolFromStack == "WRITE_VARIABLE_DIMENSION")
    {
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("variable",name));
        if(secondPass)
        {
            /*
             * create memory space when initialized in main program
             */
            if(currentTable.back() == "program")
                CodeGenerator::addDBDirective();
        }
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

            /*
             * check if non-primitive type is defined
             */
            if(secondPass)
            {
                //primitive types are already defined
                if(id_type!="int" && id_type!="float")
                {
                    if(!doesTypeOrFunctionExist(id_type))
                    {
                        throw SemanticException(+"Type not defined: "+id_type);
                    }
                }
            }
            symInfo.kind = "parameter";
            symInfo.type = id_type.append(id_arraysize);
            Semantic::symbolTables[currentTable.back()][id_value] = symInfo;

            if(Semantic::semanticStack.size() == 0)
                break;
        }

    }

    /*
     * this statement creates the variable symbol in the scoped function
     */
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
            Semantic::semanticStack.pop_back(); //pop "variable"

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

            /*
             * check if non-primitive type is defined
             */
            if(secondPass)
            {
                //primitive types are already defined
                if(id_type!="int" && id_type!="float")
                {
                    if(!doesTypeOrFunctionExist(id_type))
                    {
                        throw SemanticException(+"Type not defined: "+id_type);//TODO make a list
                    }
                }
            }

            symInfo.kind = "variable";
            symInfo.type = id_type.append(id_arraysize);
            //if symbol does not exist then write to symbol table
            if(!Semantic::doesSymbolExist(currentTable.back(),id_value))
            {
                Semantic::symbolTables[currentTable.back()][id_value] = symInfo;
            }
            /*
             * in second pass do not execute this function as it has been already checked in previous run
             */
            else if(!secondPass)
            {
                std::cerr<<"Multiple Definition of Symbol:"<<id_value<<"\n";
                throw SemanticException("Multiple Definition of Symbol: "+id_value);
            }

            if(Semantic::semanticStack.size() == 0)
                break;
        }
    }

    /*
     * THis function is called when the funciton is ended i.e '}' is found after N_funcBody in production 5
     */
    else if(symbolFromStack == "END_FUNCTION")
    {
        if(currentTable.size() > 0)
        {
            //cout<<"Function Ended="<<currentTable.back()<<std::endl;
            currentTable.pop_back();
        }
    }

    else if(symbolFromStack == "COPY_ASSIGNMENT")
    {
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("assignment","="));
    }

    else if(symbolFromStack == "TYPE_CHECK" && secondPass)
    {
        //std::cout<<"PerformTypeChecking";
        if(Semantic::semanticStack.size() == 3)
        {
            std::string right = Semantic::semanticStack.back().second;
            std::string rightFirst = Semantic::semanticStack.back().first;
            Semantic::semanticStack.pop_back(); //pop "rvalue"
            Semantic::semanticStack.pop_back(); //pop "="
            std::string left = Semantic::semanticStack.back().second;
            Semantic::semanticStack.pop_back(); //pop "lvalue"


            if( !Semantic::isTypesEqual(left,right) )
                throw SemanticException("Types are not equal:" + left + " has different type then" + right );


            if(rightFirst == "arithExprLR" || rightFirst == "EXPR")
            {
                CodeGenerator::generateCodeAssignment(left,right);
                return;
            }
            /*
             * if right contains as expression, then type checking is not required
             * as it is already checked in line 388, symbolFromStack == "ADD_SUB_IDS"
             */


        }
        //TODO: this needs to be defined what could less than 3
        else if (Semantic::semanticStack.size() < 3)
            return;
        //        else
        //            throw SemanticException("In Type Checking, the format is not 'A = B' statement has more than "
        //                    "3 tokens: "+semanticStack.size());
    }

    else if(symbolFromStack == "ADD_SUB_IDS")
    {
        std::string id_1;
        std::string id_2;
        std::string idAfterOperator;
        std::string result;
        //std::cout<<"ADD_SUB_IDS\n";
        id_1 = Semantic::semanticStack.back().second;
        Semantic::semanticStack.pop_back();
        id_2 = Semantic::semanticStack.back().second;
        Semantic::semanticStack.pop_back();
        result = id_2 + m_operator + id_1;
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("arithExprLR",result));

        //TYPE checking: rvalue.type + rv
        //id_2 is an expression, so fetch the last id
        std::size_t found = id_2.find_last_of("/*-+");
        idAfterOperator = id_2.substr(found+1) ;


        /*
         * don't check type in second pass
         */
        if(secondPass)
        {
            Semantic::semanticStack.pop_back(); // replace x = y +z to x = t
            result = CodeGenerator::generateCodeArithmetic(id_1,id_2,m_operator);
            Semantic::semanticStack.push_back(std::pair<std::string,std::string>("arithExprLR",result));
            return;
        }

        if(!Semantic::isTypesEqual(id_1,idAfterOperator))
        {
            throw SemanticException("Types not equal in expression:");
        }
    }

    else if(symbolFromStack == "FACTOR_")
    {
        //std::cout<<"FACTOR_\n";
        if(Semantic::semanticStack.back().first!="arithExprLR")
            Semantic::semanticStack.pop_back();
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("factor",stv.tds.value));
    }

    else if(symbolFromStack == "TERMLR")
    {
        //std::cout<<"TERMLR\n";

        if(Semantic::semanticStack.back().first!="TERMLR")
            return;

        if(Semantic::semanticStack.back().first!="arithExprLR")
            Semantic::semanticStack.pop_back();

        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("term",stv.tds.value));
    }

    else if(symbolFromStack == "ARITHEXPRLR")
    {
        std::string expr;
        //std::cout<<"ARITHEXPRLR\n";
        expr = Semantic::semanticStack.back().second;
        Semantic::semanticStack.pop_back();

        //        if(Semantic::semanticStack.back().first!="arithExprLR")
        //            Semantic::semanticStack.pop_back();

        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("arithExprLR",expr));
    }

    else if(symbolFromStack == "COPY_OPERATOR")
    {
        Semantic::m_operator = stv.tds.value;
    }

    else if(symbolFromStack == "TERM")
    {
        Semantic::semanticStack.pop_back();
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("termLR",stv.tds.value));
    }

    else if(symbolFromStack == "FACTOR_VALUE")
    {
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("factor",stv.tds.value));
    }

    else if(symbolFromStack == "EXPR")
    {
        std::string arithExpr = Semantic::semanticStack.back().second;
        Semantic::semanticStack.pop_back();
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("EXPR",arithExpr));
    }

    else if(symbolFromStack == "MUL_DIV_IDS")
    {
        std::string id_1;
        std::string id_2;
        std::string result;
        //std::cout<<"MUL_DIV_IDS\n";
        id_1 = Semantic::semanticStack.back().second;
        Semantic::semanticStack.pop_back();
        id_2 = Semantic::semanticStack.back().second;
        Semantic::semanticStack.pop_back();
        result = id_2 + m_operator + id_1;
        Semantic::semanticStack.push_back(std::pair<std::string,std::string>("arithExprLR",result));
        if(!Semantic::isTypesEqual(id_1,id_2))
        {
            throw SemanticException("Types not equal in expression:");
        }
    }

    else if(symbolFromStack == "CODE_GET" && secondPass)
    {
        CodeGenerator::generateCode("CODE_GET");
    }

    else if(symbolFromStack == "CODE_PUT" && secondPass)
    {
        CodeGenerator::generateCode("CODE_PUT");
    }
}

/*
 * At the moment it checks the type of the class and free function in the global table. TODO Extension could be to check a member function
 */
bool Semantic::doesTypeOrFunctionExist(std::string& id_type)
{
    if(symbolTables["global"].count(id_type) == 1)
    {
        return true;
    }
    return false;
}

Semantic::~Semantic()
{
}

bool Semantic::doesSymbolExist(const std::string& currentTableName, std::string& symbolName)
{
    if(symbolTables[currentTableName].count(symbolName)==1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Semantic::printSymbolTable()
{
    std::ofstream myfile;
    myfile.open (Semantic::outputFileName.c_str());
    for (std::map < std::string, std::map<std::string, SymbolInfo>  >::iterator iter = symbolTables.begin();
            iter != symbolTables.end();
            ++iter)
    {
        std::cout<<"+--------------------------------"<<iter->first<<"--------------------------------------------+"<<std::endl;
        myfile<<"+--------------------------------"<<iter->first<<"--------------------------------------------+"<<std::endl;
        std::cout<<std::setw(20)<<"Name"<<std::setw(20)<<"Kind"<<std::setw(20)<<"type"<<std::setw(20)<<"link"<<std::endl;
        myfile<<std::setw(20)<<"Name"<<std::setw(20)<<"Kind"<<std::setw(20)<<"type"<<std::setw(20)<<"link"<<std::endl;
        for (auto symRecord : symbolTables.find(iter->first)->second)
        {
            std::cout<<std::setw(20)<<symRecord.first<<std::setw(20)<<symRecord.second.kind<<std::setw(20)
            <<symRecord.second.type<<std::setw(20)<<symRecord.second.link<<std::endl;
            myfile<<std::setw(20)<<symRecord.first<<std::setw(20)<<symRecord.second.kind<<std::setw(20)
            <<symRecord.second.type<<std::setw(20)<<symRecord.second.link<<std::endl;
        }
        std::cout<<"+-----------------------------------------------------------------------------------+"<<std::endl;
        myfile<<"+-----------------------------------------------------------------------------------+"<<std::endl;
    }
}

void Semantic::printSemanticStack()
{
    for(std::vector<std::pair<std::string, std::string>>::reverse_iterator it = semanticStack.rbegin(); it != semanticStack.rend(); ++it)
    {
        std::cout << (*it).first << "  " <<(*it).second<<"\n";
    }
}

bool Semantic::isTypesEqual( std::string left, std::string right)
{

    std::vector<std::string> tmp;
    tmp = currentTable;

    /*
     * TODO if right is a real number then change the type to float
     * refactor this ugly fix
     */
    if(std::all_of(left.begin(), left.end(), ::isdigit) || std::all_of(right.begin(), right.end(), ::isdigit))
    {
            return true;
    }

    /*
     * TODO if the symbol is created by Codegenerator ignore it
     */
    if(left.at(0) == 't' || right.at(0) == 't' )
    {
        return true;
    }

    do
    {
        for (std::vector<std::string>::reverse_iterator  it = currentTable.rbegin() ; it != currentTable.rend(); ++it)
        {
            /*
             *Check all the table from current table to parent table
             *func1()
             *       { int x
             *         func2() {
             *         int y {
             *          func3()
             *          { x = y} } }}
             */
            if(symbolTables[*it].count(left) == 1 && symbolTables[tmp.back()].count(right) == 1 &&
                    (symbolTables[*it][left].type == symbolTables[tmp.back()][right].type))
            {
                return true;
            }
            else if(symbolTables[*it].count(right) == 1 && symbolTables[tmp.back()].count(left) == 1 &&
                    (symbolTables[*it][right].type == symbolTables[tmp.back()][left].type))
            {
                return true;
            }
        }
        if(tmp.size()==0)
            break;
        else
            tmp.pop_back();
    }
    while(tmp.size()!=0);
    //symbol not defined both in current as well as parent scopes
    return false;
}


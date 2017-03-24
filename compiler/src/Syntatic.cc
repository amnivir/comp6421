/*
 * Syntatic.cc
 *
 *  Created on: Feb 25, 2017
 *      Author: eshinig
 */
#include "Syntatic.hh"
#include "Semantic.h"
#include <iomanip>

Parser::Parser()
{
    std::vector<std::string> sampleInput= {"class","id","{","int","id","[", "intValue","]",";","}",";",
            "program", "{", "int", "id", "[", "intValue", "]", ";",
            "float", "id", "[", "intValue", "]", ";", "}" ,";",
            "float", "id","(","int", "id", "[", "intValue", "]", ")",
            "{", "float", "id", ";", "return", "(", "intValue" ,"*", "floatValue", ")", ";", "}", ";","$"};
    for(auto str : sampleInput)
    {
        SyntaticTokenValue tv;
        tv.syntacticValue = str;
        tv.tds.lineNum=1;
        tv.tds.type=NONE;
        tv.tds.value=str;
        inputSemanticValue.push_back(tv);
    }

}

Parser::Parser(std::list<TokenDS> tokenDSList)
{
    currentTokenIndex = 0;
    tokenListFromLexicalAnalyser = tokenDSList;
    this->buildInputFromLex();
}

void Parser::buildInputFromLex()
{
    TokenDS token;

    std::list<TokenDS>::iterator it;
    std::string tokenValue;
    inputSemanticValue.clear();

    std::string previousTokenValue = "";

    while(!tokenListFromLexicalAnalyser.empty())
    {
        SyntaticTokenValue tv;
        it = tokenListFromLexicalAnalyser.begin();
        tv.tds.lineNum = it->lineNum;
        tv.tds.type = it->type;
        tv.tds.value = it->value;
        if(it->type == ID )
        {
            tv.syntacticValue = "id";
            //            input.push_back("id");
        }

        else if (it->type == INT_VALUE || it->type==FLOAT_VALUE )
        {
            /**
             *  Make sure that only integer must be inside  indices [ integer ]
             *  else it does not matter
             */
            if(previousTokenValue == "[")
                tv.syntacticValue = "intValue";
            //input.push_back("intValue");

            else if (it->type == INT_VALUE)
                tv.syntacticValue = "intValue";
            //input.push_back("intValue");
            else if (it->type == FLOAT_VALUE)
                tv.syntacticValue = "floatValue";
            //input.push_back("floatValue");// num is both integer and float
        }

        else
        {
            tv.syntacticValue = it->value;
            //input.push_back(it->value);
        }

        inputSemanticValue.push_back(tv);
        previousTokenValue = it->value;

        tokenListFromLexicalAnalyser.pop_front();
    }
}

bool Parser::isTerminal(std::string& x)
{
    for (auto symbol: terminalSymbols)
        if(symbol == x)
            return true;
    return false;
}

void Parser::printDerivation()
{
    std::cout << "Derivation == ";
    for (auto symbol : derivation)
    {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;
}

void Parser::printInverseDerivation()
{
    std::cout << "inverseDerivation == ";
    for (auto symbol : stackInverseDerivation)
    {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;
}

void Parser::parseTerminalSymbol(const std::string& symbol, std::string& token)
{
    std::cout << "Matching token found! " << symbol << std::endl;
    /*
     * Found the symbol that was expected.
     */
    if (!inputSemanticValue.empty())
    {
        inputSemanticValue.pop_front();
    }
    if (!inputSemanticValue.empty())
    {
        //token = inputSemanticValue.front();
        token=inputSemanticValue.front().syntacticValue;
    }
    stackInverseDerivation.pop_back();
}


void Parser::performAction(const std::string& symbolFromStack, const SyntaticTokenValue& stv,
        const std::string& previousSymbolFromLHSProduction)
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
        symbolTables["global"][name] = symInfo;
    }

    else if(symbolFromStack == "COPY_TYPE")
    {
        std::cout<<"TYPE="<<stv.syntacticValue<<"  "<<stv.tds.value<<std::endl;
        std::cout<<"Production LHS:  "<<previousSymbolFromLHSProduction<<std::endl;
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
        symbolTables["global"][name] = symInfo;
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
        symbolTables[currentTable.back()][nonTerminalSymValue["id_value"]] = symInfo;
        nonTerminalSymValue.clear();

    }
}

void Parser::printSymbolTable(const std::string& tableName)
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

void Parser::tableDrivenParserAlgorithm()
{
    std::ofstream myfile;
    myfile.open ("syntatic_output.txt");
    std::string token;
    std::cout<<"No. of predictions="<<productions.size()<<std::endl;
    stackInverseDerivation.push_back("$");
    stackInverseDerivation.push_back("prog");
    derivation.push_back("prog");
    // token = input.front();
    token = inputSemanticValue.front().syntacticValue;

    SyntaticTokenValue previousToken;
    std::string previousSymbolFromLHSProduction;

    while(stackInverseDerivation.back()!="$")
    {
        std::cout<<"Token= "<<token<<std::endl;
        printDerivation();
        printInverseDerivation();
        std::string symbolFromStack = stackInverseDerivation.back();
        std::cout<<"Symbol From Stack=="<<symbolFromStack<<std::endl;
        if( isTerminal(symbolFromStack))
        {
            previousToken = inputSemanticValue.front();
            parseTerminalSymbol(symbolFromStack, token);
            //else skip error
        }

        else if(Semantic::isSemanticAction(symbolFromStack))
        {
            stackInverseDerivation.pop_back();
            performAction(symbolFromStack,previousToken,previousSymbolFromLHSProduction);
        }

        //non terminal
        else
        {
            int row = nonTerminalSymbolsMap[symbolFromStack];
            int column = terminalSymbolsMap.find(token)->second;
            int rule = parseTable[row][column];
            std::cout<<"Row= "<<row<<"  Column= "<<column<<"  Rule= "<<rule<<std::endl;
            if( rule < ERROR_CODE) //
            {
                std::list<std::string>::iterator it;
                std::list<std::string> tmpSymbols; //Use to reverse
                std::list<std::string> tmpSymbolsWithoutSemanticActions; //Use to reverse
                std::cout<< "Used Rule:  "<< symbolFromStack << " -> ";
                myfile << symbolFromStack << "   ->   " ;
                for(auto symbol : productions.find(rule)->second)
                {
                    std::cout<<symbol<<"   ";
                    myfile << symbol<<"   " ;
                    tmpSymbols.push_back(symbol);
                    //Do not put semantic actions into derivation
                    if(std::find(Semantic::semanticActions.begin(), Semantic::semanticActions.end(),
                            symbol) == Semantic::semanticActions.end())
                    {
                        tmpSymbolsWithoutSemanticActions.push_back(symbol);
                    }
                }
                std::cout<<std::endl;
                myfile << std::endl;
                stackInverseDerivation.pop_back();
                it = std::find(derivation.begin(), derivation.end(), symbolFromStack);
                it = derivation.erase(it);

                if(tmpSymbols.front() == "EPSILON")
                    continue;

                derivation.insert(it,tmpSymbolsWithoutSemanticActions.begin(),tmpSymbolsWithoutSemanticActions.end());
                tmpSymbols.reverse();
                for(auto symbol : tmpSymbols)
                {
                    stackInverseDerivation.push_back(symbol);
                }
            }
            previousSymbolFromLHSProduction = symbolFromStack;
        }
    }
    myfile.close();
    std::cout<<"Algorithm finished!!!!!!!\n";
    printDerivation();
    printInverseDerivation();
    printSymbolTable("global");
    printSymbolTable("f1");
}


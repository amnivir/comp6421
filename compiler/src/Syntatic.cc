/*
 * Syntatic.cc
 *
 *  Created on: Feb 25, 2017
 *      Author: eshinig
 */
#include "Syntatic.hh"
#include "Semantic.h"
#include "CodeGenerator.h"
#include "Exceptions.hh"

Parser::Parser(std::string fileName)
{
    std::vector<std::string> sampleInput= {"class","id","{","int","id","[", "intValue","]",";","}",";",
            "program", "{", "int", "id", "[", "intValue", "]", ";",
            "float", "id", "[", "intValue", "]", ";", "}" ,";",
            "float", "id","(","int", "id", "[", "intValue", "]", ")",
            "{", "float", "id", ";", "return", "(", "intValue" ,"*", "floatValue", ")", ";", "}", ";","$"};
    SyntaticTokenValue tv;
    for(auto str : sampleInput)
    {

        tv.syntacticValue = str;
        tv.tds.lineNum=1;
        tv.tds.type=NONE;
        tv.tds.value=str;
        inputSemanticValue.push_back(tv);
        //inputSemanticValueCopy.push_back(tv);//second copy
    }

    this->fileName = fileName + "_Syntactic.txt";
    Semantic::outputFileName = fileName + "_Semantic.txt";
    CodeGenerator::outputFileName = fileName + "_AssemblyCode.m";
}

Parser::Parser(std::list<TokenDS> tokenDSList,std::string fileName)
{
    tokenListFromLexicalAnalyser = tokenDSList;
    this->buildInputFromLex();
    this->fileName = fileName + "_Syntactic.txt";
    Semantic::outputFileName = fileName + "_Semantic.txt";
    CodeGenerator::outputFileName = fileName + "_AssemblyCode.m";
}

void Parser::buildInputFromLex()
{
    TokenDS token;

    std::list<TokenDS>::iterator it;
    std::string tokenValue;
    inputSemanticValue.clear();
    SyntaticTokenValue tv;
    std::string previousTokenValue = "";

    while(!tokenListFromLexicalAnalyser.empty())
    {

        it = tokenListFromLexicalAnalyser.begin();
        tv.tds.lineNum = it->lineNum;
        tv.tds.type = it->type;
        tv.tds.value = it->value;
        if(it->type == ID )
        {
            tv.syntacticValue = "id";
            //            input.push_back("id");
        }
        else if(it->value== "." )
        {
            tv.syntacticValue = ".";
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
        inputSemanticValueCopy.push_back(tv);
        previousTokenValue = it->value;

        tokenListFromLexicalAnalyser.pop_front();
    }
    tv.tds.lineNum = 0;
    tv.tds.type = NONE;
    tv.tds.value = "$";
    tv.syntacticValue="$";

    inputSemanticValue.push_back(tv);
    inputSemanticValueCopy.push_back(tv);
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
    //std::cout << "Matching token found! " << symbol << std::endl;
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

void Parser::errorRecovery(int code,std::string &token,std::string &previousToken,int lineNum)
{
    if (code == POP_CODE)
    {
        //      std::cout<<"ERROR: Missing token: "<<previousToken<<std::endl;
        stackInverseDerivation.pop_back();
    }

    else if(code == SCAN_CODE)
    {
        std::cout<<"ERROR: Missing token before : "<<token<<"  line:"<<lineNum<<std::endl;
        inputSemanticValue.pop_front();
        token=inputSemanticValue.front().syntacticValue;
    }
}

void Parser::tableDrivenParserAlgorithm(bool secondPass)
{
    std::ofstream myfile;
    myfile.open (this->fileName.c_str());
    std::string token;
    //std::cout<<"No. of predictions="<<productions.size()<<std::endl;
    stackInverseDerivation.push_back("$");
    stackInverseDerivation.push_back("prog");
    derivation.push_back("prog");
    // token = input.front();
    token = inputSemanticValue.front().syntacticValue;

    SyntaticTokenValue previousToken;
    std::string previosTokenValue;

    while(stackInverseDerivation.back()!="$")
    {
        //std::cout<<"Token= "<<token<<std::endl;
        //printDerivation();
        //printInverseDerivation();
        std::string symbolFromStack = stackInverseDerivation.back();
        //std::cout<<"Symbol From Stack=="<<symbolFromStack<<std::endl;
        if( isTerminal(symbolFromStack))
        {
            previousToken = inputSemanticValue.front();
            previosTokenValue = inputSemanticValue.front().tds.value;
            parseTerminalSymbol(symbolFromStack, token);
            //else skip error
        }

        else if(Semantic::isSemanticAction(symbolFromStack))
        {
            stackInverseDerivation.pop_back();
            Semantic::performAction(symbolFromStack,previousToken,secondPass);
        }

        else if(symbolFromStack == "EPSILON")
        {
            stackInverseDerivation.pop_back();
        }
        //non terminal
        else
        {
            int row = nonTerminalSymbolsMap[symbolFromStack];
            int column = terminalSymbolsMap.find(token)->second;
            int rule = parseTable[row][column];
            //std::cout<<"Row= "<<row<<"  Column= "<<column<<"  Rule= "<<rule<<std::endl;
            if( rule <= ERROR_CODE && rule > 0) //
            {
                std::list<std::string>::iterator it;
                std::list<std::string> tmpSymbols; //Use to reverse
                std::list<std::string> tmpSymbolsWithoutSemanticActions; //Use to reverse
                //std::cout<< "Used Rule:  "<< symbolFromStack << " -> ";
                myfile << symbolFromStack << "   ->   " ;
                for(auto symbol : productions.find(rule)->second)
                {
                    //std::cout<<symbol<<"   ";
                    myfile << symbol<<"   " ;
                    tmpSymbols.push_back(symbol);
                    //Do not put semantic actions into derivation
                    if(std::find(Semantic::semanticActions.begin(), Semantic::semanticActions.end(),
                            symbol) == Semantic::semanticActions.end() && symbol != "EPSILON")
                    {
                        tmpSymbolsWithoutSemanticActions.push_back(symbol);
                    }
                }
                //std::cout<<std::endl;
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
            else if (rule == SCAN_CODE || rule == POP_CODE)
            {
                parsing = false;
                errorRecovery(rule,token,previosTokenValue,inputSemanticValue.front().tds.lineNum);
            }
            else if( rule == 0)
            {
                throw SemanticException("No CFG Rule found!");
            }
        }
    }
    myfile.close();
    std::cout<<"Algorithm finished!!!!!!!\n";
    printDerivation();
    printInverseDerivation();
    Semantic::printSymbolTable();
    Semantic::printSemanticStack();
}

void Parser::twoPassParser()
{

    try
    {
        /**
     * in first pass, build symbol table and it should not thrown any exception
     */
    this->tableDrivenParserAlgorithm(false);

    if(!parsing)
    {
        throw SyntaticException("");
    }

    Semantic::semanticStack.clear();
    //Semantic::symbolTables.clear();
    this->stackInverseDerivation.clear();
    this->derivation.clear();
    Semantic::currentTable.clear();
    this->copySyntaticTokenValueList();

    /**
     * in second pass, verify symbol table and it should not throw any exception
     */
    this->tableDrivenParserAlgorithm(true);
    CodeGenerator::finalizeCodeGeneration();
    }
    catch ( SemanticException& e )
    {
        throw SemanticException(e.what());
    }
    catch ( SyntaticException& e )
    {
        throw SyntaticException(e.what());
    }

    std::cout<<"No Exceptions Found during parsing.. Parsing Successful!!\n";
}

void Parser::copySyntaticTokenValueList()
{
    std::string str;
    SyntaticTokenValue stv;
    this->inputSemanticValue.clear();
    for(auto item : inputSemanticValueCopy)
    {
        stv.tds.lineNum = item.tds.lineNum;
        stv.tds.type = item.tds.type;
        stv.tds.value = item.tds.value;
        stv.syntacticValue = item.syntacticValue;
        inputSemanticValue.push_back(stv);
    }
    stv.tds.lineNum = 0;
    stv.tds.type = NONE;
    stv.tds.value = "$";
    stv.syntacticValue="$";

    inputSemanticValue.push_back(stv);
}

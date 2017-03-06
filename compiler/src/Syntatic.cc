/*
 * Syntatic.cc
 *
 *  Created on: Feb 25, 2017
 *      Author: eshinig
 */
#include "Syntatic.hh"


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
    input.clear();
    while(!tokenListFromLexicalAnalyser.empty())
    {
        it = tokenListFromLexicalAnalyser.begin();
        tokenListFromLexicalAnalyser.pop_front();
        if(it->type == ID )
        {
            input.push_back("id");
        }

        else if (it->type == NUM )
        {
            input.push_back("integer");
        }

        else input.push_back(it->value);
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
    for (auto symbol : inverseDerivation)
    {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;
}

void Parser::parseTerminalSymbol(const std::string& nonTerminal, std::string& token)
{
    std::cout << "Matching token found! " << nonTerminal << std::endl;
    /*
     * Found the symbol that was expected.
     */
    if (!input.empty())
    {
        input.pop_front();
    }
    if (!input.empty())
    {
        token = input.front();
    }
    inverseDerivation.pop_back();
}

void Parser::tableDrivenParserAlgorithm()
{
    std::ofstream myfile;
    myfile.open ("syntatic_output.txt");
    std::string token;
    std::cout<<"No. of predictions"<<productions.size()<<std::endl;
    inverseDerivation.push_back("$");
    inverseDerivation.push_back("prog");
    derivation.push_back("prog");
    token = input.front();
    while(inverseDerivation.back()!="$")
    {
        std::cout<<"Token= "<<token<<std::endl;
        printDerivation();
        printInverseDerivation();
        std::string nonTerminal = inverseDerivation.back();
        if( isTerminal(nonTerminal))
        {
            parseTerminalSymbol(nonTerminal, token);
            //else skip error
        }
        //non terminal
        else
        {
            int row = nonTerminalSymbolsMap.find(nonTerminal)->second;
            int column = terminalSymbolsMap.find(token)->second;
            int rule = parseTable[row][column];
            std::cout<<"Row= "<<row<<"  Column= "<<column<<"  Rule= "<<rule<<std::endl;
            if( rule < ERROR_CODE) //
            {

                std::list<std::string>::iterator it;
                std::list<std::string> tmpSymbols; //Use to reverse
                //std::cout<< "Used Rule:  "<< nonTerminal << " -> ";
                myfile << nonTerminal << "   ->   " ;
                for(auto symbol : productions.find(rule)->second)
                {
                    //std::cout<<symbol<<"  ";
                    myfile << symbol ;
                    tmpSymbols.push_back(symbol);
                }
                //std::cout<<std::endl;
                myfile << std::endl;
                inverseDerivation.pop_back();
                it = std::find(derivation.begin(), derivation.end(), nonTerminal);
                it = derivation.erase(it);

                if(tmpSymbols.front() == "EPSILON")
                    continue;

                derivation.insert(it,tmpSymbols.begin(),tmpSymbols.end());
                tmpSymbols.reverse();
                for(auto symbol : tmpSymbols)
                {
                    inverseDerivation.push_back(symbol);
                }
            }
        }
    }
    myfile.close();
    std::cout<<"Algorithm finished!!!!!!!\n";
    printDerivation();
    printInverseDerivation();
}


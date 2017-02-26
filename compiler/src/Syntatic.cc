/*
 * Syntatic.cc
 *
 *  Created on: Feb 25, 2017
 *      Author: eshinig
 */
#include "Syntatic.hh"

Parser::Parser(){

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

void Parser::tableDrivenParserAlgorithm()
{
    std::string token;
    std::cout<<"Test";
    std::cout<<"No. of predictions"<<productions.size()<<std::endl;
    inverseDerivation.push_back("$");
    inverseDerivation.push_back("E");
    derivation.push_back("E");
    token = input.front();
    while(inverseDerivation.back()!="$")
    {
        std::cout<<"Token= "<<token<<std::endl;
        printDerivation();
        printInverseDerivation();
        std::string nonTerminal = inverseDerivation.back();
        if( isTerminal(nonTerminal))
        {
            std::cout << "Matching token found! " << nonTerminal <<std::endl;
            /*
             * Found the symbol that was expected.
             */
            input.pop_front();
            token = input.front();
            inverseDerivation.pop_back();
            //else skip error
        }
        //non terminal
        else
        {
            int row = nonTerminalSymbolsMap.find(nonTerminal)->second;
            int column = terminalSymbolsMap.find(token)->second;
            int rule = parseTable[row][column];
            std::cout<<"Row= "<<row<<"  Column= "<<column<<"  Rule= "<<rule<<std::endl;
            if( rule < 10) //
            {

                std::list<std::string>::iterator it;
                std::list<std::string> tmpSymbols; //Use to reverse
                for(auto symbol : productions.find(rule)->second)
                {
                    std::cout<<"Symbol from Rule= "<<symbol<<std::endl;
                    tmpSymbols.push_back(symbol);
                }

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
}


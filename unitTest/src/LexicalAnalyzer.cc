/*
 * Lex.cc
 *
 *  Created on: Jan 28, 2017
 *      Author: eshinig
 */

#include <LexicalAnalyzer.hh>

Lex::Lex()
{

}
Lex::Lex(std::string rawToken)
{
    currentCharIndex = 0;
    this->rawToken = rawToken;
}

std::string Lex::nextToken() // to be called by syntatic analyzer
{
    return "";
}

int Lex::table(int state,int colomn)
{
    return -1;
}

char Lex::nextChar()
{
    char ch = (char)rawToken[currentCharIndex];
    currentCharIndex++;
    return ch;
}

void Lex::backupChar()
{
    currentCharIndex--;
}

bool Lex::isFinalState(int state)
{
    if(stateFinalToken.find(state) == stateFinalToken.end())
        return false;
    else
        return true;
}

std::string Lex::createToken(int state)
{
    std::cout<<"TokenType for state="<<state<<" is"<<stateFinalToken.find(state)->second<<std::endl;
    return stateFinalToken.find(state)->second;
}

int Lex::charType(char lookup, int state)
{
    if (isalpha(lookup))
    {
        std::cout << "previous state=" << state << std::endl;
        std::cout << "regexPosition=" << regexPosition.find('l')->second << std::endl;
        state = transistionTable[state - 1][regexPosition.find('l')->second - 1];
        std::cout << "Found alpha=" << lookup << "Current  state=" << state << std::endl;
    }
    else if (isdigit(lookup))
    {
        std::cout << "previous state=" << state << std::endl;
        std::cout << "regexPosition=" << regexPosition.find('d')->second << std::endl;
        state = transistionTable[state - 1][regexPosition.find('d')->second - 1];
        std::cout << "Found digit=" << lookup << "Current  state=" << state << std::endl;
    }
    else if (lookup == '{' || lookup == '}' || lookup == '(' || lookup == '*' || lookup == ')'
            || lookup == ':' || lookup == '=' || lookup == '<' || lookup == '>' || lookup == ';')
    {
        std::cout << "previous state=" << state << std::endl;
        std::cout << "regexPosition=" << regexPosition.find(lookup)->second << std::endl;
        state = transistionTable[state - 1][regexPosition.find(lookup)->second - 1];
        std::cout << "Found alpha_numeric=" << lookup << "Current state=" << state << std::endl;
    }

    return state;
}

void Lex::findTokens()
{
    int state = 1;

    std::string token;
    char lookup;
    std::cout<<"RawToken=   "<<rawToken<<std::endl;
    token.clear();
    do
    {
        lookup = this->nextChar();
        std::cout<<"Lookup="<<"'"<<lookup<<"'"<<std::endl;

        state = charType(lookup, state);

        if(isFinalState(state) || lookup == '\0')
        {
            std::cout<<"Pushing Token='"<<token<<"'"<<std::endl;
            std::cout<<"Token State='"<<state<<"'"<<std::endl;
            std::cout<<"Token State='"<<createToken(state)<<"'"<<std::endl;
            tokenList.push_back(token);

            TokenDS tokenStructure;

            tokenStructure.lineNum =1;
            //tokenStructure.type = createToken(state + 1);
            tokenStructure.value = token;
            //tokenListDS.push_back(tokenStructure);
            token.clear();
            if(charBackTrack.find(state)->second == 'y')
            {
                std::cout<<"Backing one character!"<<std::endl;
                backupChar();
                continue;
            }
        }
        if(lookup!=' ') //ignore spaces
        {
            token += lookup;
        }
    }
    while(lookup!='\0');

    for(auto &token:tokenList)
    {
        std::cout<<"Valid Token:"<<token<<std::endl;
    }
}

/*
 * Lex.cc
 *
 *  Created on: Jan 28, 2017
 *      Author: eshinig
 */

#include "../src/Lex.hh"

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
    return stateFinalToken.find(state)->second;
}
//    nextToken()
//      state = 1
//      token = null
//      do
//        lookup = nextChar()
//        state = table(state, lookup)
//        if (isFinalState(state))
//          token = createToken(state)
//          if (table(state, “backup”) == yes)
//            backupChar()
//      until (token != null)
//      return (token)
void Lex::findTokens()
{
    int state = 1;

    int tokenLength = rawToken.size();
    int count = 0;
    std::string token;
    char lookup;
    std::cout<<"RawToken=   "<<rawToken<<std::endl;
    token.clear();
    do
    {
        lookup = this->nextChar();
        std::cout<<"Lookup="<<"'"<<lookup<<"'"<<std::endl;
        if(isalpha(lookup))
        {
            std::cout<<"previous state="<<state<<std::endl;
            std::cout<<"regexPosition="<<regexPosition.find('l')->second<<std::endl;
            state = transistionTable[state -1][regexPosition.find('l')->second - 1];
            std::cout<<"Found alpha="<<lookup<<"Current  state="<<state<<std::endl;
        }

        else if(isdigit(lookup))
        {
            std::cout<<"previous state="<<state<<std::endl;
            std::cout<<"regexPosition="<<regexPosition.find('d')->second<<std::endl;
            state = transistionTable[state -1][regexPosition.find('d')->second - 1];
            std::cout<<"Found digit="<<lookup<<"Current  state="<<state<<std::endl;
        }

        else if(lookup =='{' || lookup =='}' ||lookup =='(' ||lookup =='*' ||
                lookup ==')' ||lookup ==':' ||lookup == '=' ||lookup =='<' || lookup =='>')
        {
            std::cout<<"previous state="<<state<<std::endl;
            std::cout<<"regexPosition="<<regexPosition.find(lookup)->second<<std::endl;
            state = transistionTable[state -1][regexPosition.find(lookup)->second - 1];
            std::cout<<"Found alpha_numeric="<<lookup<<"Current state="<<state<<std::endl;
        }

//        //Handle error
//        else
//        {
//            std::cout<<"Invalid Token";
//        }

        if(isFinalState(state) || lookup == '\0')
        {
            //Create Token
            std::cout<<"Pushing Token='"<<token<<"'"<<std::endl;
            tokenList.push_back(token);
            TokenDS tokenStructure;

            tokenStructure.lineNum =1;
            tokenStructure.type = createToken(state);
            tokenStructure.value = token;
            tokenListDS.push_back(tokenStructure);
            token.clear();
            if(charBackTrack.find(state)->second == 'y')
            {
                std::cout<<"Backing one character!"<<std::endl;
                backupChar();
                continue;
            }
        }

        token += lookup;

        count++;
    }
    while(lookup!='\0');

    for(auto &token:tokenListDS )
    {
        std::cout<<"Valid Token: Token type "<<token.type<<"  Token Value="<<token.value<<std::endl;
    }
}

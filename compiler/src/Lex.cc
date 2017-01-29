/*
 * Lex.cc
 *
 *  Created on: Jan 28, 2017
 *      Author: eshinig
 */

#include "Lex.hh"

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
}

bool Lex::isFinalState(int state)
{
    return false;
}

void Lex::createToken(int state)
{

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
    std::string token = "";
    char lookup;
    std::cout<<"RawToken=   "<<rawToken<<std::endl;
    do
    {
        lookup = this->nextChar();
        std::cout<<"Lookup="<<lookup<<std::endl;
        if(isalpha(lookup))
        {
            state = transistionTable[regexPosition.find('l')->second - 1][state -1];
            std::cout<<"Found alpha="<<lookup<<"  state="<<state<<std::endl;
        }

        else if(isdigit(lookup))
        {
            state = transistionTable[regexPosition.find('d')->second - 1][state -1];
            std::cout<<"Found digit="<<lookup<<"  state="<<state<<std::endl;
        }

        else if(lookup =='{' || lookup =='}' ||lookup =='(' ||lookup =='*' ||
                lookup ==')' ||lookup ==':' ||lookup == '=' ||lookup =='<' || lookup =='>')
        {
            state = transistionTable[state -1][regexPosition.find(lookup)->second - 1];
            std::cout<<"Found alpha_numeric="<<lookup<<"  state="<<state<<std::endl;
        }


        else
        {
            std::cout<<"Invalid Token";
        }

        if(isFinalState(state))
        {

        }

        //std::cout<<this->nextChar();
        count++;
    }
    while(count<tokenLength);
}

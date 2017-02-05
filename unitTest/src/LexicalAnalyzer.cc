/*
 * Lex.cc
 *
 *  Created on: Jan 28, 2017
 *      Author: eshinig
 */

#include <LexicalAnalyzer.hh>

Lex::Lex()
{
 lineNumber = 1;
 currentCharIndex = 0;
}
Lex::Lex(std::string rawToken, int lineNumber)
{
    currentCharIndex = 0;
    this->rawToken = rawToken;
    this->lineNumber = lineNumber;
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
    {
        std::cout<<"NOT final State! state="<<state<<std::endl;
        return false;
    }
    else
    {
        std::cout<<"Final state! state="<<state<<std::endl;
        return true;
    }
}

/*
 * Error handling is done here, if ';' is missing then check if the last state is
 * letter or digit
 */
std::string Lex::createToken(int state)
{
    if(state == 4)
        state = 5;

    else if(state == 2)
        state = 3;
    std::cout<<"TokenType for state="<<state<<"  is "<<stateFinalToken.find(state)->second<<std::endl;
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
            || lookup == ':' || lookup == '=' || lookup == '<' || lookup == '>' || lookup == ';'
            || lookup == '+' || lookup == '/' || lookup == '-')
    {
        std::cout << "previous state=" << state << std::endl;
        std::cout << "regexPosition=" << regexPosition.find(lookup)->second << std::endl;
        state = transistionTable[state - 1][regexPosition.find(lookup)->second - 1];
        std::cout << "Found alpha_numeric=" << lookup << "Current state=" << state << std::endl;
    }
    return state;
}

void Lex::buildTokenDataStructureAndAddToList(const std::string& token, const std::string& tokenType)
{
    TokenDS tokenStructure;

    if( tokenType == "id")
        tokenStructure.type = ID;

    else if( tokenType == "keyword")
        tokenStructure.type = KEYWORD;

    else if( tokenType == "num")
        tokenStructure.type = NUM;

    else if( tokenType == "assign")
        tokenStructure.type = ASSIGNMENT;

    else if( tokenType == "mul" || tokenType == "div" ||tokenType == "add" ||tokenType == "div")
        tokenStructure.type = OPERATOR;

    else if( tokenType == "gt" || tokenType == "lt" || tokenType == "lesseq" || tokenType == "moreeq"
            || tokenType == "eqeq")
        tokenStructure.type = COMPARATOR;

    else if( tokenType == "openpar" || tokenType == "closepar" || tokenType == "opencurly"
            || tokenType == "closecurly" || tokenType == "opensqre" || tokenType == "closesqre")
        tokenStructure.type = BRACKETS;

    else if( tokenType == "semicolon")
        tokenStructure.type = PUNCTUATION;

    else
        tokenStructure.type = ERROR;

    tokenStructure.lineNum = this->lineNumber;
    tokenStructure.value = token;
    tokenListDS.push_back(tokenStructure);
}

std::string Lex::convertEnumToString(const TokenType& type)
{
    switch(type)
    {
    case ID:
        return "ID";// x,y

    case NUM: // 10,20
        return "Number";

    case KEYWORD:  //if , else , int , float
        return "Keyword";

    case OPERATOR: //+,-,/,*
        return "Operator";

    case COMPARATOR: //==,<=,>=
        return "Comparator";

    case ASSIGNMENT://=
        return "Assignment";

    case PUNCTUATION://;
        return "Punctuation";

    case BRACKETS:// { [ ( ) ] }
        return "Brackets";

    default:
        return "No Enum Found";
    }
}
void Lex::printTokens()
{
    for (auto& token : tokenList)
    {
        std::cout << "Valid Token:" << token << std::endl;
    }
}

void Lex::printTokenDataStruct()
{
    for (auto& token : tokenListDS)
    {
        std::cout << "Token Value=" << token.value << std::endl;
        std::cout << "Token Type=" << convertEnumToString(token.type) << std::endl;
        std::cout << "Token Line=" << token.lineNum << std::endl;
    }
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
        std::cout<<"Token="<<"'"<<token<<"'"<<std::endl;
        state = charType(lookup, state);

        if((isFinalState(state) || lookup == '\0') && !token.empty())
        {
            if(token == "<" || token == ">" )
            {
                token += lookup;
            }
            std::cout<<"Pushing Token='"<<token<<"'"<<std::endl;
            std::cout<<"Token State='"<<state<<"'"<<std::endl;
            std::cout<<"Token State='"<<createToken(state)<<"'"<<std::endl;
            tokenList.push_back(token);
            buildTokenDataStructureAndAddToList(token,createToken(state));
            token.clear();
            if(charBackTrack.find(state)->second == 'y')
            {
                std::cout<<"Backing one character!"<<std::endl;
                backupChar();
                state = 1;
                continue;
            }
            state = 1;
        }
        if(isFinalState(state)  && token.empty() && lookup!= '\0')
        {
            std::cout<<"Pushing Token='"<<lookup<<"'"<<std::endl;
            std::cout<<"Token State='"<<state<<"'"<<std::endl;
            std::cout<<"Token State='"<<createToken(state)<<"'"<<std::endl;
            tokenList.push_back(std::string(1,lookup));
            buildTokenDataStructureAndAddToList(token,createToken(state));
            state = 1;
            continue;
        }
        else if(lookup!=' ') //ignore spaces
        {
            token += lookup;
        }
    }
    while(lookup!='\0');

    printTokens();
}

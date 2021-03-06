/*
 * Lex.cc
 *
 *  Created on: Jan 28, 2017
 *      Author: eshinig
 */

#include <LexicalAnalyzer.hh>
#include <fstream>
#include<boost/tokenizer.hpp>

Lex::Lex(std::string outputFile)
{
    lineNumber = 1;
    currentCharIndex = 0;
    this->outputFileName = outputFile + "_Lexic.txt";
}

void Lex::buildTokenDSFromFile()
{
    using namespace std;
    using namespace boost;
    string line;
    int lineNumCount = 1;
    ifstream myfile(this->inputFileName);
    char_separator<char> sept
    { "" };
    std::list<std::string> rawTokensFromInputFile;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            tokenizer tokens
            { line, sept };
            for (const auto& token : tokens)
            {
                Lex lex(token, lineNumCount);
                lex.findTokenTypeAndBuildList();
                this->tokenListDS.insert(this->tokenListDS.end(),
                        lex.tokenListDS.begin(), lex.tokenListDS.end());
            }
            lineNumCount++;
        }
    }
    else
    {
        cout << "File cannot be opened";
    }
}

Lex::Lex(char* fileName,std::string outputFile)
{
    using namespace std;
    lineNumber = 1;
    currentCharIndex = 0;
    this->outputFileName = outputFile;
    this->inputFileName = fileName;
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
        //std::cout<<"NOT final State! state="<<state<<std::endl;
        return false;
    }
    else
    {
        //std::cout<<"Final state! state="<<state<<std::endl;
        return true;
    }
}

/*
 * Error handling is done here, if ';' is missing then check if the last state is
 * letter or digit
 */
std::string Lex::createToken(int state, std::string& token)
{
    if(state == 4)
        state = 5;

    else if(state == 2)
        state = 3;

    /**
     * ID could be a keyword
     */
    else if(state == 3)
    {
        for (auto &keyword : this->keywords )
            if(keyword == token)
            {
                //std::cout<<"TokenType for state="<<state<<"  is "<<stateFinalToken.find(state)->second<<
                //        " converted to Keyword"<<std::endl;
                return "keyword";
            }
    }

    //std::cout<<"TokenType for state="<<state<<"  is "<<stateFinalToken.find(state)->second<<std::endl;

    return stateFinalToken.find(state)->second;
}

int Lex::charType(char lookup, int state)
{
    if (isalpha(lookup))
    {
        //std::cout << "previous state=" << state << std::endl;
        //std::cout << "regexPosition=" << regexPosition.find('l')->second << std::endl;
        state = transistionTable[state - 1][regexPosition.find('l')->second - 1];
        //std::cout << "Found alpha=" << lookup << "Current  state=" << state << std::endl;
    }
    else if (isdigit(lookup))
    {
        //std::cout << "previous state=" << state << std::endl;
        //std::cout << "regexPosition=" << regexPosition.find('d')->second << std::endl;
        state = transistionTable[state - 1][regexPosition.find('d')->second - 1];
        //std::cout << "Found digit=" << lookup << "Current  state=" << state << std::endl;
    }
    else if (lookup == '{' || lookup == '}' || lookup == '(' || lookup == '*' || lookup == ')'
            || lookup == '[' || lookup == ']'|| lookup == ' ' || lookup == ',' || lookup == '.'
                    || lookup == ':' || lookup == '=' || lookup == '<' || lookup == '>' || lookup == ';'
                            || lookup == '+' || lookup == '/' || lookup == '-')
    {
        //std::cout << "previous state=" << state << std::endl;
        //std::cout << "regexPosition=" << regexPosition.find(lookup)->second << std::endl;
        state = transistionTable[state - 1][regexPosition.find(lookup)->second - 1];
        //std::cout << "Found alpha_numeric=" << lookup << "Current state=" << state << std::endl;
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

    /**
     * NUM can be integer or float. Num alone cannot be usefil for assignment 2
     * check '.' in token if found then it is float else integer
     */
    else if( tokenType == "num")
    {
        std::string dot = ".";
        std::size_t found = token.find(dot);
        if(found != std::string::npos)
        {
            tokenStructure.type = FLOAT_VALUE;
        }
        else
        {
            tokenStructure.type = INT_VALUE;
        }
    }

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

    else if( tokenType == "error")
        tokenStructure.type = ERROR;

    else if( tokenType == "comment")
    {
        std::cout<<"Comment found, Don't build token"<<std::endl;
        return;
    }

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

    case INT_VALUE:
        return "intValue";

    case FLOAT_VALUE:
        return "floatValue";

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

    case ERROR:// { [ ( ) ] }
        return "Error in line";

    default:
        return "No Enum Found";
    }
}
void Lex::printTokens()
{
    for (auto& token : tokenList)
    {
        //std::cout << "Valid Token:" << token << std::endl;
    }
}

void Lex::printTokenDataStruct()
{
    std::cout <<"****************************************************************"<<std::endl;
    for (auto& token : tokenListDS)
    {
        std::cout << "Token Value=" << token.value << std::endl;
        std::cout << "Token Type=" << convertEnumToString(token.type) << std::endl;
        std::cout << "Token Line=" << token.lineNum << std::endl;
        std::cout <<"****************************************************************"<<std::endl;
    }
}

void Lex::writeTokenDataStructToFile()
{
    std::ofstream myfile;
    myfile.open (this->outputFileName.c_str());
    myfile <<"****************************************************************"<<std::endl;
    for (auto& token : tokenListDS)
    {
        myfile << "Token Value=" << token.value << std::endl;
        myfile << "Token Type=" << convertEnumToString(token.type) << std::endl;
        myfile << "Token Line=" << token.lineNum << std::endl;
        myfile <<"****************************************************************"<<std::endl;
    }

    myfile << "Total Tokens=" << this->getTokenDSList().size();

    myfile.close();
}

std::list<TokenDS> Lex::getTokenDSList()
{
    return tokenListDS;
}

void Lex::findTokenTypeAndBuildList()
{
    int state = 1;

    std::string token;
    char lookup;
    //std::cout<<"RawToken=   "<<rawToken<<std::endl;
    token.clear();
    do
    {
        lookup = this->nextChar();
        //std::cout<<"Lookup="<<"'"<<lookup<<"'"<<std::endl;
        //std::cout<<"Token="<<"'"<<token<<"'"<<std::endl;
        state = charType(lookup, state);
        bool isFinalStateBool = isFinalState(state);

        if((isFinalStateBool || lookup == '\0') && !token.empty())
        {
            if((token == "<" || token == ">") && lookup == '='  )
            {
                token += lookup;
            }
            //std::cout<<"Pushing Token='"<<token<<"'"<<std::endl;
            //std::cout<<"Token State='"<<state<<"'"<<std::endl;
            //std::cout<<"Token State='"<<createToken(state,token)<<"'"<<std::endl;

            if (state != 39) // Ignore comments, comment has a state = 39
            {
                tokenList.push_back(token);
                buildTokenDataStructureAndAddToList(token,createToken(state,token));
            }

            token.clear();
            if(charBackTrack.find(state)->second == 'y')
            {
                //std::cout<<"Backing one character!"<<std::endl;
                backupChar();
                state = 1;
                continue;
            }
            state = 1;
        }
        else if(isFinalStateBool  && token.empty() && lookup!= '\0')
        {
            std::string lookupStr(std::string(1,lookup));
//            std::cout<<"Pushing loookup='"<<lookup<<"'"<<std::endl;
//            std::cout<<"Token State='"<<state<<"'"<<std::endl;
//            std::cout<<"Token State='"<<createToken(state,lookupStr)<<"'"<<std::endl;
            tokenList.push_back(lookupStr);
            buildTokenDataStructureAndAddToList(std::string(1,lookup),createToken(state,lookupStr));
            state = 1;
            continue;
        }
        else if(lookup!=' ') //ignore spaces
        {
            token += lookup;
//            std::cout<<"Concat lookup and previous token! Token="<<token<<std::endl;
        }
    }
    while(lookup!='\0');

}

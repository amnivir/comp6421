/*
 * Lex.hh
 *
 *  Created on: Jan 28, 2017
 *      Author: eshinig
 */
#include <map>
#include <list>
#include <string.h>
#include <iostream>
#include <fstream>

#ifndef LEX_HH_
#define LEX_HH_

enum TokenType
{
    ID, // x,y
    NUM, // 10,20
    KEYWORD, //if , else , int , float
    OPERATOR, //+,-,/,*
    COMPARATOR,//==,<=,>=
    ASSIGNMENT,//=
    PUNCTUATION,//;
    BRACKETS,// { [ ( ) ] }
    ERROR// any thing other then above
};

struct TokenDS
{
    TokenType type;
    std::string value;
    int lineNum;

};

enum Keywords{

};
class Lex{

public:
    Lex();
    Lex(std::string rawToken, int lineNumber);
    std::string rawToken;
    std::string nextToken(); // to be called by syntatic analyzer
    std::list<std::string> tokenList;//TODO to be removed
    std::list<TokenDS> tokenListDS;//
    std::list<std::string> keywords {
        "if", "then", "else", "for", "class", "and", "int", "not", "float", "or", "get", "put",
        "return", "program" };
    int lineNumber;
    int table(int state,int colomn);
    int currentCharIndex;
    char nextChar();
    void backupChar();
    bool isFinalState(int state);
    std::string createToken(int state, std::string& token);
    void findTokenTypeAndBuildList();
    std::string convertEnumToString(const TokenType&  type);
    void printTokens();
    void printTokenDataStruct();
    void writeTokenDataStructToFile();
    std::list<TokenDS> getTokenDSList();

    std::map<char, int> regexPosition
    {
                std::make_pair('l',1),
                std::make_pair('d',2),
                std::make_pair('{',3),
                std::make_pair('}',4),
                std::make_pair('(',5),
                std::make_pair('*',6),
                std::make_pair(')',7),
                std::make_pair(':',8),
                std::make_pair('=',9),
                std::make_pair('<',10),
                std::make_pair('>',11),
                std::make_pair(' ',12),
                std::make_pair(';',13),
                std::make_pair('+',14),
                std::make_pair('-',15),
                std::make_pair('/',16),
                std::make_pair('.',17),
                std::make_pair('[',18),
                std::make_pair(']',19),
                std::make_pair(',',20),
    };

    std::map<int, std::string> stateFinalToken
    {
                std::make_pair(3,"id"),
                std::make_pair(5,"num"),
                std::make_pair(6,"opencurly"),
                std::make_pair(7,"cmt"),
                std::make_pair(8,"openpar"),
                std::make_pair(12,"cmt"),
                std::make_pair(14,"assign"),
                std::make_pair(16,"lesseq"),
                std::make_pair(17,"noteq"),
                std::make_pair(19,"gt"),
                std::make_pair(20,"closecurly"),
                std::make_pair(21,"closepar"),
                std::make_pair(22,"lt"),
                std::make_pair(23,"gt"),
                std::make_pair(24,"closepar"),
                std::make_pair(26,"eqeq"),
                std::make_pair(27,"semicolon"),
                std::make_pair(30,"mul"),
                std::make_pair(28,"add"),
                std::make_pair(29,"sub"),
                std::make_pair(31,"div"),
                std::make_pair(32,"error"),
                std::make_pair(33,"opensqre"),
                std::make_pair(34,"closesqre"),
                std::make_pair(35,"comma"),
                std::make_pair(39,"comment"),
    };

    std::map<int, char> charBackTrack
    {
                std::make_pair(3,'y'),
                std::make_pair(5,'y'),
                std::make_pair(7,'n'),
                std::make_pair(8,'n'),
                std::make_pair(9,'n'),
                std::make_pair(12,'y'),
                std::make_pair(14,'y'),
                std::make_pair(16,'n'),
                std::make_pair(17,'n'),
                std::make_pair(19,'n'),
                std::make_pair(20,'n'),
                std::make_pair(21,'n'),
                std::make_pair(22,'y'),
                std::make_pair(23,'y'),
                std::make_pair(24,'n'),
                std::make_pair(26,'n'),
                std::make_pair(27,'n'),
                std::make_pair(30,'n'),
                std::make_pair(28,'n'),
                std::make_pair(29,'n'),
                std::make_pair(31,'n'),
                std::make_pair(32,'n'),
                std::make_pair(33,'n'),
                std::make_pair(34,'n'),
                std::make_pair(35,'n'),
                std::make_pair(39,'n'),
    };
/*
 * DFA Table
 */
    int transistionTable[39][20]={
                // 1  2  3  4  5  6  7  8  9  10 11 12  13 14 15  16   17  18  19  20
//State          //l  d  {  }  (  *  )  :  =  <  >  sp  ;  +   -   /   .   [   ]   ,
/*1*/            { 2, 4, 6,20, 8,30,21,13,25,15,18, 1, 27, 28, 29, 36,  4, 33, 34, 35},
/*2*/            { 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 , 3,  3,  3,  32, 3,  3,  3 },
/*3*/            { 1, 1, 1, 1, 1, 1, 1, 1,25, 1, 1, 1, 27, 1,  1,  1,  32, 1,  1,  35},
/*4*/            { 5, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 , 5,  5,  5,   9, 5,  5,  5 },
/*5*/            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 27, 1,  1,  1,  32, 1,  1,  35},
/*6*/            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  32, 1,  1,  1 },
/*7*/            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  32, 1,  1,  1 },
/*8*/            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  32, 1,  1,  1 },
/*9*/            {32, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  32, 1,  1,  1 },
/*10*/           {10,10,10,10,10,11,10,10,10,10,10,10,  1, 1,  1,  1,  32, 10, 10, 10},
/*11*/           {10,10,10,10,10,10,12,10,10,10,10,10,  1, 1,  1,  1,  32, 10, 10, 10},
/*12*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  32, 1,  1,  1},
/*13*/           {21,21,21,21,21,21,21,21,14,21,21,21,  1, 1,  1,  1,  32, 21, 21, 21},
/*14*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  32, 1,  1,  1},
/*15*/           {22,22,22,22,22,22,22,22,16,22,17,22,  1, 1,  1,  1,  32, 22, 22, 22},
/*16*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  32, 1,  1,  1},
/*17*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  32, 1,  1,  1},
/*18*/           {23,23,23,23,23,23,23,23,19,23,23,23,  1, 1,  1,  1,  32, 23, 23, 23},
/*19*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*20*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*21*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*22*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*23*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*24*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*25*/           {14,14, 1, 1, 1, 1, 1, 1,26, 1, 1,14,  1, 1,  1,  1,  1 , 1,  1,  1},
/*26*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*27*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*28*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*29*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*30*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*31*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*32*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*33*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1,  1},
/*34*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1, 1 },
/*35*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1, 1 },
/*36*/           {31,31, 1, 1,31,37,32, 1, 1, 1, 1,31,  1, 1,  1,  1,  1 , 1,  1, 1 },
/*37*/           {37,37,37,37,37,38,37,37,37,37,37,37, 37,37, 37, 37, 37 ,37, 37,37 },
/*38*/           {37,37,37,37,37,37,37,37,37,37,37,37, 37,37, 37, 39, 37 ,37, 37,37 },
/*39*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1,  1,  1,  1 , 1,  1, 1 },
    };

private:
    int charType(char lookup, int state);
    void buildTokenDataStructureAndAddToList(const std::string& token, const std::string& tokenType);
};

#endif /* LEX_HH_ */

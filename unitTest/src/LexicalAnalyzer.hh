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
#include <sstream>
#ifndef LEX_HH_
#define LEX_HH_

enum TokenType
{
    ID,
    NUM,
    COMPARATOR,
    ASSIGNMENT,
    PUNCTUATION,
    BRACKETS
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
    Lex(std::string rawToken);
    std::string rawToken;
    std::string nextToken(); // to be called by syntatic analyzer
    std::list<std::string> tokenList;//TODO to be removed
    std::list<TokenDS> tokenListDS;//
    std::list<std::string> keywords {
        "if", "then", "else", "for", "class", "and", "int", "not", "float", "or", "get", "put",
        "return", "program" };
    int table(int state,int colomn);
    int currentCharIndex;
    char nextChar();
    void backupChar();
    bool isFinalState(int state);
    std::string createToken(int state);
    void findTokens();
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
                std::make_pair('s',12),
                std::make_pair(';',13),
    };

    std::map<int, std::string> stateFinalToken
    {
                std::make_pair(3,"id"),
                std::make_pair(5,"num"),
                std::make_pair(7,"cmt"),
                std::make_pair(9,"openpar"),
                std::make_pair(12,"cmt"),
                std::make_pair(14,"assign"),
                std::make_pair(16,"lesseq"),
                std::make_pair(17,"noteq"),
                std::make_pair(19,"gt"),
                std::make_pair(20,"err"),
                std::make_pair(21,"colon"),
                std::make_pair(22,"lt"),
                std::make_pair(23,"gt"),
                std::make_pair(24,"closepar"),
                std::make_pair(26,"eqeq"),
                std::make_pair(27,"semicolon"),
    };

    std::map<int, char> charBackTrack
    {
                std::make_pair(3,'y'),
                std::make_pair(5,'y'),
                std::make_pair(7,'n'),
                std::make_pair(9,'n'),
                std::make_pair(12,'y'),
                std::make_pair(14,'y'),
                std::make_pair(16,'n'),
                std::make_pair(17,'n'),
                std::make_pair(19,'n'),
                std::make_pair(20,'n'),
                std::make_pair(21,'y'),
                std::make_pair(22,'y'),
                std::make_pair(23,'y'),
                std::make_pair(24,'n'),
                std::make_pair(26,'n'),
                std::make_pair(27,'n'),
    };

    int transistionTable[27][13]={
           //    // 1  2  3  4  5 6  7  8  9  10 11 12  13
//State          // l  d  {  }  ( *  )   : =  <  >  sp  ;
/*1*/            { 2, 4, 6,20, 8,20,20,13,25,15,18, 1, 27},
/*2*/            { 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
/*3*/            { 1, 1, 1, 1, 1, 1, 1, 1,25, 1, 1, 1, 1 },
/*4*/            { 5, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },
/*5*/            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*6*/            { 6, 6, 6, 7, 6, 6, 6, 6, 6, 6, 6, 6, 1 },
/*7*/            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*8*/            { 9, 9, 9, 9, 9,10, 9, 9, 9, 9, 9, 9, 1 },
/*9*/            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*10*/           {10,10,10,10,10,11,10,10,10,10,10,10, 1 },
/*11*/           {10,10,10,10,10,10,12,10,10,10,10,10, 1 },
/*12*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*13*/           {21,21,21,21,21,21,21,21,14,21,21,21, 1 },
/*14*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*15*/           {22,22,22,22,22,22,22,22,16,22,17,22, 1 },
/*16*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*17*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*18*/           {23,23,23,23,23,23,23,23,19,23,23,23, 1 },
/*19*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*20*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*21*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*22*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*23*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*24*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*25*/           {14,14, 1, 1, 1, 1, 1, 1,26, 1, 1, 1, 1 },
/*26*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*27*/           { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };

private:
    int charType(char lookup, int state);
};



#endif /* LEX_HH_ */

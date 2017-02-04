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

struct TokenDS
{
    std::string type; //TODO make this enum type
    std::string value;
    int lineNum;

};

class Lex{

public:
    Lex();
    Lex(std::string rawToken);
    std::string rawToken;
    std::string nextToken(); // to be called by syntatic analyzer
    std::list<std::string> tokenList;//TODO to be removed
    std::list<TokenDS> tokenListDS;//TODO to be removed
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
                std::make_pair('s',12)

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
    };

    std::map<int, char> charBackTrack
    {
                std::make_pair(3,'y'),
                std::make_pair(5,'y'),
                std::make_pair(7,'n'),
                std::make_pair(9,'n'),
                std::make_pair(12,'y'),
                std::make_pair(14,'n'),
                std::make_pair(16,'n'),
                std::make_pair(17,'n'),
                std::make_pair(19,'n'),
                std::make_pair(20,'n'),
                std::make_pair(21,'y'),
                std::make_pair(22,'y'),
                std::make_pair(23,'y'),
                std::make_pair(24,'n'),

    };

    int transistionTable[24][12]={
            {2, 4, 6,20, 8,20,20,13,13,15,18, 1},
            {2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {1, 1, 1, 1, 1, 1, 1, 1,13, 1, 1, 1},
            {5, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {6, 6, 6, 7, 6, 6, 6, 6, 6, 6, 6, 6},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {9, 9, 9, 9, 9,10, 9, 9, 9, 9, 9, 9},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {10,10,10,10,10,11,10,10,10,10,10,10},
            {10,10,10,10,10,10,12,10,10,10,10,10},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {21,21,21,21,21,21,21,21,14,21,21,21},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {22,22,22,22,22,22,22,22,16,22,17,22},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {23,23,23,23,23,23,23,23,19,23,23,23},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {1,1,1,1,1,1,1,1,1,1,1,1},
    };


};



#endif /* LEX_HH_ */

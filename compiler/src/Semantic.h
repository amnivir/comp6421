/*
 * Semantic.h
 *
 *  Created on: Mar 22, 2017
 *      Author: eshinig
 */
#include <vector>
#include <string>
#ifndef SRC_SEMANTIC_H_
#define SRC_SEMANTIC_H_

using namespace std;


class Semantic
{
public:
    Semantic();
    static const std::vector<std::string> semanticActions ;
    static bool isSemanticAction(const std::string& symbolFromStack);
    virtual ~Semantic();
};

#endif /* SRC_SEMANTIC_H_ */

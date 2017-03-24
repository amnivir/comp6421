/*
 * Semantic.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: eshinig
 */

#include <Semantic.h>

Semantic::Semantic()
{
}
const std::vector<std::string>  Semantic::semanticActions = {
            "CREATE_GLOBAL_TABLE",
            "CREATE_CLASS_ENTRY_TABLE",
            "CREATE_PROGRAM_ENTRY",
            "CREATE_FUNCTION_ENTRY",
            "CREATE_VARIABLE_ENTRY",
            "COPY_TYPE", // Funtion type or variable type, int or float
            "COPY_ARRAY_SIZE",
            "COPY_ID",
            "CREATE_PARAMETER_ENTRY",
            "WRITE_PARAMETER_DIMENSION"
        };

bool Semantic::isSemanticAction(const std::string& symbolFromStack)
{
        for (auto symbol : semanticActions)
        {
            if(symbol == symbolFromStack)
            {
                return true;
            }
        }
        return false;
 }

Semantic::~Semantic()
{
    // TODO Auto-generated destructor stub
}


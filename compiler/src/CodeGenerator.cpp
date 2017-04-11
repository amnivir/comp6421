/*
 * CodeGenerator.cpp
 *
 *  Created on: Apr 9, 2017
 *      Author: eshinig
 */
#include <Semantic.h>
#include <CodeGenerator.h>
#include <regex>
#include <fstream>
#include <iostream>
#include <algorithm>    // std::all_of
#include <string>


std::ofstream CodeGenerator::mainfile = {} ;
std::ofstream CodeGenerator::tmpFile = {};
std::string CodeGenerator::outputFileName={};
/*
 * at the initialization all the 14 registers are available
 * r0 , r15 are reserved for offset and jump.
 */
std::map<std::string,bool> CodeGenerator::availRegisters = {
        {"r1",true},
        {"r2",true},
        {"r3",true},
        {"r4",true},
        {"r5",true},
        {"r6",true},
        {"r7",true},
        {"r8",true},
        {"r9",true},
        {"r10",true},
        {"r11",true},
        {"r12",true},
        {"r13",true},
        {"r14",true},
};

int CodeGenerator::labelCounter = 0;

CodeGenerator::CodeGenerator()
{

}

CodeGenerator::~CodeGenerator()
{
    // TODO Auto-generated destructor stub
}

std::string CodeGenerator:: getAvailableRegister()
{
    for( auto r : availRegisters)
    {
        if(r.second == true)
        {
            availRegisters[r.first] = false;
            return r.first;
        }
    }
    return "NONE";
}

void CodeGenerator::createSrcCodeFile(std::string fileName)
{
    std::string fileNameWithEx = fileName.append(".m");
    std::string tmp = "tmpFile";
    mainfile.open (CodeGenerator::outputFileName.c_str());
    tmpFile.open(tmp.c_str());
}

void CodeGenerator::addDirectives()
{
    mainfile<<"org"<<"\t"<<217<<std::endl;
    mainfile<<"align"<<std::endl;
    mainfile<<"entry                  % Start here"<<std::endl;
}

int CodeGenerator::calucalteOffsetForObject(std::string id_type)
{
    using namespace std;
    std::string type;
    std::string arraySize;
    int offset = 0;
    auto table = Semantic::symbolTables[id_type];

    for(auto symbol : table)
    {
        type = symbol.second.type;
        if(type.find("[") != std::string::npos)
        {
            std::size_t n1 = type.find_first_of("[");
            std::size_t n2 = type.find_first_of("]");
            n1 = n1 +1;
            arraySize = type.substr(n1,n2-n1);

            if(type.find("int") != std::string::npos)
            {
                offset = offset + std::stoi( arraySize ) ;
            }

            if(type.find("float") != std::string::npos)
            {
                offset = offset + 2 * std::stoi( arraySize ) ;
            }
        }
        else if (type == "int")
            offset = offset + 1;
        else if (type == "float")
            offset = offset + 2;
    }

    return offset;
}


void CodeGenerator::addDBDirective()
{
    std::string id_type;
    std::string id;
    std::string offset = "0";
    int offsetInt = 0;
    for(auto it : Semantic::semanticStack)
    {
        if(it.first == "type")
            id_type = it.second;
        else if (it.first == "id")
            id = it.second;
        else if (it.first == "arraySize")
            offset = it.second;
    }

    if(id_type=="int")
        offsetInt = std::stoi( offset ) ;

    // Float requires double the memory
    else if(id_type=="float")
        offsetInt = std::stoi( offset ) * 2;

    else if (id_type != "float" && id_type != "int" )
    {
        offsetInt = calucalteOffsetForObject(id_type);
    }

    if(offsetInt == 0)
        tmpFile<<id<<"\t"<<"dw"<<"\t"<<std::to_string(offsetInt)<<std::endl;

    else
        tmpFile<<id<<"\t"<<"res"<<"\t"<<std::to_string(offsetInt)<<std::endl;
}

void CodeGenerator::addDBDirective(std::string label,std::string offset)
{
    tmpFile<<label<<"\t"<<"dw"<<"\t"<<offset<<std::endl;
}

void CodeGenerator::generateCode(std::string statement)
{
    std::string id;
    if(statement == "CODE_GET")
    {
        for(auto it : Semantic::semanticStack)
        {
            if (it.first == "id")
            {
                id = it.second;
                std::string reg = getAvailableRegister();

                mainfile<<"getc"<<"\t"<<reg<<std::endl;
                mainfile<<"sw"<<"\t"<<id<<"(r0),"<<reg<<std::endl;
            }
        }
    }

    else if(statement == "CODE_PUT")
    {
        for(auto it : Semantic::semanticStack)
        {
            if (it.first == "EXPR")
            {
                id = it.second;
                std::string reg = getAvailableRegister();

                mainfile<<"lw"<<"\t"<<reg<<","<<id<<"(r0)"<<endl;
                mainfile<<"putc"<<"\t"<<reg<<std::endl;
            }
        }
    }


}
void CodeGenerator::generateCodeAssignment(std::string left,std::string right)
{
    /*
     * example x = 10;
     */
    if(std::all_of(right.begin(), right.end(), ::isdigit) == true)
    {
        std::string reg = getAvailableRegister();
        mainfile<<"addi"<<"\t"<<reg<<","<<"r0"<<","<<right<<std::endl;
        mainfile<<"sb"<<"\t"<<left<<"(r0)"<<","<<reg<<std::endl;
    }
    /*
     * example x = y;
     */
    else
    {
        std::string reg = getAvailableRegister();
        mainfile<<"lb"<<"\t"<<reg<<","<<right<<"(r0)"<<std::endl;
        mainfile<<"sb"<<"\t"<<left<<"(r0)"<<","<<reg<<std::endl;
    }



}

std::string CodeGenerator::generateCodeArithmetic(std::string left,std::string right,std::string operator_)
{
    std::string reg1 = getAvailableRegister();
    mainfile<<"lw"<<"\t"<<reg1<<","<<left<<"(r0)"<<std::endl;
    //get new register
    std::string reg2 = getAvailableRegister();
    mainfile<<"lw"<<"\t"<<reg2<<","<<right<<"(r0)"<<std::endl;

    std::string reg3 = getAvailableRegister();
    std::string operatorString;
    if(operator_ == "+")
        operatorString = "add";
    else if(operator_ == "-")
        operatorString = "sub";
    else if(operator_ == "*")
        operatorString = "mul";
    else if(operator_ == "/")
        operatorString = "div";

    mainfile<<operatorString<<"\t"<<reg3<<","<<reg1<<","<<reg2<<std::endl;

    std::string label = getLabel();
    addDBDirective(label,"0");

    mainfile<<"sb"<<"\t"<<label<<"(r0)"<<","<<reg3<<std::endl;

    resetRegisters();

    return label;
}

void CodeGenerator::addHaltMergeTmpfile()
{
    tmpFile.close();
    std::ifstream in("tmpFile");
    mainfile<<"hlt"<<std::endl;
    mainfile<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
    for (std::string str; std::getline(in, str); )
    {
        mainfile << str <<std::endl;
    }
    mainfile.close();
    in.close();

}

std::string CodeGenerator::getLabel()
{
    std::string label = "t";
    label = label.append(std::to_string(labelCounter));
    labelCounter++;
    return label;
}

void CodeGenerator::resetRegisters()
{
    for(auto reg : availRegisters)
    {
        if (reg.second == false)
        {
            availRegisters[reg.first] = true;
        }
    }
}
void CodeGenerator::finalizeCodeGeneration()
{
    addHaltMergeTmpfile();
    resetRegisters();
    labelCounter=0;
}

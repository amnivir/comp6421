/*
 * CodeGenerator.cpp
 *
 *  Created on: Apr 9, 2017
 *      Author: eshinig
 */
#include <Semantic.h>
#include <CodeGenerator.h>
#include <fstream>
#include <iostream>


std::ofstream CodeGenerator::mainfile = {} ;
std::ofstream CodeGenerator::tmpFile = {};

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
    mainfile.open (fileNameWithEx.c_str());
    tmpFile.open(tmp.c_str());
}

void CodeGenerator::addDirectives()
{
    mainfile<<"org"<<"\t"<<217<<std::endl;
    mainfile<<"align"<<std::endl;
    mainfile<<"entry                  % Start here"<<std::endl;
}

void CodeGenerator::addDBDirective()
{
    std::string id_type;
    std::string id;
    std::string offset;
    for(auto it : Semantic::semanticStack)
    {
        if(it.first == "type")
            id_type = it.second;
        else if (it.first == "id")
            id = it.second;
    }

    if(id_type=="int")
        offset = "0";

    tmpFile<<id<<"\t"<<"dw"<<"\t"<<offset<<std::endl;
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

void CodeGenerator::addHaltMergeTmpfile()
{
    tmpFile.close();
    std::ifstream in("tmpFile");
    mainfile<<"hlt"<<std::endl;
    mainfile<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<std::endl;
    for (std::string str; std::getline(in, str); )
    {
        mainfile << str;
    }
    mainfile.close();
    in.close();
}

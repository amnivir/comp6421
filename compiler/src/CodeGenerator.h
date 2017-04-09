/*
 * CodeGenerator.h
 *
 *  Created on: Apr 9, 2017
 *      Author: eshinig
 */

#include <string>
#include <map>
#ifndef SRC_CODEGENERATOR_H_
#define SRC_CODEGENERATOR_H_

class CodeGenerator
{
public:
    CodeGenerator();


    virtual ~CodeGenerator();
    /*
     * creates the Moon code file
     */
    static void createSrcCodeFile(std::string fileName);
    /*
     * adds org , align and entry
     */
    static void addDirectives();
    /**
     * add symbols or integers to store in the memory
     */
    static void addDBDirective();

    /*
     * generate code based on statement in semantic stack
     */
    static void generateCode(std::string statement);
    /*
     *This the file where the logic is written
     */
    static std::ofstream mainfile;

    /*
     *This the file where the directive for the varaible is written
     */
    static std::ofstream tmpFile;

    /*
     * add halt in the main code and copy directives from tmp file
     */

    static void addHaltMergeTmpfile();

private:
    static std::map<std::string,bool> availRegisters;
    /*
     * returns the available registers
     */
    static std::string getAvailableRegister();

};

#endif /* SRC_CODEGENERATOR_H_ */

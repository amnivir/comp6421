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
    /**
     * add symbol to store in the memory for a given label and offset
     */
    static void addDBDirective(std::string label,std::string offset);

    /*
     * generate code based on statement in semantic stack
     */
    static void generateCode(std::string statement);

    /*
     * generate code for assignment statements
     * x = y
     */
    static void generateCodeAssignment(std::string left,std::string right);

    /*
     * generate code for assignment statements
     * y + z
     */
    static std::string generateCodeArithmetic(std::string left,std::string right,std::string operator_);
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

    static void finalizeCodeGeneration();

    static  std::string outputFileName;
private:
    static std::map<std::string,bool> availRegisters;
    /*
     * returns the available registers
     */
    static std::string getAvailableRegister();
    /*
     * this will increase the  number value of the id : example t1,t2
     */
    static int labelCounter;

    /*
     * returns the new id
     */
    static std::string getLabel();

    /*
     * Reset all register
     */
    static void resetRegisters();

    /*
     * calculate the offset value for the class object
     */
    static int calucalteOffsetForObject(std::string id_type);

};

#endif /* SRC_CODEGENERATOR_H_ */

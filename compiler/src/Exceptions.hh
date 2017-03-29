/*
 * Exceptions.cc
 *
 *  Created on: Mar 28, 2017
 *      Author: eshinig
 */
#include <iostream>
#include <exception>
using namespace std;

struct SemanticException : public std::exception
{
   std::string s;
   SemanticException(std::string ss) : s(ss) {}
   ~SemanticException() throw () {}
   const char* what() const throw() { return s.c_str(); }
};

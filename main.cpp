#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <list>
#include "AstNode.hpp"
#include <vector>
#include "vsop.tab.h"

extern "C"  int yylex();

#include "Token.hpp"

extern "C" FILE* yyin;

using namespace std;

list<Token> tokens;
string filename;
extern bool lex= false;
extern bool parse =false;
bool check =false;
bool err_lex =false;




int main(int argc, char** argv)
{
	try
	{
		if(argc < 2 | argc > 5)
		{
			std::cout <<"Usage: \
			./vsopc -lex <SOURCE-FILE>    # For lexical analysis\n\
			./vsopc -parse <SOURCE-FILE>  # For syntactic analysis\n\
			./vsopc -check <SOURCE-FILE>  # For semantic analysis\n\
			./vsopc <SOURCE-FILE>         # To compile to a native executable\n";
			return -1;
		}
		
		
		for(int i=1; i < argc-2;i++){
			if(strcmp(argv[i], "-lex") == 0)
				lex = true;
			if(strcmp(argv[1], "-parse") == 0)
				parse=true;
			if(strcmp(argv[1], "-check") == 0)
				check =true;
		}

		FILE* file = fopen(argv[argc-1],"r");
		if(!file)
		{
			std::cerr << "File Opening failed :" << argv[argc-1]  << std::endl;
			return -5;
		}
		filename = argv[2];
		yyin = file;

		if (err_lex)
			return -1;
		yyparse();
	}//end try
	catch(std::runtime_error& e)
	{
		std::cerr << "runtime_error:"  <<e.what() << std::endl;
		return -3;
	}
	catch(std::ios_base::failure& e)
	{
		std::cerr << "ios_based failure:"  << e.what() << std::endl;
		return -4;
	}
	catch(std::exception& e)
	{
		std::cerr << "Other Exception error:" << e.what() << std::endl;
	}

	return 0;
}

void lexical_error(int line, int col, string message="")
{
	cerr << filename << ":" << line << ":" << col << ": lexical error";
	if(message != "")
		cerr << ": " << message;
	cerr << endl;
	err_lex=true;
}

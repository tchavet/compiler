#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <list>
#include <vector>
#include <utility>

#include "nodes/AstNode.hpp"
#include "nodes/AssignNode.hpp"
#include "nodes/BinOpNode.hpp"
#include "nodes/BlockNode.hpp"
#include "nodes/BoolLitNode.hpp"
#include "nodes/CallNode.hpp"
#include "nodes/ClassNode.hpp"
#include "nodes/FieldNode.hpp"
#include "nodes/FormalNode.hpp"
#include "nodes/IfNode.hpp"
#include "nodes/IntLitNode.hpp"
#include "nodes/LetNode.hpp"
#include "nodes/MethodNode.hpp"
#include "nodes/NewNode.hpp"
#include "nodes/ObjectIdNode.hpp"
#include "nodes/ProgramNode.hpp"
#include "nodes/StringLitNode.hpp"
#include "nodes/UnOpNode.hpp"
#include "nodes/WhileNode.hpp"

#include "semantic/Semantic.hpp"

#include "vsop.tab.h"

extern "C"  int yylex();

extern "C" FILE* yyin;

using namespace std;

string filename;
ProgramNode* root= new ProgramNode();
bool lex= false;
bool parse =false;
bool check =false;
bool err_lex =false;
bool err_parse = false;

int main(int argc, char** argv)
{
	bool err_sem = false;
	try
	{
		if(argc < 2 | argc > 5)
		{
			std::cout <<"Usage: \
			./vsopc [-lex] [-parse] [-check] <SOURCE-FILE> \n\
			# For lexical analysis use -lex\n\
			# For syntactic analysis -parse\n\
			# For semantic analysis -check\n";

			return -1;
		}
		
		
		for(int i=1; i <= argc-2;i++){
			if(strcmp(argv[i], "-lex") == 0)
				lex = true;
			if(strcmp(argv[i], "-parse") == 0)
				parse=true;
			if(strcmp(argv[i], "-check") == 0)
				check =true;
		}

		filename = argv[argc-1];
		FILE* file = fopen(filename.c_str(),"r");
		if(!file)
		{
			std::cerr << "File Opening failed :" << argv[argc-1]  << std::endl;
			return -5;
		}
		yyin = file;

		yyparse();

		if (parse)
		{
			cout<<root->printTree()<<endl;
		}

		if (check || !lex && !parse && !check)
		{
			Semantic semantic(filename, root);
			err_sem = semantic.classesCheck();
			if (!err_sem)
			{
				err_sem = semantic.scopeCheck();
			}
		}

		if (check)
			cout<<root->printTree(0,true)<<endl;
	}//end try
	catch(std::runtime_error& e)
	{
		std::cerr << "runtime_error:"  <<e.what() << std::endl;
		return -3;
	}
	catch(std::exception& e)
	{
		std::cerr << "Other Exception error:" << e.what() << std::endl;
		return -4;
	}

	if (err_parse)
		return -6;

	if (err_sem)
		return -7;

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

void syntax_error(int line, int col, string message="")
{
	cerr << filename << ":" << line << ":" << col << ": syntax error";
	if(message != "")
		cerr << ": " << message;
	cerr << endl;
	err_parse=true;
}

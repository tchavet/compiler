#include <cstddef>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <list>
#include <vector>
#include <utility>
#include <string>
#include <limits.h>
#include <unistd.h>

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
#include "vsopl/Vsopl.hpp"

#include "vsop.tab.h"

extern "C"  int yylex();

extern "C" FILE* yyin;

using namespace std;

std::string getCurrentPath();

std::string filename;
std::string programName;
ProgramNode* root= new ProgramNode();
bool lex= false;
bool parse =false;
bool check =false;
bool err_lex =false;
bool err_parse = false;
bool completeCompile = false;
bool llvm =false;

int main(int argc, char** argv)
{
	bool err_sem = false;
	bool emitLlvm = false;
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
		
		/****************************************
		 *Retrieve the arguments of the function*
		 ****************************************/
		
		for(int i=1; i <= argc-2;i++){
			if(strcmp(argv[i], "-lex") == 0)
				lex = true;
			if(strcmp(argv[i], "-parse") == 0)
				parse=true;
			if(strcmp(argv[i], "-check") == 0)
				check =true;
			if(strcmp(argv[i], "-llvm") == 0)
				llvm =true;
			if(strcmp(argv[i], "-emit-llvm") == 0)
				emitLlvm =true;
		}
		if(argc == 2)
			completeCompile = true;

		/****************************************
		 *Open input file						*
		 ****************************************/
		filename = argv[argc-1];
		FILE* file = fopen(filename.c_str(),"r");
		if(!file)
		{
			std::cerr << "File Opening failed :" << argv[argc-1]  << std::endl;
			return -5;
		}

		/****************************************
		 *Define output file name				*
		 ****************************************/
		programName = filename.substr(filename.find_last_of("/")+1);
		programName = programName.substr(0, programName.find_last_of("."));

		/****************************************
		 *Configure lex and parse				*
		 ****************************************/
		
		yyin = file;

		
		/****************************************
		 *Perform the parsing and lexing		*
		 ****************************************/

		yyparse();


		/****************************************
		 *Prints the Tree if required			*
		 ****************************************/

		if (parse)
		{
			cout<<root->printTree()<<endl;
		}
		/****************************************
		 *Print errors							*
		 ****************************************/
		if (err_parse)
			return -6;

		/****************************************
		 *If parse finished, stop program		*
		 ****************************************/
		if(parse || lex)
			return 0;

		

		/****************************************
		 *Semantic testing						*
		 ****************************************/
		Vsopl::init();

		Semantic semantic(filename, root);
		err_sem = semantic.classesCheck();
		if (!err_sem)
		{
			err_sem = semantic.scopeCheck();
		}
		//print tree
		if (check)
			cout<<root->printTree(0,true)<<endl;
		//print errors
		if (err_sem)
			return -7;

		/****************************************
		 *If end check, stop program			*
		 ****************************************/
		if(check)
			return 0;

		
		/****************************************
		 *Llvm code generation					*
		 ****************************************/
		std::ofstream llvmFile;
		//open file
		llvmFile.open(programName+".ll");
		//creates our manager
		LlvmManager *llvmManager = new LlvmManager(&llvmFile);
		//Writes the Header Down
		Vsopl::llvmHeader(llvmManager);
		//declares classes and methodds
		root->llvmHeader(llvmManager);
		//Writes the main
		llvmManager->main();
		//Write the code down
		root->llvm(llvmManager);
		llvmManager->writeConstants();
		llvmFile.close();
		//if(llvm)
		//{
		//	std::cout << "The fileName of the generated file is "<< programName << ".ll" << std::endl;
		//	return 0;
		//}
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
	std::string vsopLib = getCurrentPath();
	vsopLib = vsopLib.substr(0, vsopLib.find_last_of("/"));
	vsopLib +="/vsopl/vsopl.ll";
	std::string command;
	
	if(llvm)
	{
		command = "llvm-link -S "+vsopLib+" "+programName+".ll";
		system(command.c_str());
	}
	else 
	{
		command = "llvm-link -S "+vsopLib+" "+programName+".ll -o "+programName+"_linked.ll";
		if(system(command.c_str()) != 0)
			return -8;

		command = "llc -o "+programName+".s "+programName+"_linked.ll";
		if(system(command.c_str()) != 0)
			return -8;
	
		command = "gcc "+programName+".s -o"+programName+" -lm";
		if(system(command.c_str()) != 0)
			return -8;
	
		command = "rm "+programName+".s";
		if(system(command.c_str()) != 0)
			return -8;
	
		command = "rm "+programName+"_linked.ll";
		if(system(command.c_str()) != 0)
			return -8;
	}

	if (!emitLlvm)
	{
		command = "rm "+programName+".ll";
		system(command.c_str());
	}

	//std::cout << "Your program is called "<< programName <<std::endl; 
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

std::string getCurrentPath()
{
	char result[ PATH_MAX ];
	ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
	return std::string( result, (count > 0) ? count : 0 );
}

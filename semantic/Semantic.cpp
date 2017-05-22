#include "Semantic.hpp"
#include "../nodes/ClassNode.hpp"
#include "../vsopl/Vsopl.hpp"
#include <iostream>

Semantic::Semantic(std::string filename, ProgramNode* tree)
{
	this->filename = filename;
	astTree = tree;

	std::vector<std::string> builtIns;
	builtIns.push_back("int32");
	builtIns.push_back("bool");
	builtIns.push_back("string");
	builtIns.push_back("unit");
	Types::init(builtIns);
	Vsopl::semantic();
}

bool Semantic::classesCheck()
{
	bool error = false;
	std::vector<ClassNode*> classes = astTree->getClasses();

	/* Check if each class hasn't already been defined */
	for (int i=0; i<classes.size(); i++)
	{
		if (!Types::add(classes[i]->getName(), classes[i]))
		{
			semanticError(classes[i]->getLine(), classes[i]->getColumn(), "class " + classes[i]->getName() + " already exists");
			error = true;
		}
	}

	/* Check if each parent is defined */
	for (int i=0; i<classes.size(); i++)
	{
		if (!Types::defined(classes[i]->getParentName()))
		{
			semanticError(classes[i]->getLine(), classes[i]->getColumn(), "parent class " + classes[i]->getParentName() + " undefined");
			error = true;
		}
		else
			classes[i]->setParentNode(Types::getNode(classes[i]->getParentName()));
	}

	/* Check inheritance cycles */
	for (int i=0; i<classes.size(); i++)
	{
		if (classes[i]->getParentNode() && classes[i]->getParentNode()->isA(classes[i]->getName()))
		{
			semanticError(classes[i]->getLine(), classes[i]->getColumn(), "class " + classes[i]->getName() + " cannot extend " + classes[i]->getParentName() + ": inheritance cycle");
			error = true;
		}
	}

	/* Check main class */
	if (!Types::defined("Main"))
	{
			semanticError(0,0, "program contains no Main class");
			error = true;
	}
	else
	{
		ClassNode* mainNode = Types::getNode("Main");
		MethodNode* mainMethod = mainNode->getMethod("main");
		if (!mainMethod)
		{
			semanticError(mainNode->getLine(), mainNode->getColumn(), "class Main has no main method");
			error = true;
		}
		else
		{
			if (mainMethod->getReturnType() != "int32")
			{
				semanticError(mainNode->getLine(), mainNode->getColumn(), "method main has return type " + mainMethod->getReturnType() + " but should return int32");
				error = true;
			}
			if (mainMethod->getParams().size() != 0)
			{
				semanticError(mainNode->getLine(), mainNode->getColumn(), "main method should have no arguments");
				error = true;
			}
		}
	}

	return error;
}

bool Semantic::scopeCheck()
{
	std::vector<SemErr*> errors = astTree->semCheck();
	if (errors.size() == 0)
		return false;
	for (int i=0; i<errors.size(); i++)
	{
		semanticError(errors[i]->line, errors[i]->column, errors[i]->errStr);
	}
	return true;
}

void Semantic::semanticError(int line, int column, std::string message)
{
	std::cerr << filename << ":" << line << ":" << column << ": semantic error";
	if(message != "")
		std::cerr << ": " << message;
	std::cerr << std::endl;
}

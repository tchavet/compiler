#include "Semantic.hpp"
#include "../nodes/ClassNode.hpp"
#include "../vsopl/IOClass.hpp"
#include <iostream>

Semantic::Semantic(std::string filename, ProgramNode* tree)
{
	this->filename = filename;
	astTree = tree;
	rootScope = new Scope();
	currentScope = rootScope;

	std::vector<std::string> builtIns;
	builtIns.push_back("int32");
	builtIns.push_back("bool");
	builtIns.push_back("string");
	builtIns.push_back("unit");
	builtIns.push_back("Object");
	Types::init(builtIns);
	IOClass io;
	Types::add("IO", io.getClassNode());
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
}

bool Semantic::typeCheck()
{
}

void Semantic::semanticError(int line, int column, std::string message)
{
	std::cerr << filename << ":" << line << ":" << column << ": semantic error";
	if(message != "")
		std::cerr << ": " << message;
	std::cerr << std::endl;
}

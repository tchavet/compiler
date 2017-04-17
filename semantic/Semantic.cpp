#include "Semantic.hpp"
#include "../nodes/ClassNode.hpp"
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
	types = Types(builtIns);
}

bool Semantic::classesCheck()
{
	bool error = false;
	std::vector<ClassNode*> classes = astTree->getClasses();
	for (int i=0; i<classes.size(); i++)
	{
		if (!types.defined(classes[i]->getParent()))
		{
			semanticError(classes[i]->getLine(), classes[i]->getColumn(), "parent class " + classes[i]->getParent() + " undefined");
			error = true;
		}

		if (types.defined(classes[i]->getName()))
		{
			semanticError(classes[i]->getLine(), classes[i]->getColumn(), "class " + classes[i]->getName() + " already exists");
			error = true;
		}
		else
			types.add(classes[i]->getName(), classes[i]);
	}
	return error;
}

bool Semantic::scopeCheck()
{

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

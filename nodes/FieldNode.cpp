#include "FieldNode.hpp"
#include "../semantic/Types.hpp"

FieldNode::FieldNode(int line, int column, std::string name, std::string type, ExprNode* init) : AstNode(line, column)
{
	this->name = name;
	this->type = type;
	this->init = init;
	if (init)
		init->setParent(this);
}

std::string FieldNode::printTree(int tabsNb, bool types)
{
	std::string print = "Field(" + name + "," + type;
	if (init)
	{
		print += ",\n"
			+ tabs(tabsNb+1) + init->printTree(tabsNb+1,types) + "\n"
			+ tabs(tabsNb);
	}
	print += ")";
	return print;
}

std::string FieldNode::getName()
{
	return name;
}

std::string FieldNode::getType()
{
	return type;
}

std::vector<SemErr*> FieldNode::semCheck()
{
	std::vector<SemErr*> semErr;
	std::string typeInScope = getTypeInScope(name);
	if (typeInScope != type)
	{
		semErr.push_back(new SemErr(line, column, "field " + name + " has already been defined as a " + typeInScope));
	}
	
	if (init)
	{
		ExprType* initType = init->getType();
		if (initType->error)
			semErr.insert(semErr.end(),initType->errors.begin(),initType->errors.end());

		if (initType->type != "" && initType->type != type)
		{
			ClassNode* initClass = Types::getNode(initType->type);
 
			if(!(initClass && initClass->isA(type)))
			{
				semErr.push_back(new SemErr(line, column, name + " defined as type " + type + ", but assignement expression is of type " + initType->type));
			}
		}
	}
	return semErr;
}

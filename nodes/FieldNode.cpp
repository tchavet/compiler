#include "FieldNode.hpp"

FieldNode::FieldNode(int line, int column, std::string name, std::string type, AstNode* init) : AstNode(line, column)
{
	this->name = name;
	this->type = type;
	this->init = init;
	if (init)
		init->setParent(this);
}

std::string FieldNode::printTree(int tabsNb)
{
	std::string print = "Field(" + name + "," + type;
	if (init)
	{
		print += ",\n"
			+ tabs(tabsNb+1) + init->printTree(tabsNb+1) + "\n"
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
	return semErr;
}

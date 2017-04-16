#include "FieldNode.hpp"

FieldNode::FieldNode(int line, int column, std::string name, std::string type, AstNode* init)
{
	AstNode(line, column);
	this->name = name;
	this->type = type;
	this->init = init;
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

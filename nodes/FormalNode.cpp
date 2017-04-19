#include "FormalNode.hpp"

FormalNode::FormalNode(int line, int column, std::string name, std::string type) : AstNode(line, column)
{
	this->name = name;
	this->type = type;
}

std::string FormalNode::printTree(int tabsNb, bool types)
{
	return name + " : " + type;
}

std::string FormalNode::getName()
{
	return name;
}

std::string FormalNode::getType()
{
	return type;
}

#include "ObjectIdNode.hpp"

ObjectIdNode::ObjectIdNode(int line, int column, std::string name) : AstNode(line, column)
{
	this->name = name;
}

std::string ObjectIdNode::printTree(int tabsNb)
{
	return name;
}

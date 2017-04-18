#include "ObjectIdNode.hpp"

ObjectIdNode::ObjectIdNode(int line, int column, std::string name) : ExprNode(line, column)
{
	this->name = name;
}

std::string ObjectIdNode::printTree(int tabsNb)
{
	return name;
}

ExprType* ObjectIdNode::getType()
{
}

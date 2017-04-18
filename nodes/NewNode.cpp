#include "NewNode.hpp"

NewNode::NewNode(int line, int column, std::string type) : ExprNode(line, column)
{
	this->type = type;
}

std::string NewNode::printTree(int tabsNb)
{
	return "New(" + type + ")";
}

ExprType* NewNode::getType()
{
}

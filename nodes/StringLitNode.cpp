#include "StringLitNode.hpp"

StringLitNode::StringLitNode(int line, int column, std::string str) : ExprNode(line, column)
{
	this->str = str;
}

std::string StringLitNode::printTree(int tabsNb)
{
	return "\"" + str + "\"";
}

ExprType* StringLitNode::getType()
{
}

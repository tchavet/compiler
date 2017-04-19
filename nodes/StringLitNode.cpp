#include "StringLitNode.hpp"

StringLitNode::StringLitNode(int line, int column, std::string str) : ExprNode(line, column)
{
	this->str = str;
}

std::string StringLitNode::printTree(int tabsNb, bool types)
{
	return "\"" + str + "\"" + (types ? " : " + type : "");
}

ExprType* StringLitNode::getType()
{
	type = "string";
	return new ExprType("string");
}

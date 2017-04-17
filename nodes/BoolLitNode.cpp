#include "BoolLitNode.hpp"

BoolLitNode::BoolLitNode(int line, int column, std::string value) : AstNode(line, column)
{
	this->value = value;
}

std::string BoolLitNode::printTree(int tabsNb)
{
	return value;
}

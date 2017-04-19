#include "BoolLitNode.hpp"

BoolLitNode::BoolLitNode(int line, int column, std::string value) : ExprNode(line, column)
{
	this->value = value;
}

std::string BoolLitNode::printTree(int tabsNb)
{
	return value;
}

ExprType* BoolLitNode::getType()
{
	ExprType* exprType = new ExprType("bool");
	type = "bool";
	return exprType;
}

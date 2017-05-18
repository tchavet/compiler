#include "ExprNode.hpp"

ExprNode::ExprNode(int line, int column) : AstNode(line, column)
{
	type = "unit";
}

ExprType* ExprNode::getType()
{
	return new ExprType("unit");
}

std::string ExprNode::printTree(int tabsNb, bool types)
{
	return std::string("()") + (types ? " : unit" : "");
}

std::string ExprNode::getComputedType()
{
	return type;
}

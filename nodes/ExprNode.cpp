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

std::string ExprNode::getLlvmVariable(std::string var, LlvmManager* manager, bool pointer)
{
	if (parent)
		return parent->getLlvmVariable(var, manager, pointer);
	else
		return "";
}

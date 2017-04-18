#include "AssignNode.hpp"

AssignNode::AssignNode(int line, int column, std::string name, ExprNode* expr) : ExprNode(line, column)
{
	this->name = name;
	this->expr = expr;
}

std::string AssignNode::printTree(int tabsNb)
{
	return "Assign(" + name + ",\n"
		+ tabs(tabsNb+1) + expr->printTree(tabsNb+1) + "\n"
		+ tabs(tabsNb) + ")";
}

ExprType* AssignNode::getType()
{
}

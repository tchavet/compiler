#include "UnOpNode.hpp"

UnOpNode::UnOpNode(int line, int column, std::string op, ExprNode* expr) : ExprNode(line, column)
{
	this->op = op;
	this->expr = expr;
}

std::string UnOpNode::printTree(int tabsNb)
{
	return "UnOp(" + op + ",\n"
		+ tabs(tabsNb+1) + expr->printTree(tabsNb+1) + "\n"
		+ tabs(tabsNb) + ")";
}

ExprType* UnOpNode::getType()
{
}

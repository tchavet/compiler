#include "BinOpNode.hpp"

BinOpNode::BinOpNode(int line, int column, std::string op, ExprNode* leftExpr, ExprNode* rightExpr) : ExprNode(line, column)
{
	this->op = op;
	this->leftExpr = leftExpr;
	this->rightExpr = rightExpr;
}

std::string BinOpNode::printTree(int tabsNb)
{
	return "BinOp(" + op + ",\n"
		+ tabs(tabsNb+1) + leftExpr->printTree(tabsNb+1) + ",\n"
		+ tabs(tabsNb+1) + rightExpr->printTree(tabsNb+1) + "\n"
		+ tabs(tabsNb) + ")";
}

ExprType* BinOpNode::getType()
{
}

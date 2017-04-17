#include "BinOpNode.hpp"

BinOpNode::BinOpNode(int line, int column, std::string op, AstNode* leftExpr, AstNode* rightExpr) : AstNode(line, column)
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

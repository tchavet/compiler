#include "WhileNode.hpp"

WhileNode::WhileNode(int line, int column, ExprNode* cond, ExprNode* body) : ExprNode(line, column)
{
	this->cond = cond;
	this->body = body;
}

std::string WhileNode::printTree(int tabsNb)
{
	return "While(\n"
		+ tabs(tabsNb+1) + cond->printTree(tabsNb+1) + ",\n"
		+ tabs(tabsNb+1) + body->printTree(tabsNb+1) + "\n"
		+ tabs(tabsNb) + ")";
}

ExprType* WhileNode::getType()
{
}

#include "BlockNode.hpp"

BlockNode::BlockNode(int line, int column) : ExprNode(line, column)
{
	expressions = std::vector<ExprNode*>();
}

void BlockNode::addExpression(ExprNode* expression)
{
	expressions.push_back(expression);
}

void BlockNode::addExpressions(std::vector<ExprNode*>* expressions)
{
	this->expressions.insert(this->expressions.end(), expressions->begin(), expressions->end());
}

std::string BlockNode::printTree(int tabsNb)
{
	return printList<ExprNode>(tabsNb, expressions);
}

ExprType* BlockNode::getType()
{
}

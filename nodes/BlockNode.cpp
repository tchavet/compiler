#include "BlockNode.hpp"

BlockNode::BlockNode(int line, int column)
{
	AstNode(line, column);
	expressions = std::vector<AstNode*>();
}

void BlockNode::addExpression(AstNode* expression)
{
	expressions.push_back(expression);
}

void BlockNode::addExpressions(std::vector<AstNode*>* expressions)
{
	this->expressions.insert(this->expressions.end(), expressions->begin(), expressions->end());
}

std::string BlockNode::printTree(int tabsNb)
{
	return printList<AstNode>(tabsNb, expressions);
}

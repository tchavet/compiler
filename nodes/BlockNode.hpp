#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "ExprNode.hpp"

class BlockNode : public ExprNode
{
public:
	BlockNode(int line, int column);
	std::string printTree(int tabsNb=0, bool types=false);
	void addExpression(ExprNode* expression);
	void addExpressions(std::vector<ExprNode*>* expressions);
	ExprType* getType();

protected:
	std::vector<ExprNode*> expressions;
};
#endif

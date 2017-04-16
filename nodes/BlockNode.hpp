#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "AstNode.hpp"

class BlockNode : public AstNode
{
public:
	BlockNode(int line, int column);
	std::string printTree(int tabsNb=0);
	void addExpression(AstNode* expression);
	void addExpressions(std::vector<AstNode*>* expressions);

protected:
	std::vector<AstNode*> expressions;
};
#endif

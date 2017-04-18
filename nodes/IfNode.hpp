#ifndef IFNODE_H
#define IFNODE_H

#include "ExprNode.hpp"

class IfNode : public ExprNode
{
public:
	IfNode(int line, int column, ExprNode* cond, ExprNode* then, ExprNode* els = NULL);
	std::string printTree(int tabsNb=0);
	ExprType* getType();

protected:
	ExprNode* cond;
	ExprNode* then;
	ExprNode* els;
};
#endif

#ifndef IFNODE_H
#define IFNODE_H

#include "AstNode.hpp"

class IfNode : public AstNode
{
public:
	IfNode(int line, int column, AstNode* cond, AstNode* then, AstNode* els = NULL);
	std::string printTree(int tabsNb=0);

protected:
	AstNode* cond;
	AstNode* then;
	AstNode* els;
};
#endif

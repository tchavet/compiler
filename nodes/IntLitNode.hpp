#ifndef INTLITNODE_H
#define INTLITNODE_H

#include "AstNode.hpp"

class IntLitNode : public AstNode
{
public:
	IntLitNode(int line, int column, int value);
	std::string printTree(int tabsNb=0);

protected:
	int value;
};
#endif

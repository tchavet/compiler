#ifndef STRINGLITNODE_H
#define STRINGLITNODE_H

#include "ExprNode.hpp"

class StringLitNode : public ExprNode
{
public:
	StringLitNode(int line, int column, std::string str);
	std::string printTree(int tabsNb=0);
	ExprType* getType();

protected:
	std::string str;
};
#endif

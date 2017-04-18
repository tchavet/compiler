#ifndef BOOLLITNODE_H
#define BOOLLITNODE_H

#include "ExprNode.hpp"

class BoolLitNode : public ExprNode
{
public:
	BoolLitNode(int line, int column, std::string value);
	std::string printTree(int tabsNb=0);
	ExprType* getType();

protected:
	std::string value;
};
#endif

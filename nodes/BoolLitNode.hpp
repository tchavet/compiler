#ifndef BOOLLITNODE_H
#define BOOLLITNODE_H

#include "AstNode.hpp"

class BoolLitNode : public AstNode
{
public:
	BoolLitNode(int line, int column, std::string value);
	std::string printTree(int tabsNb=0);

protected:
	std::string value;
};
#endif

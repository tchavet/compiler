#ifndef NEWNODE_H
#define NEWNODE_H

#include "ExprNode.hpp"

class NewNode : public ExprNode
{
public:
	NewNode(int line, int column, std::string type);
	std::string printTree(int tabsNb=0);
	ExprType* getType();

protected:
	std::string type;
};
#endif

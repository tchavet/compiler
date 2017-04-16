#ifndef NEWNODE_H
#define NEWNODE_H

#include "AstNode.hpp"

class NewNode : public AstNode
{
public:
	NewNode(int line, int column, std::string type);
	std::string printTree(int tabsNb=0);

protected:
	std::string type;
};
#endif

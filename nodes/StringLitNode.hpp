#ifndef STRINGLITNODE_H
#define STRINGLITNODE_H

#include "AstNode.hpp"

class StringLitNode : public AstNode
{
public:
	StringLitNode(int line, int column, std::string str);
	std::string printTree(int tabsNb=0);

protected:
	std::string str;
};
#endif

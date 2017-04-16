#ifndef ASSIGNNODE_H
#define ASSIGNNODE_H

#include "AstNode.hpp"

class AssignNode: public AstNode
{
public:
	AssignNode(int line, int column, std::string name, AstNode* expr);
	std::string printTree(int tabsNb=0);

protected:
	std::string name;
	AstNode* expr;
};
#endif

#ifndef UNOPNODE_H
#define UNOPNODE_H

#include "AstNode.hpp"

class UnOpNode : public AstNode
{
public:
	UnOpNode(int line, int column, std::string op, AstNode* expr);
	std::string printTree(int tabsNb=0);

protected:
	std::string op;
	AstNode* expr;
};
#endif

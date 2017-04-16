#ifndef BINOPNODE_H
#define BINOPNODE_H

#include "AstNode.hpp"

class BinOpNode : public AstNode
{
public:
	BinOpNode(int line, int column, std::string op, AstNode* leftExpr, AstNode* rightExpr);
	std::string printTree(int tabsNb=0);

protected:
	std::string op;
	AstNode* leftExpr;
	AstNode* rightExpr;
};
#endif

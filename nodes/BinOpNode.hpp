#ifndef BINOPNODE_H
#define BINOPNODE_H

#include "ExprNode.hpp"

class BinOpNode : public ExprNode
{
public:
	BinOpNode(int line, int column, std::string op, ExprNode* leftExpr, ExprNode* rightExpr);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();

protected:
	std::string op;
	ExprNode* leftExpr;
	ExprNode* rightExpr;
};
#endif

#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "AstNode.hpp"
#include "../semantic/structures.hpp"

class ExprNode : public AstNode
{
public:
	ExprNode(int line=0, int column=0);
	virtual ExprType* getType() {return new ExprType;};
	virtual std::string printTree(int tabsNb=0) {};

protected:
	std::string type;
};
#endif

#ifndef UNOPNODE_H
#define UNOPNODE_H

#include "ExprNode.hpp"

class UnOpNode : public ExprNode
{
public:
	UnOpNode(int line, int column, std::string op, ExprNode* expr);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	std::string llvm(LlvmManager* manager);

protected:
	std::string op;
	ExprNode* expr;
};
#endif

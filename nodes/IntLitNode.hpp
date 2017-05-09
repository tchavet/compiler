#ifndef INTLITNODE_H
#define INTLITNODE_H

#include "ExprNode.hpp"

class IntLitNode : public ExprNode
{
public:
	IntLitNode(int line, int column, int value);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	std::string llvm(LlvmManager* manager);
protected:
	int value;
};
#endif

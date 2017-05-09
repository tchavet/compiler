#ifndef BOOLLITNODE_H
#define BOOLLITNODE_H

#include "ExprNode.hpp"

class BoolLitNode : public ExprNode
{
public:
	BoolLitNode(int line, int column, std::string value);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	std::string llvm(LlvmManager* manager);

protected:
	std::string value;
};
#endif

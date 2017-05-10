#ifndef STRINGLITNODE_H
#define STRINGLITNODE_H

#include "ExprNode.hpp"

class StringLitNode : public ExprNode
{
public:
	StringLitNode(int line, int column, std::string str);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	std::string llvm(LlvmManager* manager);
protected:
	std::string str;
};
#endif

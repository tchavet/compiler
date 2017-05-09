#ifndef NEWNODE_H
#define NEWNODE_H

#include "ExprNode.hpp"

class NewNode : public ExprNode
{
public:
	NewNode(int line, int column, std::string type);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	std::string llvm(LlvmManager* manager);

protected:
	std::string type;
};
#endif

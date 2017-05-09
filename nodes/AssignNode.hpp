#ifndef ASSIGNNODE_H
#define ASSIGNNODE_H

#include "ExprNode.hpp"

class AssignNode: public ExprNode
{
public:
	AssignNode(int line, int column, std::string name, ExprNode* expr);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	std::string llvm(LlvmManager* manager);

protected:
	std::string name;
	ExprNode* expr;
};
#endif

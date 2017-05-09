#ifndef CALLNODE_H
#define CALLNODE_H

#include "ExprNode.hpp"

class CallNode : public ExprNode
{
public:
	CallNode(int line, int column, ExprNode* objExpr, std::string name, std::vector<ExprNode*> args);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	std::string llvm(LlvmManager* manager);

protected:
	ExprNode* objExpr;
	std::string name;
	std::vector<ExprNode*> args;
};
#endif

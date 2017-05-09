#ifndef WHILENODE_H
#define WHILENODE_H

#include "ExprNode.hpp"

class WhileNode : public ExprNode
{
public:
	WhileNode(int line, int column, ExprNode* cond, ExprNode* body);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	void llvm(LlvManager* manager);

protected:
	ExprNode* cond;
	ExprNode* body;
};
#endif

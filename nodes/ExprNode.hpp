#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "AstNode.hpp"
#include "../semantic/structures.hpp"

class ExprNode : public AstNode
{
public:
	ExprNode(int line=0, int column=0);
	virtual ExprType* getType();
	virtual std::string printTree(int tabsNb=0, bool types=false);
	virtual std::string llvm(LlvmManager* manager);
	virtual void setLlvmNameInScope(std::string var, std::string llvmName);
	virtual std::string getLlvmNameInScope(std::string var);
	std::string getLlvmType();

protected:
	std::string type;
};
#endif

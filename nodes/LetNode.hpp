#ifndef LETNODE_H
#define LETNODE_H

#include "ExprNode.hpp"

class LetNode : public ExprNode
{
public:
	LetNode(int line, int column, std::string name, std::string type, ExprNode* scope, ExprNode* init=NULL);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();
	std::string getTypeInScope(std::string id);
	void setLlvmNameInScope(std::string var, std::string llvmName);
	std::string getLlvmNameInScope(std::string var);
	std::string llvm(LlvmManager* manager);
protected:
	std::string name;
	std::string letType;
	ExprNode* init;
	ExprNode* scope;
	std::string llvmName;
};
#endif

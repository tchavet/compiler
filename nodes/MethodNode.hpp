#ifndef METHODNODE_H
#define METHODNODE_H

#include "AstNode.hpp"
#include "FormalNode.hpp"
#include "BlockNode.hpp"
#include "../semantic/structures.hpp"

class MethodNode : public AstNode
{
public:
	MethodNode(int line, int column, std::string name, std::vector<FormalNode*> params, std::string returnType, BlockNode* body);
	std::string printTree(int tabsNb=0, bool types=false);
	std::string getTypeInScope(std::string id);
	std::string getName();
	std::vector<FormalNode*> getParams();
	std::vector<SemErr*> semCheck();
	std::string getReturnType();
	void setLlvmNameInScope(std::string var, std::string llvmName);
	std::string getLlvmNameInScope(std::string var);

protected:
	std::string name;
	std::vector<FormalNode*> params;
	std::string returnType;
	BlockNode* body;
};
#endif

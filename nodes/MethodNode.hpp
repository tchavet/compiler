#ifndef METHODNODE_H
#define METHODNODE_H

#include "AstNode.hpp"
#include "FormalNode.hpp"
#include "BlockNode.hpp"

class MethodNode : public AstNode
{
public:
	MethodNode(int line, int column, std::string name, std::vector<FormalNode*> params, std::string returnType, BlockNode* body);
	std::string printTree(int tabsNb=0);

protected:
	std::string name;
	std::vector<FormalNode*> params;
	std::string returnType;
	BlockNode* body;
};
#endif

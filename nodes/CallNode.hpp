#ifndef CALLNODE_H
#define CALLNODE_H

#include "AstNode.hpp"

class CallNode : public AstNode
{
public:
	CallNode(int line, int column, AstNode* objExpr, std::string name, std::vector<AstNode*> args);
	std::string printTree(int tabsNb=0);

protected:
	AstNode* objExpr;
	std::string name;
	std::vector<AstNode*> args;
};
#endif

#ifndef WHILENODE_H
#define WHILENODE_H

#include "AstNode.hpp"

class WhileNode : public AstNode
{
public:
	WhileNode(int line, int column, AstNode* cond, AstNode* body);
	std::string printTree(int tabsNb=0);

protected:
	AstNode* cond;
	AstNode* body;
};
#endif

#ifndef LETNODE_H
#define LETNODE_H

#include "AstNode.hpp"

class LetNode : public AstNode
{
public:
	LetNode(int line, int column, std::string name, std::string type, AstNode* scope, AstNode* init=NULL);
	std::string printTree(int tabsNb=0);

protected:
	std::string name;
	std::string type;
	AstNode* init;
	AstNode* scope;
};
#endif

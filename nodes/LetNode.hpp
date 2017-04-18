#ifndef LETNODE_H
#define LETNODE_H

#include "ExprNode.hpp"

class LetNode : public ExprNode
{
public:
	LetNode(int line, int column, std::string name, std::string type, ExprNode* scope, ExprNode* init=NULL);
	std::string printTree(int tabsNb=0);
	ExprType* getType();

protected:
	std::string name;
	std::string type;
	ExprNode* init;
	ExprNode* scope;
};
#endif

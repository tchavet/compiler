#ifndef OBJECTIDNODE_H
#define OBJECTIDNODE_H

#include "ExprNode.hpp"

class ObjectIdNode : public ExprNode
{
public:
	ObjectIdNode(int line, int column, std::string name);
	std::string printTree(int tabsNb=0);
	ExprType* getType();

protected:
	std::string name;
};
#endif

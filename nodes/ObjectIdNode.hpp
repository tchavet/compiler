#ifndef OBJECTIDNODE_H
#define OBJECTIDNODE_H

#include "ExprNode.hpp"

class ObjectIdNode : public ExprNode
{
public:
	ObjectIdNode(int line, int column, std::string name);
	std::string printTree(int tabsNb=0, bool types=false);
	ExprType* getType();

protected:
	std::string name;
};
#endif

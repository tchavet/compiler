#ifndef OBJECTIDNODE_H
#define OBJECTIDNODE_H

#include "AstNode.hpp"

class ObjectIdNode : public AstNode
{
public:
	ObjectIdNode(int line, int column, std::string name);
	std::string printTree(int tabsNb=0);

protected:
	std::string name;
};
#endif

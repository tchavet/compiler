#ifndef FORMALNODE_H
#define FORMALNODE_H

#include "AstNode.hpp"

class FormalNode : public AstNode
{
public:
	FormalNode(int line, int column, std::string name, std::string type);
	std::string printTree(int tabsNb=0);
	std::string getName();
	std::string getType();

protected:
	std::string name;
	std::string type;
};
#endif

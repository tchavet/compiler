#ifndef FIELDNODE_H
#define FIELDNODE_H

#include "AstNode.hpp"
#include "../semantic/structures.hpp"

class FieldNode : public AstNode
{
public:
	FieldNode(int line, int column, std::string name, std::string type, AstNode* init=NULL);
	std::string printTree(int tabsNb=0, bool types=false);
	std::string getName();
	std::string getType();
	std::vector<SemErr*> semCheck();

protected:
	std::string name;
	std::string type;
	AstNode* init;
};
#endif

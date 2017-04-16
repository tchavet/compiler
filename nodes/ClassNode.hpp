#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "FieldNode.hpp"
#include "MethodNode.hpp"

class ClassNode : public AstNode
{
public:
	ClassNode(int line, int column, std::string name, std::string parent);
	std::string printTree(int tabsNb=0);
	void addField(FieldNode* field);
	void addFields(std::vector<FieldNode*>* fields);
	void addMethod(MethodNode* method);
	void addMethods(std::vector<MethodNode*>* methods);

protected:
	std::string name;
	std::string parent;
	std::vector<FieldNode*> fields;
	std::vector<MethodNode*> methods;
};
#endif

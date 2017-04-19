#ifndef PROGRAMNODE_H
#define PROGRAMNODE_H

#include "AstNode.hpp"
#include "ClassNode.hpp"

class ProgramNode : public AstNode
{
public:
	ProgramNode();
	std::string printTree(int tabsNb=0, bool types=false);
	void addClass(ClassNode* classNode);
	void addClasses(std::vector<ClassNode*>* classNodes);
	std::vector<ClassNode*> getClasses();
	std::vector<SemErr*> semCheck();

protected:
	std::vector<ClassNode*> classes;
};
#endif

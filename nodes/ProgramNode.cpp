#include "ProgramNode.hpp"

ProgramNode::ProgramNode()
{
	classes = std::vector<ClassNode*>();
}

void ProgramNode::addClass(ClassNode* classNode)
{
	classes.push_back(classNode);
}

void ProgramNode::addClasses(std::vector<ClassNode*>* classNodes)
{
	classes.insert(classes.end(), classNodes->begin(), classNodes->end());
}

std::string ProgramNode::printTree(int tabsNb)
{
	return printList<ClassNode>(tabsNb, classes);
}

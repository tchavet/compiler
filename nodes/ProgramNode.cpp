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

std::string ProgramNode::printTree(int tabsNb, bool types)
{
	return printList<ClassNode>(tabsNb,types,classes);
}

std::vector<ClassNode*> ProgramNode::getClasses()
{
	return classes;
}

std::vector<SemErr*> ProgramNode::semCheck()
{
	std::vector<SemErr*> errors;
	for (int i=0; i<classes.size(); i++)
	{
		std::vector<SemErr*> classErrors = classes[i]->semCheck();
		errors.insert(errors.end(), classErrors.begin(), classErrors.end());
	}
	return errors;
}

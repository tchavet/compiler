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
	/* Run semantic checking on all classes */
	std::vector<SemErr*> errors;
	for (int i=0; i<classes.size(); i++)
	{
		std::vector<SemErr*> classErrors = classes[i]->semCheck();
		errors.insert(errors.end(), classErrors.begin(), classErrors.end());
	}
	return errors;
}

void ProgramNode::llvmHeader(LlvmManager* manager)
{
	for (int i=0; i<classes.size(); i++)
	{
		classes[i]->llvmHeader(manager);
		manager->write("");
	}
} 

void ProgramNode::llvmMain(LlvmManager* manager)
{
	/*for (int i=0; i<classes.size(); i++)
	{
		classes[i]->llvmMain(manager);
		manager->write("");
	}*/
}

std::string ProgramNode::llvm(LlvmManager* manager, std::string retName)
{
	for (int i=0; i<classes.size(); i++)
	{
		manager->write("");
		classes[i]->llvm(manager);
	}
	return "";
}

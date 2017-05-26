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
	manager->write("declare double @pow(double, double)");
	manager->write("declare i32 @strcmp(i8*, i8*)");
} 

std::string ProgramNode::llvm(LlvmManager* manager)
{
	for (int i=0; i<classes.size(); i++)
	{
		manager->write("");
		classes[i]->llvm(manager);
	}
	return "";
}

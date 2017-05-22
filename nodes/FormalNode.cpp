#include "FormalNode.hpp"

FormalNode::FormalNode(int line, int column, std::string name, std::string type) : AstNode(line, column)
{
	this->name = name;
	this->type = type;
	llvmName = name;
}

std::string FormalNode::printTree(int tabsNb, bool types)
{
	return name + " : " + type;
}

std::string FormalNode::getName()
{
	return name;
}

std::string FormalNode::getType()
{
	return type;
}

void FormalNode::setLlvmNameInScope(std::string var, std::string llvmName)
{
	if (name == var)
		this->llvmName = llvmName;
}

std::string FormalNode::getLlvmVariable(std::string var, LlvmManager* manager, bool pointer)
{
	if (name == var)
	{
		if (pointer)
			return llvmName;
		else
			return manager->write("load "+LlvmManager::llvmType(type)+"* "+llvmName, name);
	}
	else
		return "";
}

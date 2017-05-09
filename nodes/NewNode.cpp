#include "NewNode.hpp"

NewNode::NewNode(int line, int column, std::string type) : ExprNode(line, column)
{
	this->type = type;
}

std::string NewNode::printTree(int tabsNb, bool types)
{
	return "New(" + type + ")" + (types ? " : " + type : "");
}

ExprType* NewNode::getType()
{
	return new ExprType(type);
}

std::string NewNode::llvm(LlvmManager* manager)
{
	return manager->write("alloca "+LlvmManager::llvmType(type));
}

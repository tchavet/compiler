#include "IntLitNode.hpp"
#include <sstream>

IntLitNode::IntLitNode(int line, int column, int value) : ExprNode(line, column)
{
	this->value = value;
}

std::string IntLitNode::printTree(int tabsNb, bool types)
{
	std::stringstream ss;
	ss << value;
	return ss.str() + (types ? " : " + type : "");
}

ExprType* IntLitNode::getType()
{
	type = "int32";
	return new ExprType("int32");
}

std::string llvm(LlvmManager* manager)
{
	return this->value;
}

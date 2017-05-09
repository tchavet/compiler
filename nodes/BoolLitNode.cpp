#include "BoolLitNode.hpp"

BoolLitNode::BoolLitNode(int line, int column, std::string value) : ExprNode(line, column)
{
	this->value = value;
}

std::string BoolLitNode::printTree(int tabsNb, bool types)
{
	return value + (types ? " : " + type : "");
}

ExprType* BoolLitNode::getType()
{
	ExprType* exprType = new ExprType("bool");
	type = "bool";
	return exprType;
}

std::string BoolLitNode::llvm(LlvmManager* manager)
{
	if(this->value == "false")
		return "0";
	return "1";	
}

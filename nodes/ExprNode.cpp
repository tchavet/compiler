#include "ExprNode.hpp"

ExprNode::ExprNode(int line, int column) : AstNode(line, column)
{
	type = "unit";
}

ExprType* ExprNode::getType()
{
	return new ExprType("unit");
}

std::string ExprNode::printTree(int tabsNb, bool types)
{
	return std::string("()") + (types ? " : unit" : "");
}

void ExprNode::setLlvmNameInScope(std::string var, std::string llvmName)
{
	if (parent)
		parent->setLlvmNameInScope(var, llvmName);
}

std::string ExprNode::getLlvmNameInScope(std::string var)
{
	if (parent)
		return parent->getLlvmNameInScope(var);
	else
		return "";
}

std::string getLlvmType()
{
	if (type == "bool")
		return "i1";
	else if (type == "int32")
		return "i32";
	else if (type == "string")
		return "i8*";
	else if (type == "unit")
		return "void";
	else
		return "%class."+type;
}

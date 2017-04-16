#include "IntLitNode.hpp"
#include <sstream>

IntLitNode::IntLitNode(int line, int column, int value)
{
	AstNode(line, column);
	this->value = value;
}

std::string IntLitNode::printTree(int tabsNb)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

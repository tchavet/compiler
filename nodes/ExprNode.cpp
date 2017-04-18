#include "ExprNode.hpp"

ExprNode::ExprNode(int line, int column) : AstNode(line, column)
{
	type = "";
}

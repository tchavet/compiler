#include "CallNode.hpp"

CallNode::CallNode(int line, int column, ExprNode* objExpr, std::string name, std::vector<ExprNode*> args) : ExprNode(line, column)
{
	this->objExpr = objExpr;
	this->name = name;
	this->args = args;
}

std::string CallNode::printTree(int tabsNb)
{
	return "Call(\n"
		+ tabs(tabsNb+1) + objExpr->printTree(tabsNb+1) + ",\n"
		+ tabs(tabsNb+1) + name + ",\n"
		+ tabs(tabsNb+1) + printList<ExprNode>(tabsNb+1,args) + "\n"
		+ tabs(tabsNb) + ")";
}

ExprType* CallNode::getType()
{
}

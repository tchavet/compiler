#include "CallNode.hpp"

CallNode::CallNode(int line, int column, AstNode* objExpr, std::string name, std::vector<AstNode*> args)
{
	AstNode(line, column);
	this->objExpr = objExpr;
	this->name = name;
	this->args = args;
}

std::string CallNode::printTree(int tabsNb)
{
	return "Call(\n"
		+ tabs(tabsNb+1) + objExpr->printTree(tabsNb+1) + ",\n"
		+ tabs(tabsNb+1) + name + ",\n"
		+ tabs(tabsNb+1) + printList<AstNode>(tabsNb+1,args) + "\n"
		+ tabs(tabsNb) + ")";
}

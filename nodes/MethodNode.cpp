#include "MethodNode.hpp"

MethodNode::MethodNode(int line, int column, std::string name, std::vector<FormalNode*> params, std::string returnType, BlockNode* body)
{
	AstNode(line, column);
	this->name = name;
	this->params = params;
	this->returnType = returnType;
	this->body = body;
}

std::string MethodNode::printTree(int tabsNb)
{
	return "Method(" + name + ",\n"
		+ tabs(tabsNb+1) + printList<FormalNode>(tabsNb+1, params) + ",\n"
		+ tabs(tabsNb+1) + returnType + ",\n"
		+ tabs(tabsNb+1) + body->printTree(tabsNb+1) + "\n"
		+ tabs(tabsNb) + ")";
}

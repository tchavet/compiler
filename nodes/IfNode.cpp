#include "IfNode.hpp"

IfNode::IfNode(int line, int column, AstNode* cond, AstNode* then, AstNode* els)
{
	AstNode(line, column);
	this->cond = cond;
	this->then = then;
	this->els = els;
}

std::string IfNode::printTree(int tabsNb)
{
	std::string print = "If(\n"
		+ tabs(tabsNb+1) + cond->printTree(tabsNb+1) + ",\n"
		+ tabs(tabsNb+1) + then->printTree(tabsNb+1);
	if (els)
	{
		print += ",\n"
			+ tabs(tabsNb+1) + els->printTree(tabsNb+1);
	}
	print += "\n"
		+ tabs(tabsNb) + ")";
	return print;
}

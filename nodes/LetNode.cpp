#include "LetNode.hpp"

LetNode::LetNode(int line, int column, std::string name, std::string type, ExprNode* scope, ExprNode* init) : ExprNode(line, column)
{
	this->name = name;
	this->type = type;
	this->scope = scope;
	this->init = init;
}

std::string LetNode::printTree(int tabsNb)
{
	std::string print = "Let(" + name + ", " + type + ",\n";
	if (init)
	{
		print += tabs(tabsNb+1) + init->printTree(tabsNb+1) + ",\n";
	}
	print += tabs(tabsNb+1) + scope->printTree(tabsNb+1) + "\n"
		+ tabs(tabsNb) + ")";
	return print;
}

ExprType* LetNode::getType()
{
}

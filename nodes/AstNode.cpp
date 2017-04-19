#include "AstNode.hpp"
#include <string>
#include <sstream>

AstNode::AstNode(int line, int column)
{
	this->line = line;
	this->column = column;
	parent = NULL;
}

std::string AstNode::tabs(int tabsNb)
{
	std::string tab = "  ";
	std::string tabsStr = "";
	for (int i=0; i<tabsNb; i++)
		tabsStr += tab;
	return tabsStr;
}

int AstNode::getLine()
{
	return line;
}

int AstNode::getColumn()
{
	return column;
}

std::string AstNode::getTypeInScope(std::string id)
{
	if (parent)
		return parent->getTypeInScope(id);
	return "";
}

void AstNode::setParent(AstNode* parent)
{
	this->parent = parent;
}

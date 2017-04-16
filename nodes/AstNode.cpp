#include "AstNode.hpp"
#include <string>
#include <sstream>

#include <iostream>

AstNode::AstNode(int line, int column)
{
	this->line = line;
	this->column = column;
}

std::string AstNode::tabs(int tabsNb)
{
	std::string tab = "  ";
	std::string tabsStr = "";
	for (int i=0; i<tabsNb; i++)
		tabsStr += tab;
	return tabsStr;
}

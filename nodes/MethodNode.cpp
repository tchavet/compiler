#include "MethodNode.hpp"

MethodNode::MethodNode(int line, int column, std::string name, std::vector<FormalNode*> params, std::string returnType, BlockNode* body) : AstNode(line, column)
{
	this->name = name;
	this->params = params;
	this->returnType = returnType;
	this->body = body;
	body->setParent(this);
	for (int i=0; i<params.size(); i++)
		params[i]->setParent(this);
}

std::string MethodNode::printTree(int tabsNb)
{
	return "Method(" + name + ",\n"
		+ tabs(tabsNb+1) + printList<FormalNode>(tabsNb+1, params) + ",\n"
		+ tabs(tabsNb+1) + returnType + ",\n"
		+ tabs(tabsNb+1) + body->printTree(tabsNb+1) + "\n"
		+ tabs(tabsNb) + ")";
}

std::string MethodNode::getTypeInScope(std::string id)
{
	for (int i=0; i<params.size(); i++)
	{
		if (params[i]->getName() == id)
			return params[i]->getType();
	}
	if (parent)
		return parent->getTypeInScope(id);
	else
		return "";
}

std::string MethodNode::getName()
{
	return name;
}

std::vector<FormalNode*> MethodNode::getParams()
{
	return params;
}

std::vector<SemErr*> MethodNode::semCheck()
{
	return body->getType()->errors;
}

std::string MethodNode::getReturnType()
{
	return returnType;
}

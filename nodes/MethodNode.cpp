#include "MethodNode.hpp"
#include "../semantic/Types.hpp"
#include "ClassNode.hpp"

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

std::string MethodNode::printTree(int tabsNb, bool types)
{
	return "Method(" + name + ",\n"
		+ tabs(tabsNb+1) + printList<FormalNode>(tabsNb+1,types, params) + ",\n"
		+ tabs(tabsNb+1) + returnType + ",\n"
		+ tabs(tabsNb+1) + body->printTree(tabsNb+1,types) + "\n"
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
	std::vector<SemErr*> errors;
	if (!Types::defined(returnType))
	{
		errors.push_back(new SemErr(line, column, "undefined return type " + returnType));
	}
	if (((ClassNode*)parent)->redefinedMethod(this))
	{
		errors.push_back(new SemErr(line, column, "multiple definitions of method " + name));
	}
	for (int i=0; i<params.size(); i++)
	{
		if (params[i]->getName() == "self")
		{
			errors.push_back(new SemErr(line, column, "cannot use self as an argument name"));
		}
		else if (!Types::defined(params[i]->getType()))
		{
			errors.push_back(new SemErr(line, column, "undefined type " + params[i]->getType()));
		}
		for (int j=0; j<i; j++)
		{
			if (params[i]->getName() == params[j]->getName())
			{
			errors.push_back(new SemErr(line, column, "multiple definitions of parameter " + params[i]->getName()));
			}
		}
	}
	ExprType* bodyType = body->getType();
	std::vector<SemErr*> bodyErrors = bodyType->errors;
	errors.insert(errors.begin(), bodyErrors.begin(), bodyErrors.end());
	if (bodyType->type != "")
	{
		ClassNode* classNode = Types::getNode(bodyType->type);
		if (!classNode && bodyType->type != returnType || classNode && !classNode->isA(returnType))
		{
			errors.push_back(new SemErr(line, column, "method return type is " + returnType + " but method body returns a " + bodyType->type));
		}
	}
	return errors;
}

std::string MethodNode::getReturnType()
{
	return returnType;
}

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
	llvmType = "";
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

std::string MethodNode::getReturnType()
{
	return returnType;
}

std::vector<FormalNode*> MethodNode::getParams()
{
	return params;
}

std::vector<SemErr*> MethodNode::semCheck()
{
	std::vector<SemErr*> errors;
	if (!Types::defined(returnType)) // If the return type is not a valid type
	{
		errors.push_back(new SemErr(line, column, "undefined return type " + returnType));
	}
	if (((ClassNode*)parent)->redefinedMethod(this)) // If the method has already been defined
	{
		errors.push_back(new SemErr(line, column, "multiple definitions of method " + name));
	}
	/* Check parameters */
	for (int i=0; i<params.size(); i++)
	{
		if (params[i]->getName() == "self") // If the parameter's name is self
		{
			errors.push_back(new SemErr(line, column, "cannot use self as an argument name"));
		}
		else if (!Types::defined(params[i]->getType())) // If the parameter's type is undefined
		{
			errors.push_back(new SemErr(line, column, "undefined type " + params[i]->getType()));
		}
		/* Check that there are no other parameters with the same name */
		for (int j=0; j<i; j++)
		{
			if (params[i]->getName() == params[j]->getName())
			{
			errors.push_back(new SemErr(line, column, "multiple definitions of parameter " + params[i]->getName()));
			}
		}
	}
	/* Run semantic checking on the method's body */
	ExprType* bodyType = body->getType();
	std::vector<SemErr*> bodyErrors = bodyType->errors;
	errors.insert(errors.begin(), bodyErrors.begin(), bodyErrors.end());
	/* Check that the body's return type is the same as the one that was declared */
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

void MethodNode::setLlvmNameInScope(std::string var, std::string llvmName)
{
	for (int i=0; i<params.size(); i++)
	{
		if (params[i]->getName() == var)
		{
			params[i]->setLlvmNameInScope(var, llvmName);
		}
	}
}

std::string MethodNode::getLlvmNameInScope(std::string var)
{
	for (int i=0; i<params.size(); i++)
	{
		if (params[i]->getName() == var)
		{
			return params[i]->getLlvmNameInScope(var);
		}
	}
	if (parent)
		return parent->getLlvmNameInScope(var);
	else
		return "";
}

void MethodNode::llvmHeader(LlvmManager* manager)
{
	llvmType = "%method.type."+((ClassNode*)parent)->getName()+"."+name;
	/* Define the method type */
	// %method.type.<className>.<methodName> = type <retType> (<paramType>, <paramType,...)
	std::string methodType = llvmType+" = type "+LlvmManager::llvmType(returnType) + " (";
	for(int i=0; i<params.size(); ++i)
	{
		if(i != 0)
		{
			methodType += ", ";
		}

		methodType += LlvmManager::llvmType(params[i]->getType());
	}
	methodType += ")*";
	manager->write(methodType);
}

std::string MethodNode::getLlvmType()
{
	return llvmType;
}

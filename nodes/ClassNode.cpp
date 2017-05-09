#include "ClassNode.hpp"
#include <sstream>

ClassNode::ClassNode(int line, int column, std::string name, std::string parent) : AstNode(line, column)
{
	this->name = name;
	this->parentName = parent;
	fields = std::vector<FieldNode*>();
	methods = std::vector<MethodNode*>();
	parentNode = NULL;
}

void ClassNode::addField(FieldNode* field)
{
	fields.push_back(field);
	field->setParent(this);
}

void ClassNode::addFields(std::vector<FieldNode*>* fields)
{
	this->fields.insert(this->fields.end(), fields->begin(), fields->end());
	for (int i=0; i<fields->size(); i++)
		(*fields)[i]->setParent(this);
}

void ClassNode::addMethod(MethodNode* method)
{
	methods.push_back(method);
	method->setParent(this);
}

void ClassNode::addMethods(std::vector<MethodNode*>* methods)
{
	this->methods.insert(this->methods.end(), methods->begin(), methods->end());
	for (int i=0; i<methods->size(); i++)
		(*methods)[i]->setParent(this);
}

std::string ClassNode::getName()
{
	return name;
}

std::string ClassNode::getParentName()
{
	return parentName;
}

void ClassNode::setParentNode(ClassNode* parent)
{
	parentNode = parent;
}

ClassNode* ClassNode::getParentNode()
{
	return parentNode;
}

bool ClassNode::isA(std::string className)
{
	if (className == name || className == "Object")
		return true;
	else if (!parentNode)
		return false;
	else
		return parentNode->isA(className);
}

std::string ClassNode::commonAncestor(ClassNode* class2)
{
	if (this->isA(class2->getName()))
		return class2->getName();
	if (class2->isA(name))
		return name;

	ClassNode* curClass = class2;
	while (curClass->getName() != "Object")
	{
		if (this->isA(curClass->getName()))
			return curClass->getName();
		curClass = curClass->getParentNode();
	}
	return "";
}

std::string ClassNode::printTree(int tabsNb, bool types)
{
	std::string print = "";
	
	print += "Class(" + name + ", " + parentName + ",\n"
		+ tabs(tabsNb+1) + printList<FieldNode>(tabsNb+1,types, fields) + ",\n"
		+ tabs(tabsNb+1) + printList<MethodNode>(tabsNb+1,types, methods) + "\n"
		+ tabs(tabsNb) + ")";
	return print;
}

std::string ClassNode::getTypeInScope(std::string id)
{
	if (id == "self")
		return name;
	for (int i=0; i<fields.size(); i++)
	{
		if (fields[i]->getName() == id)
			return fields[i]->getType();
	}
	if (parentNode)
		return parentNode->getTypeInScope(id);
	return AstNode::getTypeInScope(id);
}

MethodNode* ClassNode::getMethod(std::string methodName)
{
	for (int i=0; i<methods.size(); i++)
	{
		if (methods[i]->getName() == methodName)
			return methods[i];
	}
	if (parentNode)
		return parentNode->getMethod(methodName);
	return NULL;
}

std::vector<SemErr*> ClassNode::semCheck()
{
	std::vector<SemErr*> errors;
	for (int i=0; i<methods.size(); i++)
	{
		std::vector<SemErr*> methodErrors = methods[i]->semCheck();
		errors.insert(errors.end(), methodErrors.begin(), methodErrors.end());
	}
	for (int i=0; i<fields.size(); i++)
	{
		std::vector<SemErr*> fieldErrors = fields[i]->semCheck();
		errors.insert(errors.end(), fieldErrors.begin(), fieldErrors.end());
	}
	return errors;
}

bool ClassNode::redefinedField(FieldNode* field)
{
	if (parentNode && parentNode->getTypeInScope(field->getName()) != "")
		return true;
	for (int i=0; i<fields.size(); i++)
	{
		if (fields[i] != field && fields[i]->getName() == field->getName())
			return true;
	}
	return false;
}

bool ClassNode::redefinedMethod(MethodNode* method)
{
	for (int i=0; i<methods.size(); i++)
	{
		if (methods[i] != method && methods[i]->getName() == method->getName())
			return true;
	}
	if (parentNode)
	{
		MethodNode* redefMethod = parentNode->getMethod(method->getName());
		if (redefMethod)
		{
			if (redefMethod->getReturnType() != method->getReturnType())
				return true;
			std::vector<FormalNode*> methodParams = method->getParams();
			std::vector<FormalNode*> redefParams = redefMethod->getParams();
			if (redefParams.size() != methodParams.size())
				return true;
			for (int i=0; i<methodParams.size(); i++)
			{
				if (methodParams[i]->getType() != redefParams[i]->getType() || methodParams[i]->getName() != redefParams[i]->getName())
					return true;
			}
		}
	}
	return false;
}

stringmap* ClassNode::getFuncStruct()
{
	if(funcStruct == NULL)
	{
		if(parentNode != NULL)
		{
			funcStruct = parentNode->getFuncStruct();
		}
		else
		{
			funcStruct = new stringmap();
		}

		for(int j = funcStruct->size(); j < funcStruct->size() + methods.size(); ++j)
		{
			std::stringstream ss;
			ss << j;
			funcStruct->insert({{methods[j]->getName(), ss.str()}});
		}
	}

	return funcStruct;
}

stringmap* ClassNode::getFieldsStruct()
{
	if(fieldsStruct == NULL)
	{
		if(parentNode != NULL)
		{
			fieldsStruct = parentNode->getFieldsStruct();
		}
		else
		{
			fieldsStruct = new stringmap();
		}

		for(int j = fieldsStruct->size(); j < fieldsStruct->size() + fields.size(); ++j)
		{
			std::stringstream ss;
			ss << j;
			fieldsStruct->insert({{fields[j]->getName(), ss.str()}});
		}
	}

	return fieldsStruct;
}

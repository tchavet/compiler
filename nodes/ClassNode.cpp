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

	/* Cycle through the parents */
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
	/* Check the classes fields */
	for (int i=0; i<fields.size(); i++)
	{
		if (fields[i]->getName() == id)
			return fields[i]->getType();
	}
	/* Check the parent's fields */
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

std::vector<FieldNode*> ClassNode::getFields()
{
	return fields;
}

std::vector<MethodNode*> ClassNode::getMethods()
{
	return methods;
}

std::vector<SemErr*> ClassNode::semCheck()
{
	std::vector<SemErr*> errors;
	/* Run semantic checking on each method */
	for (int i=0; i<methods.size(); i++)
	{
		std::vector<SemErr*> methodErrors = methods[i]->semCheck();
		errors.insert(errors.end(), methodErrors.begin(), methodErrors.end());
	}
	/* Run semantic checking on each field */
	for (int i=0; i<fields.size(); i++)
	{
		std::vector<SemErr*> fieldErrors = fields[i]->semCheck();
		errors.insert(errors.end(), fieldErrors.begin(), fieldErrors.end());
	}
	return errors;
}

bool ClassNode::redefinedField(FieldNode* field)
{
	if (parentNode && parentNode->getTypeInScope(field->getName()) != "") // If the field exists in the parent
		return true;
	for (int i=0; i<fields.size(); i++)
	{
		if (fields[i] != field && fields[i]->getName() == field->getName()) // If there is another field of that name in this class
			return true;
	}
	return false;
}

bool ClassNode::redefinedMethod(MethodNode* method)
{
	for (int i=0; i<methods.size(); i++)
	{
		if (methods[i] != method && methods[i]->getName() == method->getName()) // If there is another method of that name in this class
			return true;
	}
	if (parentNode)
	{
		MethodNode* redefMethod = parentNode->getMethod(method->getName());
		if (redefMethod) // If a method of that name was found in the parent's class
		{
			if (redefMethod->getReturnType() != method->getReturnType()) // If the parent's method has a different return type
				return true;
			std::vector<FormalNode*> methodParams = method->getParams();
			std::vector<FormalNode*> redefParams = redefMethod->getParams();
			if (redefParams.size() != methodParams.size()) // If the parent's method has a different number of parameters
				return true;
			for (int i=0; i<methodParams.size(); i++)
			{
				if (methodParams[i]->getType() != redefParams[i]->getType() || methodParams[i]->getName() != redefParams[i]->getName()) // If the parent's method's parameter type or name is different
					return true;
			}
		}
	}
	return false;
}

stringmap* ClassNode::getAllMethods()
{
	if(!allMethods)
	{
		if(parentNode)
		{
			allMethods = parentNode->getAllMethods();
		}
		else
		{
			allMethods = new stringmap();
		}

		for(int j = allMethods->size(); j < allMethods->size() + methods.size(); ++j)
		{
			allMethods->insert({{methods[j]->getName(), j}});
		}
	}

	return allMethods;
}

stringmap* ClassNode::getAllFields()
{
	if(!allFields)
	{
		if(parentNode)
		{
			allFields = parentNode->getAllFields();
		}
		else
		{
			allFields = new stringmap();
		}

		for(int j = allFields->size(); j < allFields->size() + fields.size(); ++j)
		{
			allFields->insert({{fields[j]->getName(), j}});
		}
	}

	return allFields;
}

void ClassNode::setLlvmNameInScope(std::string var, std::string llvmName)
{
	for (int i=0; i<fields.size(); i++)
	{
		if (fields[i]->getName() == var)
		{
			fields[i]->setLlvmNameInScope(var, llvmName);
		}
	}
}

std::string ClassNode::getLlvmNameInScope(std::string var)
{
	for (int i=0; i<fields.size(); i++)
	{
		if (fields[i]->getName() == var)
		{
			return fields[i]->getLlvmNameInScope(var);
		}
	}
	if (parentNode)
		return parentNode->getLlvmNameInScope(var);
	else
		return "";
}

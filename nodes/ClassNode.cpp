#include "ClassNode.hpp"
#include <sstream>

ClassNode::ClassNode(int line, int column, std::string name, std::string parent) : AstNode(line, column)
{
	this->name = name;
	this->parentName = parent;
	fields = std::vector<FieldNode*>();
	methods = std::vector<MethodNode*>();
	allFields = stringmap();
	allMethods = stringmap();
	allMethodsSet = false;
	allFieldsSet = false;
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
	{
		(*fields)[i]->setParent(this);
	}
}

void ClassNode::addMethod(MethodNode* method)
{
	methods.push_back(method);
	method->setParent(this);
	method->setClass(this);
}

void ClassNode::addMethods(std::vector<MethodNode*>* methods)
{
	this->methods.insert(this->methods.end(), methods->begin(), methods->end());
	for (int i=0; i<methods->size(); i++)
	{
		(*methods)[i]->setParent(this);
		(*methods)[i]->setClass(this);
	}
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

FieldNode* ClassNode::getField(std::string fieldName)
{
	for (int i=0; i<fields.size(); i++)
	{
		if (fields[i]->getName() == fieldName)
			return fields[i];
	}
	if (parentNode)
		return parentNode->getField(fieldName);
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

stringmap ClassNode::getAllMethods()
{
	if(!allMethodsSet)
	{
		if(parentNode)
		{
			allMethods = parentNode->getAllMethods();
		}
		for(int i = 0; i < methods.size(); ++i)
		{
			allMethods.insert({{methods[i]->getName(), allMethods.size()}});
		}
	}

	return allMethods;
}

stringmap ClassNode::getAllFields()
{
	if(!allFieldsSet)
	{
		if(parentNode)
		{
			allFields = parentNode->getAllFields();
		}
		for(int i = 0; i < fields.size(); ++i)
		{
			allFields.insert({{fields[i]->getName(), allFields.size()}});
		}
	}

	return allFields;
}

void ClassNode::llvmHeader(LlvmManager* manager)
{
	/* Fill the hashmap with the methods */
	getAllMethods();

	/* Fill the hashmap with the fields */
	getAllFields();

	manager->addClass(name, allMethods, allFields);
	
	/* Define the type for the methods that this class declares */
	for (int i=0; i<methods.size(); i++)
	{
		methods[i]->llvmHeader(manager);
	}

	std::vector<std::string> methodNames(allMethods.size()); // Names of the methods in the right order
	/* Fill the methodNames vector in the right order */
	for(stringmap::const_iterator it = allMethods.cbegin(); it != allMethods.cend(); ++it)
	{
		methodNames[it->second] = it->first;
	}

	/* Add the methods types to the methods structure type */
	std::string methodsType = "%methods.type."+name+" = type {"; // Type of the structure containing the pointers to the methods
	for(int i = 0; i < methodNames.size(); ++i)
	{
		if(i != 0)
		{
			methodsType += ", ";
		}

		MethodNode* methodNode = getMethod(methodNames[i]);
		methodsType += methodNode->getLlvmType();
	}
	methodsType += "}";
	manager->write(methodsType);
	/* Instantiate the methods structure */
	std::string methodsVector = "@methods."+name+" = global %methods.type."+name+" {";
	//manager->write("@methods."+name+" = global %methods.type."+name+" zeroinitializer");
	for(int i = 0; i < methodNames.size(); ++i)
	{
		if(i != 0)
		{
			methodsVector += ", ";
		}

		MethodNode* methodNode = getMethod(methodNames[i]);
		methodsVector += methodNode->getLlvmType() + " " + methodNode->getLlvmName() ;
	}
	methodsVector += "}";
	manager->write(methodsVector);

	/* Define the class structure type */
	std::string classType = "%class."+name+" = type {%methods.type."+name+"*";
	std::vector<std::string> fieldNames(allFields.size()); // Names of the fields in the right order
	/* Fill the fieldNames vector in the right order */
	for(stringmap::const_iterator it = allFields.cbegin(); it != allFields.cend(); ++it)
	{
		fieldNames[it->second] = it->first;
	}

	for(int i = 0; i < fieldNames.size(); ++i)
	{
		classType += ", ";
		classType += LlvmManager::llvmType(getTypeInScope(fieldNames[i]));
	}

	classType += "}";

	manager->write(classType);
}

void ClassNode::llvmMain(LlvmManager* manager)
{
	std::vector<std::string> methodNames(allMethods.size()); // Names of the methods in the right order
	/* Fill the methodNames vector in the right order */
	for(stringmap::const_iterator it = allMethods.cbegin(); it != allMethods.cend(); ++it)
	{
		methodNames[it->second] = it->first;
	}

	/* Fill the methods structure */
	for (int i=0; i<methodNames.size(); i++)
	{
		MethodNode* methodNode = getMethod(methodNames[i]);
		// %# = getelementpointer %methods.type.<className>* @methods.<className>, i32 0, i32 #
		std::string ptr = manager->write("getelementptr %methods.type."+name+"* @methods."+name+", i32 0, i32 "+std::to_string(i), ".");
		// store <methodType> @method.<className>.<methodName>, <methodType>* %#
		manager->write("store "+methodNode->getLlvmType()+" @method."+methodNode->getClass()->getName()+"."+methodNode->getName()+", "+methodNode->getLlvmType()+"* "+ptr);
	}
}

std::string ClassNode::llvm(LlvmManager *manager)
{
	for (int i=0; i<methods.size(); i++)
	{
		methods[i]->llvm(manager);
	}
	return "";
}

std::string ClassNode::llvmAllocate(LlvmManager *manager)
{
	/* Allocate the class struct */
	std::string objPtr = manager->write("alloca %class."+name, ".new_"+name);

	/* Set the methods vector */
	// %# = getelementpointer %class.<className>* %<objPtr>, i32 0, i32 0
	std::string methodsPtr = manager->write("getelementptr %class."+name+"* "+objPtr+", i32 0, i32 0", ".");
	// store %methods.type.<className> @methods.<className>, %methods.type.<className>* %#
	manager->write("store %methods.type."+name+"* @methods."+name+", %methods.type."+name+"** "+methodsPtr);

	/* Set all the fields */
	std::vector<std::string> fieldNames(allFields.size()); // Names of the fields in the right order
	/* Fill the fieldNames vector in the right order */
	for(stringmap::const_iterator it = allFields.cbegin(); it != allFields.cend(); ++it)
	{
		fieldNames[it->second] = it->first;
	}
	for (int i=0; i<fieldNames.size(); i++)
	{
		FieldNode *fieldNode = getField(fieldNames[i]);
		// %# = getelementptr %class.<className>* %<objPtr>, i32 0, <fieldType> <i+1>
		std::string ptr = manager->write("getelementptr %class."+name+"* "+objPtr+", i32 0, i32 "+std::to_string(i+1), ".");
		std::string fieldType = LlvmManager::llvmType(fieldNode->getType());
		std::string initExpr = fieldNode->llvm(manager);
		// store <fieldType> <fieldInit>, <fieldType>* %#
		manager->write("store "+fieldType+" "+initExpr+", "+fieldType+"* "+ptr);
	}
	return objPtr;
}

void ClassNode::declareMethods(LlvmManager *manager)
{
	for (int i=0; i<methods.size(); i++)
	{
		methods[i]->declare(manager);
	}
}

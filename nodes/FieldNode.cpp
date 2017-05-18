#include "FieldNode.hpp"
#include "../semantic/Types.hpp"

FieldNode::FieldNode(int line, int column, std::string name, std::string type, ExprNode* init) : AstNode(line, column)
{
	this->name = name;
	this->type = type;
	this->init = init;
	llvmName = name;
	if (init)
		init->setParent(this);
}

std::string FieldNode::printTree(int tabsNb, bool types)
{
	std::string print = "Field(" + name + "," + type;
	if (init)
	{
		print += ",\n"
			+ tabs(tabsNb+1) + init->printTree(tabsNb+1,types) + "\n"
			+ tabs(tabsNb);
	}
	print += ")";
	return print;
}

std::string FieldNode::getName()
{
	return name;
}

std::string FieldNode::getType()
{
	return type;
}

std::vector<SemErr*> FieldNode::semCheck()
{
	std::vector<SemErr*> semErr;
	std::string typeInScope;
    if (parent)
		typeInScope = parent->getTypeInScope(name); // Ask the class for the type of the field
	if (typeInScope != type) // If the field has already been defined with another type
	{
		semErr.push_back(new SemErr(line, column, "field " + name + " has already been defined as a " + typeInScope));
	}
	else if (((ClassNode*)parent)->redefinedField(this)) // If the field has already been defined in the class or a parent
	{
		semErr.push_back(new SemErr(line, column, "multiple definitions of field " + name));
	}
	if (!Types::defined(type)) // If the field's type is undefined
	{
		semErr.push_back(new SemErr(line, column, "undefined type " + type));
	}
	
	if (init) // If an initialization expression was provided
	{
		/* Run semantic check on the initialization expression */
		ExprType* initType = init->getType();
		if (initType->error)
			semErr.insert(semErr.end(),initType->errors.begin(),initType->errors.end());

		if (initType->type != "" && initType->type != type) // If the initialization expression's type is different from the declared field type
		{
			ClassNode* initClass = Types::getNode(initType->type);
 
			if(!(initClass && initClass->isA(type))) // If the types are not compatible (inheritance)
			{
				semErr.push_back(new SemErr(line, column, name + " defined as type " + type + ", but assignement expression is of type " + initType->type));
			}
		}
	}
	return semErr;
}

std::string FieldNode::getTypeInScope(std::string id)
{
	return "";
}

std::string FieldNode::llvm(LlvmManager* manager)
{
	if (init)
		return init->llvm(manager);
	else
		return "0";
}

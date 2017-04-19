#include "LetNode.hpp"
#include "../semantic/Types.hpp"

LetNode::LetNode(int line, int column, std::string name, std::string type, ExprNode* scope, ExprNode* init) : ExprNode(line, column)
{
	this->name = name;
	this->type = type;
	this->scope = scope;
	this->init = init;
	scope->setParent(this);
	if (init)
		init->setParent(this);
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
	ExprType* exprType = new ExprType;
	/* The identifier cannot be self */
	if (name == "self")
	{
		SemErr* semErr = new SemErr(line, column, "self is a reserved keyword and cannot be used as an identifier");
		exprType->addError(semErr);
	}
	/* Check that type exists */
	if (!Types::defined(type))
	{
		SemErr* semErr = new SemErr(line, column, "type " + type + " undefined");
		exprType->addError(semErr);
	}
	else
		exprType->type = type;

	/* Check that type of assigned expression is the same */
	if (init)
	{
		ExprType* initType = init->getType();
		if (initType->error)
			exprType->addErrors(initType->errors);


		if (initType->type != "" && initType->type != type)
		{
			ClassNode* initClass = Types::getNode(initType->type);
 
			if(!(initClass && initClass->isA(type)))
			{
				SemErr* semErr = new SemErr(line, column, name + " defined as type " + type + ", but assignement expression is of type " + initType->type);
				exprType->addError(semErr);
			}
		}
	}

	ExprType* scopeType = scope->getType();
	if (scopeType->error)
		exprType->addErrors(scopeType->errors);

	type = exprType->type;
	return exprType;
}

std::string LetNode::getTypeInScope(std::string id)
{
	if (name == id)
		return type;
	return AstNode::getTypeInScope(id);
}

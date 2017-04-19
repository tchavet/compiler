#include "LetNode.hpp"
#include "../semantic/Types.hpp"

LetNode::LetNode(int line, int column, std::string name, std::string type, ExprNode* scope, ExprNode* init) : ExprNode(line, column)
{
	this->name = name;
	this->letType = type;
	this->scope = scope;
	this->init = init;
	scope->setParent(this);
	if (init)
		init->setParent(this);
}

std::string LetNode::printTree(int tabsNb, bool types)
{
	std::string print = "Let(" + name + ", " + letType + ",\n";
	if (init)
	{
		print += tabs(tabsNb+1) + init->printTree(tabsNb+1,types) + ",\n";
	}
	print += tabs(tabsNb+1) + scope->printTree(tabsNb+1,types) + "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
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
	if (!Types::defined(letType))
	{
		SemErr* semErr = new SemErr(line, column, "type " + letType + " undefined");
		exprType->addError(semErr);
	}

	/* Check that type of assigned expression is the same */
	if (init)
	{
		ExprType* initType = init->getType();
		if (initType->error)
			exprType->addErrors(initType->errors);


		if (initType->type != "" && initType->type != letType)
		{
			ClassNode* initClass = Types::getNode(initType->type);
 
			if(!(initClass && initClass->isA(letType)))
			{
				SemErr* semErr = new SemErr(line, column, name + " defined as type " + letType + ", but assignement expression is of type " + initType->type);
				exprType->addError(semErr);
			}
		}
	}

	ExprType* scopeType = scope->getType();
	if (scopeType->error)
		exprType->addErrors(scopeType->errors);
	exprType->type = scopeType->type;

	type = exprType->type;
	return exprType;
}

std::string LetNode::getTypeInScope(std::string id)
{
	if (name == id)
		return letType;
	return AstNode::getTypeInScope(id);
}

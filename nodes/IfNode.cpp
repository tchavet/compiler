#include "IfNode.hpp"
#include "ClassNode.hpp"
#include "../semantic/Types.hpp"

IfNode::IfNode(int line, int column, ExprNode* cond, ExprNode* then, ExprNode* els) : ExprNode(line, column)
{
	this->cond = cond;
	this->then = then;
	this->els = els;
	cond->setParent(this);
	then->setParent(this);
	if (els)
		els->setParent(this);
}

std::string IfNode::printTree(int tabsNb, bool types)
{
	std::string print = "If(\n"
		+ tabs(tabsNb+1) + cond->printTree(tabsNb+1,types) + ",\n"
		+ tabs(tabsNb+1) + then->printTree(tabsNb+1,types);
	if (els)
	{
		print += ",\n"
			+ tabs(tabsNb+1) + els->printTree(tabsNb+1,types);
	}
	print += "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
	return print;
}

ExprType* IfNode::getType()
{
	/* Get types of cond, then and else expressions */
	ExprType* condType = cond->getType();
	ExprType* thenType = then->getType();
	ExprType* elsType;
	ExprType* exprType = new ExprType;
	if (els)
		elsType = els->getType();
	else
	{
		elsType = new ExprType("unit");
	}

	/* If one of those expressions had a type error, get these errors */
	if (condType->error || thenType->error || elsType->error);
	{
		if (condType->error)
			exprType->addErrors(condType->errors);
		if (thenType->error)
			exprType->addErrors(thenType->errors);
		if (elsType->error)
			exprType->addErrors(elsType->errors);
	}
	/* If the conditions type is non-boolean, it is an error */
	if (condType->type != "" && condType->type != "bool")
	{
		SemErr* semErr = new SemErr(line, column, "condition type of if is of non-boolean type " + condType->type);
		exprType->addError(semErr);
	}

	/* If one expression had an error making its type undeterminable, the type of the if-expresssion is undeterminable as well */
	if (thenType->type == "" || elsType->type == "")
	{
		exprType->type = "";
	}
	/* If one of those types is unit, the type of the if-expression is unit as well */
	else if (thenType->type == "unit" || elsType->type == "unit")
	{
		exprType->type = "unit";
	}
	else
	{
		ClassNode* thenClass = Types::getNode(thenType->type);
		ClassNode* elsClass = Types::getNode(elsType->type);
		std::string comAncest = "";

		if(thenClass && elsClass)
		{
			comAncest = thenClass->commonAncestor(elsClass);
		}

		/*If both types are the same, the type of the if-expression is that type */
		if (comAncest != "")
		{
			exprType->type = comAncest;
		}
		else if(thenType->type == elsType->type)
		{
			exprType->type = thenType->type;
		}

		/* Otherwise the types are different, it is an error */
		else
		{
			SemErr* semErr = new SemErr(line, column, "return types for then and else are not the same: then type is " + thenType->type + " and else type is " + elsType->type);
			exprType->addError(semErr);
		}
	}

	type = exprType->type;
	return exprType;
}

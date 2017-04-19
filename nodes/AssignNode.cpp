#include "AssignNode.hpp"

AssignNode::AssignNode(int line, int column, std::string name, ExprNode* expr) : ExprNode(line, column)
{
	this->name = name;
	this->expr = expr;
	expr->setParent(this);
}

std::string AssignNode::printTree(int tabsNb, bool types)
{
	return "Assign(" + name + ",\n"
		+ tabs(tabsNb+1) + expr->printTree(tabsNb+1, types) + "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
}

ExprType* AssignNode::getType()
{
	ExprType* exprType;
	/* self cannot be assigned */
	if (name == "self")
	{
		exprType = new ExprType;
		SemErr* semErr = new SemErr(line, column, "self is a reserved keyword and cannot be assigned");
		exprType->addError(semErr);
		exprType->addErrors(expr->getType()->errors);
	}
	else
	{
		exprType = expr->getType();
		if (exprType->type != "")
		{
			/* Check that the expression type is the same as the one of the variable being assigned, and that this variable exists */
			std::string nameType = getTypeInScope(name);
			if (nameType == "")
			{
				SemErr* semErr = new SemErr(line, column, "variable " + name + " is undefined in this scope");
				exprType->addError(semErr);
				exprType->type == "";
			}
			else if (nameType != exprType->type)
			{
				SemErr* semErr = new SemErr(line, column, "types do not match: type of " + name + " is " + nameType + " but type of the expression to assign is " + exprType->type);
				exprType->addError(semErr);
				exprType->type = nameType;
			}
		}
	}
	type = exprType->type;
	return exprType;
}

#include "WhileNode.hpp"

WhileNode::WhileNode(int line, int column, ExprNode* cond, ExprNode* body) : ExprNode(line, column)
{
	this->cond = cond;
	this->body = body;
	cond->setParent(this);
	body->setParent(this);
}

std::string WhileNode::printTree(int tabsNb, bool types)
{
	return "While(\n"
		+ tabs(tabsNb+1) + cond->printTree(tabsNb+1,types) + ",\n"
		+ tabs(tabsNb+1) + body->printTree(tabsNb+1,types) + "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
}

ExprType* WhileNode::getType()
{
	ExprType* condType = cond->getType();
	ExprType* exprType = body->getType();
	/* get the errors of cond */
	if (condType->error)
		exprType->addErrors(condType->errors);
	/* If cond is non-boolean, it is an error */
	if (condType->type != "" && condType->type != "bool")
	{
		SemErr* semErr = new SemErr(line, column, "condition type of while is of non-boolean type " + condType->type);
		exprType->addError(semErr);
	}

	type = exprType->type;
	return exprType;
}

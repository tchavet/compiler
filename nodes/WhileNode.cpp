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

	exprType->type = "unit";
	type = exprType->type;
	return exprType;
}

std::string WhileNode::llvm(LlvmManager* manager){
	std::string begin_label = manager->getNewLabel("while_cond");
	std::string while_body = manager->getNewLabel("while_body");
	std::string end_label = manager->getNewLabel("while_end");
	manager->writeLabel(begin_label);
	std::string cond = this->cond->llvm(manager);
	manager->write("br i1 "+ cond  + ", label %"+while_body +", label %"+end_label);
	manager->writeLabel(while_body);
	this->body->llvm(manager);
	manager->writeLabel(end_label);
	return "";
}

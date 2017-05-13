#include "BlockNode.hpp"

BlockNode::BlockNode(int line, int column) : ExprNode(line, column)
{
	expressions = std::vector<ExprNode*>();
}

void BlockNode::addExpression(ExprNode* expression)
{
	expressions.push_back(expression);
	expression->setParent(this);
}

void BlockNode::addExpressions(std::vector<ExprNode*>* expressions)
{
	this->expressions.insert(this->expressions.end(), expressions->begin(), expressions->end());
	for (int i=0; i<expressions->size(); i++)
		(*expressions)[i]->setParent(this);
}

std::string BlockNode::printTree(int tabsNb, bool types)
{
	if (expressions.size() == 1)
		return expressions[0]->printTree(tabsNb, types);
	return printList<ExprNode>(tabsNb,types, expressions) + (types ? " : " + type : "");
}

ExprType* BlockNode::getType()
{
	ExprType* exprType;
	/* If the block only contains one expression, the block type is that expression's type */
	if (expressions.size() == 0)
		exprType = new ExprType("unit");
	if (expressions.size() == 1)
		exprType = expressions[0]->getType();
	else
	{
		exprType = new ExprType;
		/* Get all the type errors of the expressions */
		for (int i=0; i<expressions.size(); i++)
			exprType->addErrors(expressions[i]->getType()->errors);
		exprType->type = expressions[expressions.size()-1]->getType()->type;
	}
	type = exprType->type;
	return exprType;
}
std::string BlockNode::llvm(LlvmManager* manager)
{
	for(int i = 0; i < expressions.size(); i++)
	{
		if(i == expressions.size()-1)
			return expressions[i]->llvm(manager);
		expressions[i]->llvm(manager);
	}//end for
	return "";
}

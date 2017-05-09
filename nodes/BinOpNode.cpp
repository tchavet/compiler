#include "BinOpNode.hpp"

BinOpNode::BinOpNode(int line, int column, std::string op, ExprNode* leftExpr, ExprNode* rightExpr) : ExprNode(line, column)
{
	this->op = op;
	this->leftExpr = leftExpr;
	this->rightExpr = rightExpr;
	leftExpr->setParent(this);
	rightExpr->setParent(this);
}

std::string BinOpNode::printTree(int tabsNb, bool types)
{
	return "BinOp(" + op + ",\n"
		+ tabs(tabsNb+1) + leftExpr->printTree(tabsNb+1,types) + ",\n"
		+ tabs(tabsNb+1) + rightExpr->printTree(tabsNb+1,types) + "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
}

ExprType* BinOpNode::getType()
{
	ExprType* exprType = new ExprType;
	ExprType* leftType = leftExpr->getType();
	ExprType* rightType = rightExpr->getType();

	if (leftType->error)
		exprType->addErrors(leftType->errors);
	if (rightType->error)
		exprType->addErrors(rightType->errors);

	/* Check that the type of both operands is good */
	if (leftType->type != "" && rightType->type != "")
	{
		std::string opType = "int32";
		if (op == "and")
			opType = "bool";
		else if (op == "=")
			opType = leftType->type;
		if (leftType->type != opType)
		{
			SemErr* semErr = new SemErr(line, column, "cannot do opperation " + op + " on left operand object of type " + leftType->type);
			exprType->addError(semErr);
		}
		if (rightType->type != opType)
		{
			SemErr* semErr = new SemErr(line, column, "cannot do opperation " + op + " on right operand object of type " + rightType->type);
			exprType->addError(semErr);
		}
		if (leftType->type == opType && rightType->type == opType)
		{
			if (op == "and" || op == "=" || op == "<" || op == "<=")
				exprType->type = "bool";
			else if (op == "+" || op == "-" || op == "*" || op == "/" || op == "^")
				exprType->type = "int32";
		}
	}
	type = exprType->type;
	return exprType;
}

std::string BinOpNode::llvm(LlvmManager* manager)
{
	std::string leftExprLlvm = leftExpr->llvm(manager);
	std::string rightExprLlvm = rightExpr->llvm(manager);
	if (op == "=")
		return manager.write("icmp eq "+leftExpr->getLlvmType()+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "<")
		return manager.write("icmp ult "+leftExpr->getLlvmType()+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "<=")
		return manager.write("icmp slt "+leftExpr->getLlvmType()+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "+")
		return manager.write("add "+leftExpr->getLlvmType()+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "-")
		return manager.write("sub "+leftExpr->getLlvmType()+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "*")
		return manager.write("mul "+leftExpr->getLlvmType()+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "/")
		return manager.write("sdiv "+leftExpr->getLlvmType()+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "^")
		return manager.write("prout "+leftExpr->getLlvmType()+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "and")
		return manager.write("and i1 "+leftExprLlvm+", "+rightExprLlvm, ".");
	else
		return "";
}

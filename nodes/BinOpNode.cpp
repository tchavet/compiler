#include "BinOpNode.hpp"
#include <iostream>
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

	/* Get the errors that happened in the subexpressions */
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
		/* Check that the type of both expression is consistant with the type required by the operand */
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
	if (op == "and")
	{
		std::string left_true = manager->getNewLabel("left_true");
		std::string and_true = manager->getNewLabel("and_true");
		std::string and_false = manager->getNewLabel("and_false");
		std::string and_end = manager->getNewLabel("and_end");
		manager->write("br i1 "+leftExprLlvm+", label %"+left_true+", label %"+and_false);
		manager->writeLabel(left_true);
		std::string rightExprLlvm = rightExpr->llvm(manager);
		manager->write("br i1 "+rightExprLlvm+", label %"+and_true+", label %"+and_false);
		manager->decIndent();
		manager->writeLabel(and_true);
		manager->write("br label %"+and_end);
		manager->decIndent();
		manager->writeLabel(and_false);
		manager->write("br label %"+and_end);
		manager->decIndent();
		manager->writeLabel(and_end);
		std::string result = manager->write("phi i1 [1, %"+and_true+"], [0, %"+and_false+"]",".");
		manager->decIndent();
		return result;
	}
	std::string rightExprLlvm = rightExpr->llvm(manager);
	if (op == "=")
	{
		if (rightExpr->getComputedType()!="string")
			return manager->write("icmp eq "+LlvmManager::llvmType(leftExpr->getComputedType())+" "+leftExprLlvm+", "+rightExprLlvm, ".");
		std::string eq =  manager->write("call i32 @strcmp(i8* "+ leftExprLlvm +", i8* "+rightExprLlvm +")" ,".");
		return manager->write("icmp eq i32 0, " + eq,".");
	}
	else if (op == "<")
		return manager->write("icmp slt "+LlvmManager::llvmType(leftExpr->getComputedType())+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "<=")
		return manager->write("icmp sle "+LlvmManager::llvmType(leftExpr->getComputedType())+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "+")
		return manager->write("add "+LlvmManager::llvmType(leftExpr->getComputedType())+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "-")
		return manager->write("sub "+LlvmManager::llvmType(leftExpr->getComputedType())+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "*")
		return manager->write("mul "+LlvmManager::llvmType(leftExpr->getComputedType())+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "/")
		return manager->write("sdiv "+LlvmManager::llvmType(leftExpr->getComputedType())+" "+leftExprLlvm+", "+rightExprLlvm, ".");
	else if (op == "^")
	{
		std::string doubleLeft = manager->write("sitofp i32 "+leftExprLlvm+" to double",".");

		std::string doubleRight = manager->write("sitofp i32 "+rightExprLlvm+" to double",".");
		std::string tmp = manager->write("call double @pow(double "+doubleLeft+", double "+doubleRight+")",".");
		return manager->write("fptosi double "+tmp+" to i32",".");
	}
	else
		return "";
}

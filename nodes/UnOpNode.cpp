#include "UnOpNode.hpp"
#include "../semantic/Types.hpp"
#include "ClassNode.hpp"

UnOpNode::UnOpNode(int line, int column, std::string op, ExprNode* expr) : ExprNode(line, column)
{
	this->op = op;
	this->expr = expr;
	expr->setParent(this);
}

std::string UnOpNode::printTree(int tabsNb, bool types)
{
	return "UnOp(" + op + ",\n"
		+ tabs(tabsNb+1) + expr->printTree(tabsNb+1,types) + "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
}

ExprType* UnOpNode::getType()
{
	ExprType* exprType = expr->getType();
	if (op == "not")
	{
		/* The type of an expression on which 'not' is applied must be bool */
		if (exprType->type != "" && exprType->type != "bool")
		{
			SemErr* semErr = new SemErr(line, column, "cannot do opperation " + op + " on expression of type " + exprType->type);
			exprType->addError(semErr);
			exprType->type = "";
		}
	}
	else if (op == "-")
	{
		/* The type of an expression on which '-' is applied must be int32 */
		if (exprType->type != "" && exprType->type != "int32")
		{
			SemErr* semErr = new SemErr(line, column, "cannot do opperation " + op + " on expression of type " + exprType->type);
			exprType->addError(semErr);
			exprType->type = "";
		}
	}
	else if (op == "isnull")
	{
		/* The type of an expression on which 'isnull' is applied must be a class type */
		if (exprType->type != "" && exprType->type != "Object")
		{
			ClassNode* classNode = Types::getNode(exprType->type);
			if (!classNode || !classNode->isA("Object"))
			{
				SemErr* semErr = new SemErr(line, column, "cannot do opperation " + op + " on expression of type " + exprType->type);
				exprType->addError(semErr);
				exprType->type = "";
			}
		}
		exprType->type = "bool"; // isnull returns a bool
	}
	type = exprType->type;
	return exprType;
}

std::string UnOpNode::llvm(LlvmManager* manager, std::string retName)
{
	std::string exprLlvm = expr->llvm(manager);
	if (op == "not")
		return manager->write("xor i1 1, "+exprLlvm, retName);
	else if (op == "-")
		return manager->write("sub i32 0, "+exprLlvm, retName);
	else if (op == "isnull")
		return manager->write("icmp eq "+LlvmManager::llvmType(expr->getComputedType())+" 0, "+exprLlvm, retName);
	else
		return "";
}

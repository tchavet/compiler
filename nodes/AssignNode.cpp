#include "AssignNode.hpp"
#include "ClassNode.hpp"
#include "IntLitNode.hpp"
#include "../semantic/Types.hpp"

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
			if (nameType == "") // If there is no type, it means that the variable is undefined
			{
				SemErr* semErr = new SemErr(line, column, "variable " + name + " is undefined in this scope");
				exprType->addError(semErr);
				exprType->type == "";
			}
			else if (nameType != exprType->type) // If the type of the variable being assigned and the one of the expression to assign are different
			{
				ClassNode* exprClass = Types::getNode(exprType->type);

				if(!(exprClass && exprClass->isA(nameType))) // Check if the types are compatible by inheritance
				{
					SemErr* semErr = new SemErr(line, column, "types do not match: type of " + name + " is " + nameType + " but type of the expression to assign is " + exprType->type);
					exprType->addError(semErr);
				}
				
				exprType->type = nameType;
			}
		}
	}
	type = exprType->type;
	return exprType;
}

std::string AssignNode::llvm(LlvmManager* manager)
{
	std::string exprLlvmName = expr->llvm(manager); // Convert the expression to llvm and get the unnamed variable where the result is stored
	// Cast if needed
	if (LlvmManager::llvmType(expr->getComputedType()) != type)
		exprLlvmName = manager->write("bitcast "+LlvmManager::llvmType(expr->getComputedType())+" "+exprLlvmName+" to "+LlvmManager::llvmType(type), ".");
	if (getLlvmNameInScope(name) == "") // The variable is a field
	{
		std::string objPtr = getLlvmNameInScope("obj.ptr");
		std::string varPtr = manager->getField(getTypeInScope("self"), name, objPtr);
		manager->write("store "+LlvmManager::llvmType(type)+" "+exprLlvmName+", "+LlvmManager::llvmType(type)+"* "+varPtr);
		return exprLlvmName;
	}
	else
	{
		std::string varName = "";
		if (dynamic_cast<IntLitNode*>(expr))
			varName = exprLlvmName;
		else
			varName =  manager->write(exprLlvmName, name); // %name.x = %exprLlvmName
		setLlvmNameInScope(name, varName);
		return varName;
	}
}

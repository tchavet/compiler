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
	std::string llvmType = LlvmManager::llvmType(type);
	// Cast if needed
	if (expr->getComputedType() != type)
		exprLlvmName = manager->write("bitcast "+LlvmManager::llvmType(expr->getComputedType())+" "+exprLlvmName+" to "+llvmType, name);
	std::string llvmVarName = getLlvmVariable(name, manager);
	if (llvmVarName == "") // The variable is a field
	{
		std::string objPtr = getLlvmVariable("obj.ptr", manager);
		llvmVarName = manager->getField(getTypeInScope("self"), name, objPtr);
	}
	manager->write("store "+llvmType+" "+exprLlvmName+", "+llvmType+"* "+llvmVarName);
	return exprLlvmName;
}

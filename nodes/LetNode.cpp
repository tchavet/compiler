#include "LetNode.hpp"
#include "StringLitNode.hpp"
#include "../semantic/Types.hpp"

LetNode::LetNode(int line, int column, std::string name, std::string type, ExprNode* scope, ExprNode* init) : ExprNode(line, column)
{
	this->name = name;
	this->letType = type;
	this->scope = scope;
	this->init = init;
	scope->setParent(this);
	llvmName = name;
	if (init)
		init->setParent(this);
}

std::string LetNode::printTree(int tabsNb, bool types)
{
	std::string print = "Let(" + name + ", " + letType + ",\n";
	if (init)
	{
		print += tabs(tabsNb+1) + init->printTree(tabsNb+1,types) + ",\n";
	}
	print += tabs(tabsNb+1) + scope->printTree(tabsNb+1,types) + "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
	return print;
}

ExprType* LetNode::getType()
{
	ExprType* exprType = new ExprType;
	/* The identifier cannot be self */
	if (name == "self")
	{
		SemErr* semErr = new SemErr(line, column, "self is a reserved keyword and cannot be used as an identifier");
		exprType->addError(semErr);
	}
	/* Check that type exists */
	if (!Types::defined(letType))
	{
		SemErr* semErr = new SemErr(line, column, "type " + letType + " undefined");
		exprType->addError(semErr);
	}

	/* Check that type of assigned expression is the same */
	if (init)
	{
		ExprType* initType = init->getType();
		if (initType->error)
			exprType->addErrors(initType->errors);


		if (initType->type != "" && initType->type != letType)
		{
			ClassNode* initClass = Types::getNode(initType->type);
 
			if(!(initClass && initClass->isA(letType)))
			{
				SemErr* semErr = new SemErr(line, column, name + " defined as type " + letType + ", but assignement expression is of type " + initType->type);
				exprType->addError(semErr);
			}
		}
	}

	ExprType* scopeType = scope->getType();
	if (scopeType->error)
		exprType->addErrors(scopeType->errors);
	exprType->type = scopeType->type;

	type = exprType->type;
	return exprType;
}

std::string LetNode::getTypeInScope(std::string id)
{
	if (name == id)
		return letType;
	return AstNode::getTypeInScope(id);
}

std::string LetNode::getLlvmVariable(std::string var, LlvmManager* manager, bool pointer)
{
	if (var == name)
	{
		if (pointer)
			return llvmName;
		else
			return manager->write("load "+LlvmManager::llvmType(letType)+"* "+llvmName, name);
	}
	if (parent)
		return parent->getLlvmVariable(var, manager, pointer);
	else
		return "";
}

std::string LetNode::llvm(LlvmManager* manager)
{
	std::string exprLlvmName;
	std::string llvmLetType = LlvmManager::llvmType(letType);
	if(init != NULL)	
	{
		exprLlvmName = init->llvm(manager);
		// If needed, cast the expression result
		if (init->getComputedType() != letType)
		{
			exprLlvmName = manager->write("bitcast "+LlvmManager::llvmType(init->getComputedType())+" "+exprLlvmName+" to "+llvmLetType+" ", name);
		}
	}
	else 
	{
		if (letType == "int32" || letType == "bool")
			exprLlvmName = "0";
		else if (letType == "string")
		{
			exprLlvmName = "null";
		}
		else
			exprLlvmName = "null";
//			llvmName = manager->write("getelementptr "+LlvmManager::llvmType(letType)+" null", name);
	}

	llvmName = manager->write("alloca "+llvmLetType, name);
	manager->write("store "+llvmLetType+" "+exprLlvmName+", "+llvmLetType+"* "+llvmName);

	return scope->llvm(manager);
}

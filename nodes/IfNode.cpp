#include "IfNode.hpp"
#include "ClassNode.hpp"
#include "../semantic/Types.hpp"

IfNode::IfNode(int line, int column, ExprNode* cond, ExprNode* then, ExprNode* els) : ExprNode(line, column)
{
	this->cond = cond;
	this->then = then;
	this->els = els;
	cond->setParent(this);
	then->setParent(this);
	if (els)
		els->setParent(this);
}

std::string IfNode::printTree(int tabsNb, bool types)
{
	std::string print = "If(\n"
		+ tabs(tabsNb+1) + cond->printTree(tabsNb+1,types) + ",\n"
		+ tabs(tabsNb+1) + then->printTree(tabsNb+1,types);
	if (els)
	{
		print += ",\n"
			+ tabs(tabsNb+1) + els->printTree(tabsNb+1,types);
	}
	print += "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
	return print;
}

ExprType* IfNode::getType()
{
	/* Get types of cond, then and else expressions */
	ExprType* condType = cond->getType();
	ExprType* thenType = then->getType();
	ExprType* elsType;
	ExprType* exprType = new ExprType;
	if (els)
		elsType = els->getType();
	else
	{
		elsType = new ExprType("unit");
	}

	/* If one of those expressions had a type error, get these errors */
	if (condType->error || thenType->error || elsType->error);
	{
		if (condType->error)
			exprType->addErrors(condType->errors);
		if (thenType->error)
			exprType->addErrors(thenType->errors);
		if (elsType->error)
			exprType->addErrors(elsType->errors);
	}
	/* If the conditions type is non-boolean, it is an error */
	if (condType->type != "" && condType->type != "bool")
	{
		SemErr* semErr = new SemErr(line, column, "condition type of if is of non-boolean type " + condType->type);
		exprType->addError(semErr);
	}

	/* If one expression had an error making its type undeterminable, the type of the if-expresssion is undeterminable as well */
	if (thenType->type == "" || elsType->type == "")
	{
		exprType->type = "";
	}
	/* If one of those types is unit, the type of the if-expression is unit as well */
	else if (thenType->type == "unit" || elsType->type == "unit")
	{
		exprType->type = "unit";
	}
	else
	{
		ClassNode* thenClass = Types::getNode(thenType->type);
		ClassNode* elsClass = Types::getNode(elsType->type);
		std::string comAncest = "";

		if(thenClass && elsClass)
		{
			comAncest = thenClass->commonAncestor(elsClass);
		}

		/*If both types are the same, the type of the if-expression is that type */
		if (comAncest != "")
		{
			exprType->type = comAncest;
		}
		else if(thenType->type == elsType->type)
		{
			exprType->type = thenType->type;
		}

		/* Otherwise the types are different, it is an error */
		else
		{
			SemErr* semErr = new SemErr(line, column, "return types for then and else are not the same: then type is " + thenType->type + " and else type is " + elsType->type);
			exprType->addError(semErr);
		}
	}

	type = exprType->type;
	return exprType;
}

std::string IfNode::llvm(LlvmManager* manager)
{
	std::string if_true = manager->getNewLabel("if_true");
	std::string if_false = manager->getNewLabel("if_false");
	std::string if_end = manager->getNewLabel("if_end");
	std::string cond = this->cond->llvm(manager);
	std::string retLlvmType = LlvmManager::llvmType(type);

	/* Create pointer where to put the result */
	std::string resultPtr="";
	if (type != "unit")
		resultPtr = manager->write("alloca "+retLlvmType, ".if_result_ptr");

	/* Condition */
	manager->write("br i1 "+ cond  + ", label %"+if_true +", label %"+if_false);

	/* Then */
	manager->writeLabel(if_true);
	std::string thenResult = then->llvm(manager);
	// Cast if necessary and if return type different than unit 
	std::string thenType = then->getComputedType();
	std::string thenLlvmType = LlvmManager::llvmType(thenType);
	if (thenType != type && type != "unit")
		thenResult = manager->write("bitcast "+thenLlvmType+" "+thenResult+" to "+retLlvmType, ".cast_then");
	if (type != "unit")
		manager->write("store "+retLlvmType+" "+thenResult+", "+retLlvmType+"* "+resultPtr);
	manager->write("br label %"+if_end);
	manager->decIndent();

	/* Else */
	manager->writeLabel(if_false);
	std::string elseResult = "";
	if(els != NULL)
	{
		elseResult = els->llvm(manager);
		std::string elseType = els->getComputedType();
		std::string elseLlvmType = LlvmManager::llvmType(elseType);
		if (elseType != type && type != "unit")
			elseResult = manager->write("bitcast "+elseLlvmType+" "+elseResult+" to "+retLlvmType, ".cast_else");
		if (type != "unit")
			manager->write("store "+retLlvmType+" "+elseResult+", "+retLlvmType+"* "+resultPtr);
	}
	manager->write("br label %"+if_end);
	manager->decIndent();

	/* Get value */
	manager->writeLabel(if_end);
	if(type == "unit")
		return "";
	std::string ret = manager->write("load "+retLlvmType+"* "+resultPtr, ".if_result");
	manager->decIndent();
	return ret;
}

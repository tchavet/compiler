#include "CallNode.hpp"
#include "ClassNode.hpp"
#include "MethodNode.hpp"
#include "FormalNode.hpp"
#include "../semantic/Types.hpp"
#include <sstream>

CallNode::CallNode(int line, int column, ExprNode* objExpr, std::string name, std::vector<ExprNode*> args) : ExprNode(line, column)
{
	this->objExpr = objExpr;
	this->name = name;
	this->args = args;
	objExpr->setParent(this);
	for (int i=0; i<args.size(); i++)
		args[i]->setParent(this);
}

std::string CallNode::printTree(int tabsNb, bool types)
{
	return "Call(\n"
		+ tabs(tabsNb+1) + objExpr->printTree(tabsNb+1,types) + ",\n"
		+ tabs(tabsNb+1) + name + ",\n"
		+ tabs(tabsNb+1) + printList<ExprNode>(tabsNb+1,types,args) + "\n"
		+ tabs(tabsNb) + ")" + (types ? " : " + type : "");
}

ExprType* CallNode::getType()
{
	ExprType* exprType = new ExprType;
	/* Get type of objExpr, and verify that it exists */
	ExprType* objType = objExpr->getType();
	if (objType->type == "")
	{
		exprType->addErrors(objType->errors);
	}
	else
	{
		ClassNode* callingClass = Types::getNode(objType->type);
		if (callingClass)
		{
			/* Check that the method exists in class */
			MethodNode* methodNode = callingClass->getMethod(name);
			if (methodNode)
			{
				/* Check arguments */
				std::vector<FormalNode*> params = methodNode->getParams();
				// check number
				if (args.size() != params.size())
				{
					std::stringstream paramsSize;
					paramsSize << params.size();
					std::stringstream argsSize;
					argsSize << args.size();
					SemErr* semErr = new SemErr(line, column, "method " + name + " in " + callingClass->getName() + " expects " + paramsSize.str() + " arguments, " + argsSize.str() + " given");
					exprType->addError(semErr);
				}
				else
				{
					bool argErr = false;
					for (int i=0; i<args.size(); i++)
					{
						// check type
						ExprType* argType = args[i]->getType();
						if (argType->error)
							exprType->addErrors(argType->errors);
						if (argType->type != "")
						{
							ClassNode* argClass = Types::getNode(argType->type);
							if (!argClass && argType->type != params[i]->getType() || argClass && !argClass->isA(params[i]->getType()))
							{
								std::stringstream ss;
								ss << i+1;
								SemErr* semErr = new SemErr(line, column, "parameter " + ss.str() + " of method " + name + " in " + callingClass->getName() + " is of type " + params[i]->getType() + " but the given argument is of type " + argType->type);
								exprType->addError(semErr);
								argErr = true;
							}
						}
					}
					if (!argErr)
						exprType->type = methodNode->getReturnType();
				}
			}
			else
			{
				SemErr* semErr = new SemErr(line, column, "method " + name + " is not defined in class " + callingClass->getName());
				exprType->addError(semErr);
			}
		}
		else
		{
			SemErr* semErr = new SemErr(line, column, "expression is of non-defined class type " + objType->type);
			exprType->addError(semErr);
		}
	}
	type = exprType->type;
	return exprType;
}

std::string llvm(LlvmManager* manager)
{
	std::string function = manager->getFunction(objExpr->getComputedType(), name, objExpr->llvm(manager));
	MethodNode* methodNode = Types::getNode(objExpr->getComputedType())->getMethod(name);
	std::vector<FormalNode*> methodParams = methodNode->getParams();
	std::string llvm = "call fastcc "+LlvmManager::llvmType(methodNode->getReturnType())+" (";
	for (int i=0; i<methodParams.size(); i++)
	{
		llvm += methodParams[i]->getType();
		if (i < methodParams.size()-1)
			llvm += ", ";
	}
	llvm += ")* "+function+"(";
	for (int i=0; i<args.size(); i++)
	{
		std::string argLlvm = args[i]->llvm(manager);
		llvm += LlvmManager::llvmType(args[i]->getComputedType())+" "+argLlvm;
		if (i < args.size()-1)
			llvm += ", ";
	}
	llvm += ")";
	return manager.write(llvm);
}

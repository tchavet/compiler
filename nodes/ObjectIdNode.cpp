#include "ObjectIdNode.hpp"

ObjectIdNode::ObjectIdNode(int line, int column, std::string name) : ExprNode(line, column)
{
	this->name = name;
}

std::string ObjectIdNode::printTree(int tabsNb, bool types)
{
	return name + (types ? " : " + type : "");
}

ExprType* ObjectIdNode::getType()
{
	/* Get the type of the object */
	ExprType* exprType = new ExprType(getTypeInScope(name));
	/* If type is "", it means the id is undifined in this scope */
	if (exprType->type == "")
	{
		SemErr* semErr = new SemErr(line, column, "object id " + name + " is undefined in this scope");
		exprType->addError(semErr);
	}
	type = exprType->type;
	return exprType;
}

std::string ObjectIdNode::llvm(LlvmManager* manager, std::string retName)
{
	std::string var = getLlvmNameInScope(name);
	if (var != "")
		return var;
	// If it isn't defined locally, then it is an object field
	std::string objPtr = getLlvmNameInScope("obj.ptr");
	std::string varPtr = manager->getField(getTypeInScope("self"), name, objPtr);
	return manager->write("load "+LlvmManager::llvmType(type)+"* "+varPtr, retName);
}

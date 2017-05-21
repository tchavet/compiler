#ifndef OBJECTIDNODE_H
#define OBJECTIDNODE_H

#include "ExprNode.hpp"

/**
 * Node of a vsop object id.
 * An object id is a name that begins with a lower case letter
 * They represent variables
 */
class ObjectIdNode : public ExprNode
{
public:
	/**
	 * Constructor
	 * @see AstNode
	 * @param name The object id
	 */
	ObjectIdNode(int line, int column, std::string name);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager, std::string retName=".");

protected:
	std::string name; /**< The object id */
};
#endif

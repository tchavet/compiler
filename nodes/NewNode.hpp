#ifndef NEWNODE_H
#define NEWNODE_H

#include "ExprNode.hpp"

/**
 * Node representing a vsop 'new' expression, used to instantiate an object
 */
class NewNode : public ExprNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 * @param type The type of the variable to instantiate
	 */
	NewNode(int line, int column, std::string type);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager);

protected:
	std::string type; /**< The type to instantiate */
};
#endif

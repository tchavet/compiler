#ifndef ASSIGNNODE_H
#define ASSIGNNODE_H

#include "ExprNode.hpp"

/**
 * Node of an assign expression
 */
class AssignNode: public ExprNode
{
public:
	/**
	 * Constructor
	 *
	 * @see AstNode
	 * @param name The name of the variable being assigned
	 * @param expr The expression to assign
	 */
	AssignNode(int line, int column, std::string name, ExprNode* expr);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager);

protected:
	std::string name; /**< The name of the variable being assigned */
	ExprNode* expr; /**< The expression to assign */
};
#endif

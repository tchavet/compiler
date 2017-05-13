#ifndef UNOPNODE_H
#define UNOPNODE_H

#include "ExprNode.hpp"

/**
 * Node of a unary operator (operator applied to one expression)
 */
class UnOpNode : public ExprNode
{
public:
	/**
	 * Constructor 
	 * 
	 * @see AstNode
	 * @param op The unary operator
	 * @param expr The expression to apply the operator on
	 */
	UnOpNode(int line, int column, std::string op, ExprNode* expr);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager);

protected:
	std::string op; /**< The operator */
	ExprNode* expr; /**< The expression to apply the operator on */
};
#endif

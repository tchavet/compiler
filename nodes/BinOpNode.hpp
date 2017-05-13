#ifndef BINOPNODE_H
#define BINOPNODE_H

#include "ExprNode.hpp"

/**
 * A node representing an operation between 2 expressions
 */
class BinOpNode : public ExprNode
{
public:
	/**
	 * Constructor
	 *
	 * @see AstNode
	 * @param op The binary operator
	 * @param leftExpr The left expression
	 * @param rightExpr The right expression
	 */
	BinOpNode(int line, int column, std::string op, ExprNode* leftExpr, ExprNode* rightExpr);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();
	
	std::string llvm(LlvmManager* manager);

protected:
	std::string op; /**< The binary operator */
	ExprNode* leftExpr; /**< The left expression */
	ExprNode* rightExpr; /**< The right expression */
};
#endif

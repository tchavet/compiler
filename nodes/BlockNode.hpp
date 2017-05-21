#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "ExprNode.hpp"

/**
 * A block node
 */
class BlockNode : public ExprNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 */
	BlockNode(int line, int column);

	std::string printTree(int tabsNb=0, bool types=false);

	/**
	 * Add an expression to the block
	 *
	 * @param expression The expression to add
	 */
	void addExpression(ExprNode* expression);

	/**
	 * Add expressions to the block
	 *
	 * @param expressions A vector of the expressions to add
	 */
	void addExpressions(std::vector<ExprNode*>* expressions);

	ExprType* getType();

	std::string llvm(LlvmManager* manager, std::string retName=".");

protected:
	std::vector<ExprNode*> expressions; /**< The expressions composing the block */
};
#endif

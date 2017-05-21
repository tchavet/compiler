#ifndef IFNODE_H
#define IFNODE_H

#include "ExprNode.hpp"

/**
 * Node of an if expression
 */
class IfNode : public ExprNode
{
public:
	/**
	 * Constructor
	 * 
	 * @param cond The expression of the condition
	 * @param then The expression of the 'then' body
	 * @param els The expression of the 'else' body
	 */
	IfNode(int line, int column, ExprNode* cond, ExprNode* then, ExprNode* els = NULL);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager, std::string retName=".");

protected:
	ExprNode* cond; /**< The expression of the condition */
	ExprNode* then; /**< The expression of the 'then' body */
	ExprNode* els; /**< The expression of the 'else' body */
};
#endif

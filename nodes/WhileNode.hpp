#ifndef WHILENODE_H
#define WHILENODE_H

#include "ExprNode.hpp"

/**
 * Node of a vsop while loop
 */
class WhileNode : public ExprNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 * @param cond The conditional expression of the loop
	 * @param body The body of the loop
	 */
	WhileNode(int line, int column, ExprNode* cond, ExprNode* body);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager);

protected:
	ExprNode* cond; /**< The conditional expression of the loop */
	ExprNode* body; /**< The body of the loop */
};
#endif

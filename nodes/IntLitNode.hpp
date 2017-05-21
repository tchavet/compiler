#ifndef INTLITNODE_H
#define INTLITNODE_H

#include "ExprNode.hpp"

/**
 * A node representing an integer literal
 */
class IntLitNode : public ExprNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 * @param value The value of the literal
	 */
	IntLitNode(int line, int column, int value);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager, std::string retName=".");
protected:
	int value; /**< The value of the literal */
};
#endif

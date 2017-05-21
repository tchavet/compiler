#ifndef BOOLLITNODE_H
#define BOOLLITNODE_H

#include "ExprNode.hpp"

/**
 * A boolean literal node
 */
class BoolLitNode : public ExprNode
{
public:
	/**
	 * @see AstNode
	 * @param value The value of the boolean
	 */
	BoolLitNode(int line, int column, std::string value);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager, std::string retName=".");

protected:
	std::string value; /**< The value of the boolean literal */
};
#endif

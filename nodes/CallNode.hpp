#ifndef CALLNODE_H
#define CALLNODE_H

#include "ExprNode.hpp"

/**
 * A method call node
 */
class CallNode : public ExprNode
{
public:
	/**
	 * @see AstNode
	 * @param objExpr The expression representing the object on which to call the method
	 * @param name The name of the method to call
	 * @param args A vector of expressions representing the arguments
	 */
	CallNode(int line, int column, ExprNode* objExpr, std::string name, std::vector<ExprNode*> args);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string llvm(LlvmManager* manager, std::string retName=".");

protected:
	ExprNode* objExpr; /**< The expression representing the object on which to call the method */
	std::string name; /**< The name of the method to call */
	std::vector<ExprNode*> args; /**< The arguments given to the function call */
};
#endif

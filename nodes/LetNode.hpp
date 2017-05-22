#ifndef LETNODE_H
#define LETNODE_H

#include "ExprNode.hpp"

/**
 * A node representing a vsop let expression (used to declare a new variable)
 */
class LetNode : public ExprNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 * @param name The name of the variable being created
	 * @param type The vsop type of the variable
	 * @param scope The expression in which this variable can be used
	 * @param init The variable initialization expression
	 */
	LetNode(int line, int column, std::string name, std::string type, ExprNode* scope, ExprNode* init=NULL);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();

	std::string getTypeInScope(std::string id);

	std::string getLlvmVariable(std::string var, LlvmManager* manager);

	std::string llvm(LlvmManager* manager);
protected:
	std::string name; /**< The name of the variable being created */
	std::string letType; /**< The type of the variable */
	ExprNode* init; /**< The initialization expression */
	ExprNode* scope; /**< The expression in which this variable can be used */
	std::string llvmName; /**< The current llvm variable name of this variable */
};
#endif

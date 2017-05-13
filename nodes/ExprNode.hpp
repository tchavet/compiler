#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "AstNode.hpp"
#include "../semantic/structures.hpp"

/**
 * A node representing an expression
 */
class ExprNode : public AstNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 */
	ExprNode(int line=0, int column=0);

	/**
	 * Get the type of the expression, and run semantic checking.
	 * Recursivly run on the subnodes
	 * 
	 * @return An ExprType structure containing the type and the semantic errors that occured
	 */
	virtual ExprType* getType();

	virtual std::string printTree(int tabsNb=0, bool types=false);

	virtual std::string llvm(LlvmManager* manager);

	virtual void setLlvmNameInScope(std::string var, std::string llvmName);

	virtual std::string getLlvmNameInScope(std::string var);

	/**
	 * Get the type that was found by getType
	 * 
	 * @return The type of the expression
	 */
	std::string getComputedType();

protected:
	std::string type; /**< The type of the expression (need to run getType to get it)*/
};
#endif

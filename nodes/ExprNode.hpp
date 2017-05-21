#ifndef EXPRNODE_H
#define EXPRNODE_H

#include "AstNode.hpp"
#include "../semantic/structures.hpp"

class LlvmManager;
class AstNode;

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

	virtual std::string llvm(LlvmManager* manager, std::string retName="."){return "";};

	/**
	 * Get the type that was found by getType
	 * 
	 * @return The type of the expression
	 */
	virtual std::string getComputedType();

protected:
	std::string type; /**< The type of the expression (need to run getType to get it)*/
};
#endif

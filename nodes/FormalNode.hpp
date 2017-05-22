#ifndef FORMALNODE_H
#define FORMALNODE_H

#include "AstNode.hpp"

/**
 * A node representing a vsop formal (a type and a name, method parameters for example)
 */
class FormalNode : public AstNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 * @param name The name of the formal
	 * @param type The vsop type of the formal
	 */
	FormalNode(int line, int column, std::string name, std::string type);

	std::string printTree(int tabsNb=0, bool types=false);

	/**
	 * Get the name of the formal
	 * 
	 * @return The name of the formal
	 */
	std::string getName();

	/**
	 * Get the vsop type of the formal
	 * 
	 * @return The type of the formal
	 */
	std::string getType();

	void setLlvmNameInScope(std::string var, std::string llvmName);

	std::string getLlvmVariable(std::string var, LlvmManager* manager);

protected:
	std::string name; /**< The name of the formal */
	std::string type; /**< The vsop type of the formal */
	std::string llvmName; /**< The current llvm variable name */
};
#endif

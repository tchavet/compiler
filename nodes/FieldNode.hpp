#ifndef FIELDNODE_H
#define FIELDNODE_H

#include "AstNode.hpp"
#include "../semantic/structures.hpp"
#include "ExprNode.hpp"

/**
 * A node representing a vsop class field
 */
class FieldNode : public AstNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 * @param name The name of the field
	 * @param type The vsop type of the field
	 * @param init The field initialization expression
	 */
	FieldNode(int line, int column, std::string name, std::string type, ExprNode* init=NULL);

	std::string printTree(int tabsNb=0, bool types=false);

	/**
	 * Get the name of the field
	 * 
	 * @return The name of the field
	 */
	std::string getName();

	/**
	 * Get the type of the field
	 * 
	 * @return The type of the field
	 */
	std::string getType();

	std::vector<SemErr*> semCheck();

	std::string getTypeInScope(std::string id);

	void setLlvmNameInScope(std::string var, std::string llvmName);

	std::string getLlvmNameInScope(std::string var);
	
	std::string llvm(LlvmManager* manager);

protected:
	std::string name; /**< The name of the field */
	std::string type; /**< The vsop type of the field */
	ExprNode* init; /**< The initialization expression */
	std::string llvmName; /**< The current llvm name of the field */
};
#endif

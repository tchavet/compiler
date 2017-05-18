#ifndef METHODNODE_H
#define METHODNODE_H

#include "AstNode.hpp"
#include "FormalNode.hpp"
#include "BlockNode.hpp"
#include "../semantic/structures.hpp"

/**
 * A node representing a class method
 */
class MethodNode : public AstNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 * @param name The name of the method
	 * @param params The method parameters
	 * @param returnType The method return type
	 * @param body The method body
	 */
	MethodNode(int line, int column, std::string name, std::vector<FormalNode*> params, std::string returnType, BlockNode* body);

	std::string printTree(int tabsNb=0, bool types=false);

	std::string getTypeInScope(std::string id);

	/**
	 * Get the name of the method
	 * 
	 * @return The method's name
	 */
	std::string getName();

	/**
	 * Get the parameters of the method
	 * 
	 * @return A vector of the method's parameters
	 */
	std::vector<FormalNode*> getParams();

	std::vector<SemErr*> semCheck();

	std::string getReturnType();

	void setLlvmNameInScope(std::string var, std::string llvmName);

	std::string getLlvmNameInScope(std::string var);

	/**
	 * Write the llvm header for this method.
	 * - Write the definition of the type of the method
	 *   
	 * @param manager The LlvmManager
	 */
	void llvmHeader(LlvmManager* manager);

	/**
	 * Get the llvm variable representing the type of the method in llvm.
	 * 
	 * @return The llvm variable of the type of the method
	 */
	std::string getLlvmType();

	std::string llvm(LlvmManager* manager);

protected:
	std::string name; /**< The name of the method */
	std::vector<FormalNode*> params; /**< The method's parameters */
	std::string returnType; /**< The method's return type */
	BlockNode* body; /**< The method's body */
	std::string llvmType; /**< The llvm variable holding the type of the method */
	std::string objPtr; /**< The llvm variable name pointing to the object calling this method */
};
#endif

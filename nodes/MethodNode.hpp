#ifndef METHODNODE_H
#define METHODNODE_H

#include "AstNode.hpp"
#include "FormalNode.hpp"
#include "BlockNode.hpp"
#include "../semantic/structures.hpp"

class ClassNode;

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

	/**
	 * Get the node of the class this method is defined in
	 * 
	 * @return The class node
	 */
	ClassNode* getClass();

	/**
	 * Set the node of the class this method is defined in
	 * 
	 * @param classNode The class node
	 */
	ClassNode* setClass(ClassNode* classNode);

	std::vector<SemErr*> semCheck();

	std::string getReturnType();

	std::string getLlvmVariable(std::string var, LlvmManager* manager);

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

	/**
	 * Get the llvm variable representing the name of the method in llvm.
	 * 
	 * @return The llvm variable of the name of the method
	 */
	std::string getLlvmName();

	std::string llvm(LlvmManager* manager);

	/**
	 * Write the code to declare the function
	 * 
	 * @param manager The LlvmManager
	 */
	void declare(LlvmManager* manager);

protected:
	std::string name; /**< The name of the method */
	std::vector<FormalNode*> params; /**< The method's parameters */
	std::string returnType; /**< The method's return type */
	BlockNode* body; /**< The method's body */
	ClassNode* classNode; /**< The node of the class that this method is defined in */
	std::string llvmType; /**< The llvm variable holding the type of the method */
	std::string llvmName; /**< The llvm variable holding the name of the method */
	std::string objPtr; /**< The llvm variable name pointing to the object calling this method */
};
#endif

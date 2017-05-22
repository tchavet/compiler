#ifndef VSOPL_H
#define VSOPL_H

#include "../nodes/ClassNode.hpp"
#include "../LlvmManager.hpp"
#include <vector>

class Vsopl
{
public:
	/**
	 * Create the required ClassNodes of the vsop library
	 */
	static void init();

	/**
	 * Add the vsop library classes to the Types tables
	 */
	static void semantic();

	/**
	 * Create the llvm header for the vsop library classes
	 * 
	 * @param manager The LlvmManager
	 */
	static void llvmHeader(LlvmManager* manager);

	/**
	 * Instantiate the methods structures in the llvm main function
	 * 
	 * @param manager The LlvmManager
	 */
	static void llvmMain(LlvmManager* manager);

protected:
	static std::vector<ClassNode*> classes; /**< The vsop library classes */

	static void createObjectClass(); /**< Create the class node for the Object class */
	static void createIoClass(); /**< Create the class node for the IO class */
};

#endif

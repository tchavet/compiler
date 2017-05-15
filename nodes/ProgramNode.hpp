#ifndef PROGRAMNODE_H
#define PROGRAMNODE_H

#include "AstNode.hpp"
#include "ClassNode.hpp"

class LlvmManager;

/**
 * The root node of the program tree.
 * The program node is the parent of all the class nodes
 */
class ProgramNode : public AstNode
{
public:
	/**
	 * Constructor
	 */
	ProgramNode();

	std::string printTree(int tabsNb=0, bool types=false);

	/**
	 * Add a class node
	 * 
	 * @param classNode The class node to add
	 */
	void addClass(ClassNode* classNode);

	/**
	 * Add class nodes
	 * 
	 * @param classNodes A vector containing the class nodes to add
	 */
	void addClasses(std::vector<ClassNode*>* classNodes);

	/**
	 * Get the class nodes of the program
	 * 
	 * @return A vector containing the class node
	 */
	std::vector<ClassNode*> getClasses();

	/**
	 * Run semantic checking on the program.
	 * Semantic checking will be run on all the class nodes
	 * 
	 * @return A vector of all the semantic errors that occured
	 */
	std::vector<SemErr*> semCheck();

	/**
	 * Write the llvm of the program.
	 * Calls LlvmHeader and llvm for each class
	 * 
	 * @param LlvmManager manager The llvm manager
	 * 
	 * @return An empty string
	 */
	std::string llvm(LlvmManager* manager);

protected:
	std::vector<ClassNode*> classes; /**< The nodes representing the classes of the program */
};
#endif

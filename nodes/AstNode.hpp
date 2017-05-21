#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <string>
#include "../LlvmManager.hpp"

class LlvmManager;

/**
 * A syntactic tree node
 */
class AstNode
{
public:
	/**
	 * Constructor
	 *
	 * @param line The line number where the statement begins
	 * @param column The column numbre where the statement begins
	 */
	AstNode(int line=0, int column=0);

	/**
	 * Print the syntactic tree of this node
	 *
	 * @param tabsNb The number of tabs to indent the tree
	 * @param types Display the expression types or not
	 * 
	 * @return The tree
	 */
	virtual std::string printTree(int tabsNb=0, bool types=false) {};

	/**
	 * Get the line number of the node
	 *
	 * @return The line number
	 */
	int getLine();

	/**
	 * Get the column number of the node
	 *
	 * @return The column number
	 */
	int getColumn();

	/**
	 * Set the parent node
	 *
	 * @param parent The parent node of this node
	 */
	void setParent(AstNode* parent);

	/**
	 * Get the parent node
	 * 
	 * @return A pointer to the parent node
	 */
	AstNode* getParent();

	/**
	 * Get the type of a variable in the scope where this node is located
	 *
	 * @param id The name of the variable
	 *
	 * @return The type of the variable. If this variable was not found, "" is returned
	 */
	virtual std::string getTypeInScope(std::string id);

	/**
	 * Convert this node into llvm
	 *
	 * @param manager The LlvmManager
	 * @param retName The name of the llvm variable to assign this expression to
	 *
	 * @return The local llvm variable name that this node's value was assigned to (if the node has a value)
	 */
	virtual std::string llvm(LlvmManager* manager, std::string retName = "."){};

	/**
	 * Set the current local llvm variable name of a variable in this scope (example: x is x.5)
	 *
	 * @param var The vsop variable to set the llvm name of
	 * @param llvmName The llvm variable name
	 */
	virtual void setLlvmNameInScope(std::string var, std::string llvmName);

	/**
	 * Get the current local llvm variable name of a variable in this scope
	 *
	 * @param var The vsop variable name
	 *
	 * @return The current llvm variable name in this scope
	 */
	virtual std::string getLlvmNameInScope(std::string var);

protected:
	int line; /**< The line number where this node's vsop code begins */
	int column; /**< The column number where this node's vsop code begins */
	AstNode* parent; /**< The parent node of this node */

	/**
	 * Get a string made of <tabsNb> tabs
	 * 
	 * @param tabsNb The number of tabs to produce
	 * 
	 * @return A string composed of the given number of tabs
	 */
	std::string tabs(int tabsNb);

	/**
	 * Print the tree of a list of nodes
	 * 
	 * @param tabsNb The number of tabs to indent the tree
	 * @param types Display the expression types or not
	 * @param nodes A vector of the nodes to print the tree of
	 * 
	 * @return A string of the nodes' trees
	 */
	template <class T>
	std::string printList(int tabsNb,bool types, std::vector<T*> nodes)
	{
		std::string print="[\n";
		for (int i=0; i<nodes.size(); i++)
		{
			print += tabs(tabsNb+1) + nodes[i]->printTree(tabsNb+1,types);
			if (i != nodes.size()-1)
				print += ",";
			print += "\n";
		}
		print += tabs(tabsNb) + "]";
		return print;
	}
};
#endif

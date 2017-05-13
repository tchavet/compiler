#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../nodes/ProgramNode.hpp"
#include "Types.hpp"

/**
 * Class doing semantic checking
 */
class Semantic
{
public:
	/**
	 * Constructor
	 * 
	 * @param filename The name of the vsop file being checked (used in error output)
	 * @param tree The root node of the program
	 */
	Semantic(std::string filename, ProgramNode* tree);

	/**
	 * Run semantic checking for classes
	 * 
	 * Checks:
	 *   - Class redefinition
	 *   - Parent definition
	 *   - Inheritance cycles
	 *   - Main class and method prototype
	 *   
	 * @return true if there was no error
	 */
	bool classesCheck();

	/**
	 * Run semantic checking of the classes' content
	 * 
	 * @return true if there was no error
	 */
	bool scopeCheck();

protected:
	/**
	 * Print a semantic error
	 * 
	 * @param line The line where the error occured
	 * @param column The column where the error occured
	 * @param message The additionnal optionnal message to print
	 */
	void semanticError(int line, int column, std::string message="");

	std::string filename; /**< The name of the vsop file being checked (used in error output) */
	ProgramNode* astTree; /**< The root node of the program */
};
#endif

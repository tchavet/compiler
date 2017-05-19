#ifndef STRINGLITNODE_H
#define STRINGLITNODE_H

#include "ExprNode.hpp"

/**
 * A node of a string literal (a string defined in the source code using "")
 */
class StringLitNode : public ExprNode
{
public:
	/**
	 * Constructor
	 * 
	 * @see AstNode
	 * @param str The string
	 */
	StringLitNode(int line, int column, std::string str);

	std::string printTree(int tabsNb=0, bool types=false);

	ExprType* getType();
	std::string llvm(LlvmManager* manager);
protected:
	std::string str; /**< The string */
	std::string llvmStr; /**< The llvm compliant string */
	int llvmStrSize; /**< The size of the llvm string */
};
#endif

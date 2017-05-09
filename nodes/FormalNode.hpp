#ifndef FORMALNODE_H
#define FORMALNODE_H

#include "AstNode.hpp"

class FormalNode : public AstNode
{
public:
	FormalNode(int line, int column, std::string name, std::string type);
	std::string printTree(int tabsNb=0, bool types=false);
	std::string getName();
	std::string getType();
	void setLlvmNameInScope(std::string var, std::string llvmName);
	std::string getLlvmNameInScope(std::string var);

protected:
	std::string name;
	std::string type;
	std::string llvmName;
};
#endif

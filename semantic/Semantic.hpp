#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../nodes/ProgramNode.hpp"
#include "Types.hpp"

class Semantic
{
public:
	Semantic(std::string filename, ProgramNode* tree);
	bool classesCheck();
	bool scopeCheck();

protected:
	void semanticError(int line, int column, std::string message="");
	std::string filename;
	ProgramNode* astTree;
};
#endif

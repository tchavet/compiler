#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "Scope.hpp"
#include "../nodes/ProgramNode.hpp"
#include "Types.hpp"

class Semantic
{
public:
	Semantic(std::string filename, ProgramNode* tree);
	bool classesCheck();
	bool scopeCheck();
	bool typeCheck();

protected:
	void semanticError(int line, int column, std::string message="");
	std::string filename;
	Scope* rootScope;
	Scope* currentScope;
	ProgramNode* astTree;
};
#endif

#ifndef SCOPE_H
#define SCOPE_H

#include "../nodes/AstNode.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class Scope
{
public:
	Scope(Scope* parent=NULL);
	bool add(std::string id, AstNode* node);
	bool defined(std::string id);
	AstNode* find(std::string id);

protected:
	std::unordered_map<std::string, AstNode*> table;
	Scope* parent;
	std::vector<Scope*> children;
};
#endif

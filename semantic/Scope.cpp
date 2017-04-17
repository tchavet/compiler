#include "Scope.hpp"

Scope::Scope(Scope* parent)
{
	table = std::unordered_map<std::string, AstNode*>();
	this->parent = parent;
	children = std::vector<Scope*>();
}

bool Scope::add(std::string id, AstNode* node)
{
	if (table.count(id))
		return false;
	table.insert({{id, node}});
}

bool Scope::defined(std::string id)
{
	if (table.count(id))
		return true;
	else if (parent)
		return parent->defined(id);
	else
		return false;
}

AstNode* Scope::find(std::string id)
{
	AstNode* node = table[id];
	if (node)
		return node;
	else if (parent)
		return parent->find(id);
	else
		return NULL;
}

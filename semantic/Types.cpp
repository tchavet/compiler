#include "Types.hpp"

std::unordered_map<std::string, ClassNode*> Types::table = std::unordered_map<std::string, ClassNode*>();

void Types::init(std::vector<std::string> builtIns)
{
	for (int i=0; i<builtIns.size(); i++)
	{
		Types::table.insert({{builtIns[i], NULL}});
	}
}

bool Types::add(std::string id, ClassNode* node)
{
	if (Types::table.count(id))
		return false;
	Types::table.insert({{id, node}});
	return true;
}

bool Types::defined(std::string id)
{
	return Types::table.count(id);
}

ClassNode* Types::getNode(std::string id)
{
	return Types::table[id];
}

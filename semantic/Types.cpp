#include "Types.hpp"

Types::Types(std::vector<std::string> builtIns)
{
	table = std::unordered_map<std::string, ClassNode*>();
	for (int i=0; i<builtIns.size(); i++)
	{
		table.insert({{builtIns[i], NULL}});
	}
}

bool Types::add(std::string id, ClassNode* node)
{
	if (table.count(id))
		return false;
	table.insert({{id, node}});
	return true;
}

bool Types::defined(std::string id)
{
	return table.count(id);
}

ClassNode* Types::getNode(std::string id)
{
	return table[id];
}

#include "Types.hpp"
#include <algorithm>

std::unordered_map<std::string, ClassNode*> Types::table = std::unordered_map<std::string, ClassNode*>();
std::vector<std::string> Types::primitives = std::vector<std::string>();

void Types::init(std::vector<std::string> builtIns)
{
	for (int i=0; i<builtIns.size(); i++)
	{
		Types::table.insert({{builtIns[i], NULL}});
	}
	Types::primitives = builtIns;
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

bool Types::isPrimitive(std::string id)
{
	return std::find(Types::primitives.begin(), Types::primitives.end(), id) != Types::primitives.end();
}

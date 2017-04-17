#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <unordered_map>
#include "../nodes/ClassNode.hpp"

class Types
{
public:
	Types(std::vector<std::string> builtIns = std::vector<std::string>());
	/**
	 * Add a type to the type table
	 *
	 * @param id The id of the type
	 * @param node A pointer to the class node defining the type
	 *
	 * @return False if the type already exists
	 */
	bool add(std::string id, ClassNode* node);

	/** check if a type is defined
	 */
	bool defined(std::string id);

	/**
	 * Get the ClassNode of a type
	 */
	ClassNode* getNode(std::string id);

protected:
	std::unordered_map<std::string, ClassNode*> table;
};
#endif

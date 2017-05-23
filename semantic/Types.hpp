#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <unordered_map>
#include "../nodes/ClassNode.hpp"

/**
 * Static class containing the functions for vsop types managing
 */
class Types
{
public:
	/**
	 * A hash table containing all the defined vsop types (built in and defined classes)
	 * The key is the type name, the value is a pointer to the class node (for built in types the pointer is null)
	 */
	static std::unordered_map<std::string, ClassNode*> table;

	/**
	 * Initialize the types table
	 * 
	 * @param builtIns The built in vsop types
	 */
	static void init(std::vector<std::string> builtIns = std::vector<std::string>());

	/**
	 * Add a type to the type table
	 *
	 * @param id The id of the type
	 * @param node A pointer to the class node defining the type
	 *
	 * @return false if the type already exists
	 */
	static bool add(std::string id, ClassNode* node);

	/**
	 * check if a type is defined
	 * 
	 * @param id The name of the type
	 * 
	 * @return true if the type is defined
	 */
	static bool defined(std::string id);

	/**
	 * Get the ClassNode of a type
	 * 
	 * @param id The name of the type
	 * 
	 * @return The class node of that type
	 */
	static ClassNode* getNode(std::string id);

	/**
	 * Check if a type is a primitive type or not
	 * 
	 * @param id The name of the type
	 * 
	 * @return True if it is a primitive type, false otherwise
	 */
	static bool isPrimitive(std::string id);

protected:
	static std::vector<std::string> primitives;
};
#endif

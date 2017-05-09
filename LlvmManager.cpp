#include "LlvmManager.hpp"

std::string LlvmManager::llvmType(std::string type)
{
	if (type == "bool")
		return "i1";
	else if (type == "int32")
		return "i32";
	else if (type == "string")
		return "i8*";
	else if (type == "unit")
		return "void";
	else
		return "%class."+type;
}

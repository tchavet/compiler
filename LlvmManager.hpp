#ifndef LLVMMANAGER_H
#define LLVMMANAGER_H

#include <string>
#include "unordered_map.hpp"
#include "ClassNode.hpp"

typedef std::unordered_map<std::string,stringmap*> classmap;

class LlvmManager
{
	public:
		LlvmManager(ProgramNode pgrn, outstream out, std::string moduleID = "main.vsop");
		std::string write(std::string towrite, std::string ret = "");
		std::string writeLabel(std::string label);
		std::string getFunction(std::string className, std::string fctName, std::string obj);

	protected:
		classmap cm = classmap();
		classmap cfm = classmap();
};
#endif

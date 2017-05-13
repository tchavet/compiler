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
		void writeLabel(std::string label);
		std::string getNewLabel(std::string label);
		std::string getFunction(std::string className, std::string fctName, std::string obj);

	protected:
		classmap cm = classmap();
		classmap cfm = classmap();
		std::string getField(std::string className, std::string fieldName, std::string obj);
		static std::string llvmType(std::string type);
};
#endif

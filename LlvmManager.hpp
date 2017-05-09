#ifndef LLVMMANAGER_H
#define LLVMMANAGER_H

#include <string>

class LlvmManager
{
	public:
		std::string write(std::string towrite, std::string ret = "");
		void writeLabel(std::string label);
		std::string getNewLabel(std::string label);
		std::string getFunction(std::string className, std::string fctName, std::string obj);
		std::string getField(std::string className, std::string fieldName, std::string obj);
		static std::string llvmType(std::string type);
};
#endif

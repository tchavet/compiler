#ifndef LLVMMANAGER_H
#define LLVMMANAGER_H

#include <string>

class LlvmManager
{
	public:
		std::string write(std::string towrite, std::string ret = "");
		std::string writeLabel(std::string label);
		std::string getFunction(std::string className, std::string fctName, std::string obj);
};
#endif

#include "StringLitNode.hpp"
#include <sstream>
#include <iostream>

StringLitNode::StringLitNode(int line, int column, std::string str) : ExprNode(line, column)
{
	this->str = str;
	llvmStrSize = 0;
	llvmStr = str;
	for (int i=0; i<llvmStr.size(); i++)
	{
		llvmStrSize++;
		if (llvmStr[i] == '\\' && llvmStr[i+1] == 'x')
		{
			if (llvmStr[i+1] == 'x')
			{
				llvmStr.erase(i+1,1);
			}
			else if (llvmStr[i+1] == '\\')
			{
				llvmStr.replace(i+1, 1, "5C");
			}
			else if (llvmStr[i+1] == '"')
			{
				llvmStr.replace(i+1, 1, "22");
			}
			i+=2;
		}
	}
	llvmStr += "\00";
	llvmStrSize++;
}

std::string StringLitNode::printTree(int tabsNb, bool types)
{
	return "\"" + str + "\"" + (types ? " : " + type : "");
}

ExprType* StringLitNode::getType()
{
	type = "string";
	return new ExprType("string");
}
std::string StringLitNode::llvm(LlvmManager* manager)
{
	std::ostringstream oss;
	oss << "constant [" << llvmStrSize << " x i8] c\"" << llvmStr << "\\00\"";
	std::string stringCst = manager->addCst(oss.str(), ".");
	oss = std::ostringstream();
	oss << "getelementptr [" << llvmStrSize << " x i8]* " << stringCst << ", i32 0, i32 0";
	std::string stringPtr = manager->write(oss.str(), ".");
	return stringPtr;
}

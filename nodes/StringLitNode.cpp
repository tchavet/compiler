#include "StringLitNode.hpp"
#include <sstream>

StringLitNode::StringLitNode(int line, int column, std::string str) : ExprNode(line, column)
{
	this->str = str;
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

	int sizeString = this->str.size();
/*	std::string stringPtr = manager->write("alloca i8*, align 8",".");
	
	//WITHOUT DEFINING the @.str before... Maybe there is a need !
	oss << "store i8* getelementptr inbounds [" << sizeString  << " x i8], [" << sizeString << " x i8]*c\"" << this->str << "\\00\" align 1";
*/
	oss << "constant [" << sizeString << " x i8] c\"" << str << "\\00\"";
	std::string stringCst = manager->addCst(oss.str(), ".");
	oss = std::ostringstream();
	oss << "getelementptr [" << sizeString << " x i8]* " << stringCst << ", i32 0, i32 0";
	std::string stringPtr = manager->write(oss.str(), ".");
	return stringPtr;
}

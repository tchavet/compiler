#ifndef LLVMMANAGER_H
#define LLVMMANAGER_H

#include <string>
#include <unordered_map>
#include <ostream>
#include <vector>

class ProgramNode;
class ClassNode;

typedef std::unordered_map<std::string,int> stringmap;
typedef std::unordered_map<std::string,stringmap*> classmap;

class LlvmManager
{
	public:
		LlvmManager(ProgramNode *programNode, std::vector<std::ostream*> outs, std::string moduleId = "main.vsop");
		LlvmManager(ProgramNode *programNode, std::ostream *out, std::string moduleId = "main.vsop");
		std::string write(std::string toWrite, std::string ret = "");
		void writeLabel(std::string llvmLabel);
		std::string getNewLabel(std::string label);
		std::string getFunction(std::string className, std::string functionName, std::string object);
		std::string getField(std::string className, std::string fieldName, std::string object);
		static std::string llvmType(std::string type);

	protected:
		bool indent;
		classmap methodsMap = classmap(); //for the functions
		classmap fieldsMap = classmap(); //for the fields//
		stringmap llvmVars; 
		stringmap llvmLabels; 
		std::vector<std::ostream*> outputs;
		void implementMethodsVector(ClassNode *classNode);
};
#endif

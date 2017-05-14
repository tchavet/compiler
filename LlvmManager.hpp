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
		LlvmManager(ProgramNode *pgrn, std::vector<std::ostream*> outs, std::string moduleID = "main.vsop");
		LlvmManager(ProgramNode *pgrn, std::ostream *out, std::string moduleID = "main.vsop");
		std::string write(std::string towrite, std::string ret = ".");
		void writeLabel(std::string llvmLabel);
		std::string getNewLabel(std::string label);
		std::string getFunction(std::string className, std::string fctName, std::string obj);
		std::string getField(std::string className, std::string fieldName, std::string obj);
		static std::string llvmType(std::string type);

	protected:
		bool indent;
		classmap cm = classmap(); //for the functions
		classmap cfm = classmap(); //for the fields//
		stringmap llvmVars; 
		stringmap llvmLabels; 
		std::vector<std::ostream*> outputs;
		void implementMethodsVector(ClassNode *cn);
};
#endif

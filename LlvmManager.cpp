#include "LlvmManager.hpp"
#include "nodes/ProgramNode.hpp"

using namespace std;

LlvmManager::LlvmManager(ProgramNode *programNode, vector<ostream*> outs, std::string moduleId)
{
	indent = false;
	outputs = outs;
	llvmVars = stringmap();
	std::vector<ClassNode*> classes = programNode->getClasses();
	
	write("; ModuleID = '"+moduleId +"'");
	
	for(vector<ClassNode*>::const_iterator it = classes.cbegin(); it != classes.cend(); ++it)
	{
		ClassNode* classNode = *it;
		stringmap* methods = classNode->getAllMethods();
		methodsMap.emplace(classNode->getName(), methods);

		stringmap* fields = classNode->getAllMethods();
		fieldsMap.emplace(classNode->getName(), fields);
		
		string methodsStruct = "%methods.type."+classNode->getName()+" = type {";
		string classStruct = "%class."+classNode->getName()+" type {%methods.type."+classNode->getName()+"*";
		vector<string> methodNames(methods->size());
		for(stringmap::const_iterator jt = fields->cbegin(); jt != fields->cend(); ++jt)
		{
			methodNames[jt->second] = jt->first;
		}
		for(int i = 0; i < methodNames.size(); ++i)
		{
			if(i != 0)
			{
				methodsStruct += ", ";
			}

			MethodNode* methodNode = classNode->getMethod(methodNames[i]);
			methodsStruct += llvmType(methodNode->getReturnType()) + " (";
			std::vector<FormalNode*> params = methodNode->getParams();
			for(int j=0; params.size(); ++j)
			{
				if(j != 0)
				{
					methodsStruct += ", ";
				}

				methodsStruct += llvmType(params[j]->getType());
			}
			methodsStruct += ")*";
		}

		methodsStruct += "}";
		write(methodsStruct);

		write("%methods."+classNode->getName()+" = alloca %methods.type."+classNode->getName());

		implementMethodsVector(classNode);


		string clasStruct = "";
		vector<string> fieldNames(fields->size());
		for(stringmap::const_iterator jt = fields->cbegin(); jt != fields->cend(); ++jt)
		{
			fieldNames[jt->second] = jt->first;
		}

		for(int i = 0; i < fieldNames.size(); ++i)
		{
			clasStruct += ", ";

			clasStruct += llvmType(classNode->getTypeInScope(fieldNames[i]));
		}
	
		clasStruct += "}";

		write(clasStruct);
	}
}

LlvmManager::LlvmManager(ProgramNode *programNode, ostream* out, std::string moduleId)
{
	LlvmManager(programNode, vector<ostream*>(1,out), moduleId);
}

std::string LlvmManager::write(std::string toWrite, std::string ret)
{
	string var = "";
	if(ret != ".")
	{
		stringmap::iterator it = llvmVars.find(ret);
		if(it == llvmVars.end())
		{
			it = (llvmVars.emplace(ret,0)).first;
		}
		else 
		{
			++(it->second);
		}
		var = "%"+it->first+"."+to_string(it->second);
		toWrite = var+" = "+toWrite;

	}	

	if(indent)
	{
		toWrite = "\t" + toWrite;
	}

	toWrite += "\n";

	for(vector<ostream*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
	{
		(*it)->write(toWrite.c_str(), toWrite.size());
	}

	return var;
}

void LlvmManager::writeLabel(std::string llvmLabel)
{
	indent = true;
	
	llvmLabel += ":\n";

	for(vector<ostream*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
	{
		(*it)->write(llvmLabel.c_str(), llvmLabel.size());
	}
}

std::string LlvmManager::getNewLabel(std::string label)
{
	stringstream ss();
	
	stringmap::iterator it = llvmLabels.find(label);
	if(it == llvmLabels.end())
	{
		it = (llvmLabels.emplace(label,0)).first;
	}
	else
	{
		++(it->second);
	}

	return it->first+to_string(it->second);

}

std::string LlvmManager::getFunction(std::string className, std::string methodName, std::string object)
{
	int methodPos = (*(methodsMap[className]))[methodName];
	return write("getelementptr class."+className+"* "+object+", i32 0, i32 0, i32 0, i32 "+to_string(methodPos), "");
}

std::string LlvmManager::getField(std::string className, std::string fieldName, std::string object)
{
	int fieldPos = (*(fieldsMap[fieldName]))[fieldName]+1; // +1 because the struct start with a pointer to the methods
	return write("getelementptr class."+className+"* "+object+", i32 0, i32 "+to_string(fieldPos), "");
}

void LlvmManager::implementMethodsVector(ClassNode *classNode)
{
	
}

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

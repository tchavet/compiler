#include "LlvmManager.hpp"
#include "nodes/ProgramNode.hpp"
#include <iostream>

using namespace std;

LlvmManager::LlvmManager(vector<ostream*> outs, std::string moduleId)
{
	indent = false;
	outputs = outs;
	llvmVars = stringmap();
	
	write("; ModuleID = '"+moduleId +"'");
}

LlvmManager::LlvmManager(ostream* out, std::string moduleId) : LlvmManager(vector<ostream*>(1,out), moduleId)
{
}

std::string LlvmManager::write(std::string toWrite, std::string ret)
{
	string var = "";
	if(ret != "")
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
		if (ret == ".")
			var = "%"+to_string(it->second);
		else
			var = "%"+it->first+"."+to_string(it->second);
		toWrite = var+" = "+toWrite;

	}	

	if(indent)
	{
		toWrite = "\t" + toWrite;
	}

	for(vector<ostream*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
	{
		(**it) << toWrite << endl;
//		(*it)->flush();
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

void LlvmManager::addClass(std::string className, stringmap* methods, stringmap* fields)
{
	methodsMap.emplace(className, methods);
	fieldsMap.emplace(className, fields);
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

#include "LlvmManager.hpp"
#include "nodes/ProgramNode.hpp"
#include "nodes/ClassNode.hpp"
#include "semantic/Types.hpp"
#include <iostream>

using namespace std;

LlvmManager::LlvmManager(vector<ostream*> outs, std::string moduleId)
{
	indent = 0;
	outputs = outs;
	llvmVars = stringmap();
	
	write("; ModuleID = '"+moduleId +"'");
	write("");
}

LlvmManager::LlvmManager(ostream* out, std::string moduleId) : LlvmManager(vector<ostream*>(1,out), moduleId)
{
}

std::string LlvmManager::write(std::string toWrite, std::string ret, bool global)
{
	string var = global ? "@" : "%";
	if(ret != "")
	{
		stringmap::iterator it = llvmVars.find(ret);
		if(it == llvmVars.end())
		{
			it = (llvmVars.emplace(ret,1)).first;
		}
		else 
		{
			++(it->second);
		}
		if (ret == ".")
			var += ret + to_string(it->second);
		else
			var += it->first+"."+to_string(it->second);
		toWrite = var+" = "+toWrite;

	}	

	if(indent)
	{
		toWrite = string(indent,'\t') + toWrite;
	}

	for(vector<ostream*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
	{
		(**it) << toWrite << endl;
//		(*it)->flush();
	}

	return var;
}

std::string LlvmManager::addCst(std::string toWrite, std::string ret)
{
	string var = "@";
	if(ret != "")
	{
		stringmap::iterator it = llvmVars.find(ret);
		if(it == llvmVars.end())
		{
			it = (llvmVars.emplace(ret,1)).first;
		}
		else 
		{
			++(it->second);
		}
		if (ret == ".")
			var += ret + to_string(it->second);
		else
			var += it->first+"."+to_string(it->second);
		toWrite = var+" = "+toWrite;
	}
	constants.push_back(toWrite);
	return var;
}

void LlvmManager::writeConstants()
{
	for (int i=0; i<constants.size(); i++)
	{
		for(vector<ostream*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
		{
			(**it) << constants[i] << endl;
			(*it)->flush();
		}
	}
}

std::string LlvmManager::getNewVarName(std::string name)
{
	stringmap::iterator it = llvmVars.find(name);
	if(it == llvmVars.end())
	{
		it = (llvmVars.emplace(name,1)).first;
	}
	else 
	{
		++(it->second);
	}
	return "%"+it->first+"."+to_string(it->second);
}

void LlvmManager::writeLabel(std::string llvmLabel)
{
	llvmLabel += ":";

/*	for(vector<ostream*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
	{
		(*it)->write(llvmLabel.c_str(), llvmLabel.size());
	}
*/
	write(llvmLabel);
	incIndent();
}

std::string LlvmManager::getNewLabel(std::string label)
{
	stringstream ss();
	
	stringmap::iterator it = llvmLabels.find(label);
	if(it == llvmLabels.end())
	{
		it = (llvmLabels.emplace(label,1)).first;
	}
	else
	{
		++(it->second);
	}

	return it->first+to_string(it->second);

}

std::string LlvmManager::getFunction(std::string className, std::string methodName, std::string methodClassName, std::string object)
{
	int methodPos = methodsMap[className][methodName];
	std::string ptr2vector = write("getelementptr %class."+className+"* "+object+", i32 0, i32 0", ".");
	std::string vector = write("load %methods.type."+className+"** "+ptr2vector, ".");
	std::string ptr2ptr2method = write("getelementptr %methods.type."+className+"* "+vector+", i32 0, i32 "+to_string(methodPos), ".");
	return write("load %method.type."+methodClassName+"."+methodName+"* "+ptr2ptr2method, className+"."+methodName);
}

std::string LlvmManager::getField(std::string className, std::string fieldName, std::string object)
{
	int fieldPos = fieldsMap[className][fieldName]+1; // +1 because the struct start with a pointer to the methods
	return write("getelementptr %class."+className+"* "+object+", i32 0, i32 "+to_string(fieldPos), className+"."+fieldName);
}

void LlvmManager::addClass(std::string className, stringmap methods, stringmap fields)
{
	methodsMap.emplace(className, methods);
	fieldsMap.emplace(className, fields);
}

void LlvmManager::beginMain()
{
	write("define fastcc i32 @main()");
	write("{");
	incIndent();
}

void LlvmManager::endMain()
{
	std::string mainObj = Types::getNode("Main")->llvmAllocate(this);
	std::string mainRet = write("call fastcc i32 @method.Main.main(%class.Main* "+mainObj+")", ".main_result");
	write("ret i32 "+mainRet);
	decIndent();
	write("}");
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
		return "%class."+type+"*";
}

void LlvmManager::incIndent()
{
	indent++;
}

void LlvmManager::decIndent()
{
	indent--;
}

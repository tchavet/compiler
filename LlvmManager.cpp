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
	
	//Start the file by writing the module name
	write("; ModuleID = '"+moduleId +"'");
	write("");
}

LlvmManager::LlvmManager(ostream* out, std::string moduleId) : LlvmManager(vector<ostream*>(1,out), moduleId)
{
}

std::string LlvmManager::write(std::string toWrite, std::string ret, bool global)
{
	// The variable name must start with its locality indicator: @ for global and % for local
	string var = global ? "@" : "%";
	if(ret != "")
	{
		stringmap::iterator it = llvmVars.find(ret);
		
		// The variable is not yet inside the table
		if(it == llvmVars.end())
		{
			it = (llvmVars.emplace(ret,1)).first;
		}
		// The variable is inside the table and thus, its value must be incremented
		else 
		{
			++(it->second);
		}

		// "." correspond to the case where it is an unnamed variable
		if (ret == ".")
			var += ret + to_string(it->second);
		else
			var += it->first+"."+to_string(it->second);
		toWrite = var+" = "+toWrite;

	}	

	//Indent the number of times it must.
	if(indent)
	{
		toWrite = string(indent,'\t') + toWrite;
	}

	//Write to each outputs specified when instanciated
	for(vector<ostream*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
	{
		(**it) << toWrite << endl;
//		(*it)->flush();
	}

	return var;
}

std::string LlvmManager::addCst(std::string toWrite, std::string ret)
{
	//Constants are global variables
	string var = "@";
	if(ret != "")
	{
		stringmap::iterator it = llvmVars.find(ret);
		
		// the return variable is not in the table yet
		if(it == llvmVars.end())
		{
			it = (llvmVars.emplace(ret,1)).first;
		}
		// The variable is already in the table and thus, its value must be incremented
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
	// add the line to the vector that'll be written at the end of the file 
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
	// The variable is not yet inside the table
	if(it == llvmVars.end())
	{
		it = (llvmVars.emplace(name,1)).first;
	}
	// The variable is inside the table and thus, its value must be incremented
	else 
	{
		++(it->second);
	}
	return "%"+it->first+"."+to_string(it->second);
}

void LlvmManager::writeLabel(std::string llvmLabel)
{
	// Label names must be followed by :
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
	// The Label name is not yet inside the table
	if(it == llvmLabels.end())
	{
		it = (llvmLabels.emplace(label,1)).first;
	}
	// The label name is inside the table and thus, its value must be incremented
	else
	{
		++(it->second);
	}

	return it->first+to_string(it->second);

}

std::string LlvmManager::getFunction(std::string className, std::string methodName, std::string methodClassName, std::string object)
{
	//Get the position of the function in the methods vector of the specified class
	int methodPos = methodsMap[className][methodName];
	//Get the pointer to the methods vector wich is the first element of the object structure
	std::string ptr2vector = write("getelementptr %class."+className+"* "+object+", i32 0, i32 0", ".");
	//Load the pointer representing the vector
	std::string vector = write("load %methods.type."+className+"** "+ptr2vector, ".");
	//Get the pointer to the methods, put it inside an unnamed variable and return it
	std::string ptr2ptr2method = write("getelementptr %methods.type."+className+"* "+vector+", i32 0, i32 "+to_string(methodPos), ".");
	return write("load %method.type."+methodClassName+"."+methodName+"* "+ptr2ptr2method, className+"."+methodName);
}

std::string LlvmManager::getField(std::string className, std::string fieldName, std::string object)
{
	//Get the position of the field in the structure of the specified class
	//The Class structure starting by the methods vector pointer, the value must be incremented by 1
	int fieldPos = fieldsMap[className][fieldName]+1;
	//Put the pointer to the field in an unnamed variable and return it
	return write("getelementptr %class."+className+"* "+object+", i32 0, i32 "+to_string(fieldPos), className+"."+fieldName);
}

void LlvmManager::addClass(std::string className, stringmap methods, stringmap fields)
{
	methodsMap.emplace(className, methods);
	fieldsMap.emplace(className, fields);
}

void LlvmManager::main()
{
	write("define fastcc i32 @main()");
	write("{");
	incIndent();
	//Instantiate the Main Class to call its method main and start the program
	std::string mainObj = Types::getNode("Main")->llvmAllocate(this);
	std::string mainRet = write("call fastcc i32 @method.Main.main(%class.Main* "+mainObj+")", ".main_result");
	//The program return the value returned by the Main
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

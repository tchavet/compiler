#include "LlvmManager.hpp"
#include "nodes/ProgramNode.hpp"

using namespace std;

LlvmManager::LlvmManager(ProgramNode *pgrn, vector<ostream*> outs, std::string moduleID)
{
	indent = false;
	outputs = outs;
	llvmVars = stringmap();
	std::vector<ClassNode*> classes = pgrn->getClasses();
	
	write("; ModuleID = '"+moduleID +"'");
	
	for(vector<ClassNode*>::const_iterator it = classes.cbegin(); it != classes.cend(); ++it)
	{
		stringmap* fs = (*it)->getFuncStruct();
		cm.emplace((*it)->getName(), fs);

		stringmap* fis = (*it)->getFieldsStruct();
		cfm.emplace((*it)->getName(), fis);
		
		string s = "%methods.type."+(*it)->getName()+" = type{";
		string classStruct = "class."+(*it)->getName()+" type{%methods.type."+(*it)->getName()+"*";
		vector<string> vss(fs->size());
		for(stringmap::const_iterator jt = fs->cbegin(); jt != fs->cend(); ++jt)
		{
			vss[jt->second] = jt->first;
		}
		for(int i = 0; i < vss.size(); ++i)
		{
			if(i != 0)
			{
				s += ", ";
			}

			MethodNode* method = (*it)->getMethod(vss[i]);
			s += llvmType(method->getReturnType()) + " (";
			std::vector<FormalNode*> params = method->getParams();
			for(int j=0; params.size(); ++j)
			{
				if(j != 0)
				{
					s += ", ";
				}

				s += llvmType(params[j]->getType());
			}
			s += ")*";
		}

		s += "}";
		write(s);

		write("%methods."+(*it)->getName()+" = alloca %methods.type."+(*it)->getName());

		implementMethodsVector(*it);


		string clasStruct = "";
		vector<string> vfis(fis->size());
		for(stringmap::const_iterator jt = fis->cbegin(); jt != fis->cend(); ++jt)
		{
			vss[jt->second] = jt->first;
		}

		for(int i = 0; i < vfis.size(); ++i)
		{
			clasStruct += ", ";

			clasStruct += llvmType((*it)->getTypeInScope(vfis[i]));
		}
	
		clasStruct += "}";

		write(clasStruct);
	}
}

LlvmManager::LlvmManager(ProgramNode *pgrn, ostream* out, std::string moduleID)
{
	LlvmManager(pgrn, vector<ostream*>(1,out), moduleID);
}

std::string LlvmManager::write(std::string towrite, std::string ret)
{
	//stringstream ss();
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
		//ss = stringstream(it->second);
		var = "%"+it->first+"."+to_string(it->second);
		towrite = var+" = "+towrite;

	}	

	if(indent)
	{
		towrite = "\t" + towrite;
	}

	towrite += "\n";

	for(vector<ostream*>::iterator it=outputs.begin(); it!=outputs.end(); ++it)
	{
		(*it)->write(towrite.c_str(), towrite.size());
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
	//string llvmLabel = "";
	
	stringmap::iterator it = llvmLabels.find(label);
	if(it == llvmLabels.end())
	{
		it = (llvmLabels.emplace(label,0)).first;
	}
	else
	{
		++(it->second);
	}

	//ss = it->second;
	return it->first+to_string(it->second);

}

std::string LlvmManager::getFunction(std::string className, std::string fctName, std::string obj)
{
	//stringstream funcPos = cm[fctName];
	int fctNum = cm[className][0][fctName];
	return write("getelementptr class."+className+"* "+obj+", i32 0, i32 0, i32 0, i32 "+to_string(fctNum), "");
}

std::string LlvmManager::getField(std::string className, std::string fieldName, std::string obj)
{
	int fieldPos = cfm[fieldName][0][fieldName]+1; // +1 because the struct start with a pointer to the methods
	return write("getelementptr class."+className+"* "+obj+", i32 0, i32 "+to_string(fieldPos), "");
}

void LlvmManager::implementMethodsVector(ClassNode *cn)
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

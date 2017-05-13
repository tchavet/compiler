#include "ProgramNode.hpp"
#include "LlvmManager.hpp"

LlvmManager::LlvmManager(ProgramNode pgrn, outstream out, std::string moduleID)
{
	std::vector<ClassNode*> classes = getClasses();
	
	write("; ModuleID = "+"'"+moduleID +"'");
	
	for(const_iterator it = classes.cbegin(); iterator != classes.cend(); ++it)
	{
		stringmap* fs = (*it)->getFuncStruct();
		cm.insert((*it)->getName(), *fs);

		stringmap* fis = (*it)->getFieldStruct();
		cfm.insert((*it)->getName(), *fis);
		
		string s = "%methods.type."+(*it)->getName()+" = type{";
		string classStruct = "class."+(*it)->getName()+" type{%methods.type."+(*it)->getName()+"*";
		vector<string> vss(fs.size());
		for(const_iterator jt = fs->cbegin(); jt != fs->cend(); ++jt)
		{
			vss[(*jt)->second] = (*jt)->first;
		}
		for(int i = 0; i < vss.size(); ++i)
		{
			if(i != 0)
			{
				s += ", ";
			}

			MethodNode* method = getMethod(vss[i]);
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
			s += ")*"
		}

		s += "}";
		write(s);

		write("%methods."+(*it)->getName()+" = alloca %methods.type."+(*it)->getName());

		implementMethodsVector(*it);

		vector<string> vfis(fis.size();
		for(const_iterator jt = fis->cbegin(); jt != fis->cend(); ++jt)
		{
			vss[(*jt)->second] = (*jt)->first;
		}

		for(int i = 0; i < vfis.size(); ++i)
		{
			clasStruct += ", ";

			clasStruct += llvmType(getTypeInScope(vfis[i]));
		}
	
		clasStruct += "}"

		write(clasStruct);
	}
}

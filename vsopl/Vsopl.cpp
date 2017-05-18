#include "Vsopl.hpp"
#include "../semantic/Types.hpp"

std::vector<ClassNode*> Vsopl::classes = std::vector<ClassNode*>();

void Vsopl::init()
{
	createIoClass();
}

void Vsopl::semantic()
{
	for (int i=0; i<classes.size(); i++)
	{
		Types::add(classes[i]->getName(), classes[i]);
	}
}

void Vsopl::llvmHeader(LlvmManager* manager)
{
	for (int i=0; i<classes.size(); i++)
	{
		classes[i]->llvmHeader(manager);
		manager->write("");
	}
}

void Vsopl::llvmMain(LlvmManager* manager)
{
	for (int i=0; i<classes.size(); i++)
	{
		classes[i]->llvmMain(manager);
		manager->write("");
	}
}

void Vsopl::createIoClass()
{
	ClassNode* classNode = new ClassNode(0,0,"IO");

	std::vector<FormalNode*> params;

	params.push_back(new FormalNode(0,0,"str", "string"));
	MethodNode* method = new MethodNode(0,0,"print", params, "IO", new BlockNode(0,0));
	classNode->addMethod(method);

	params = std::vector<FormalNode*>();
	params.push_back(new FormalNode(0,0,"nb", "int32"));
	method = new MethodNode(0,0,"printInt32",params, "IO", new BlockNode(0,0));
	classNode->addMethod(method);

	params = std::vector<FormalNode*>();
	params.push_back(new FormalNode(0,0,"var", "bool"));
	method = new MethodNode(0,0,"printBool",params, "IO", new BlockNode(0,0));
	classNode->addMethod(method);

	classNode->addMethod(new MethodNode(0,0,"inputInt32",std::vector<FormalNode*>(), "int32", new BlockNode(0,0)));
	classNode->addMethod(new MethodNode(0,0,"inputBool",std::vector<FormalNode*>(), "bool", new BlockNode(0,0)));
	classNode->addMethod(new MethodNode(0,0,"inputLine",std::vector<FormalNode*>(), "string", new BlockNode(0,0)));

	classes.push_back(classNode);
}

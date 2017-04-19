#include "IOClass.hpp"
#include "../nodes/MethodNode.hpp"
#include "../nodes/FieldNode.hpp"
#include "../nodes/FormalNode.hpp"
#include "../nodes/BlockNode.hpp"

IOClass::IOClass()
{
	classNode = new ClassNode(0,0,"IO");

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
}

ClassNode* IOClass::getClassNode()
{
	return classNode;
}

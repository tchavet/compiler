#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "FieldNode.hpp"
#include "MethodNode.hpp"

class ClassNode : public AstNode
{
public:
	ClassNode(int line, int column, std::string name, std::string parent = "Object");
	std::string printTree(int tabsNb=0, bool types=false);
	std::string getTypeInScope(std::string id);
	void addField(FieldNode* field);
	void addFields(std::vector<FieldNode*>* fields);
	void addMethod(MethodNode* method);
	void addMethods(std::vector<MethodNode*>* methods);
	void setParentNode(ClassNode* parent);
	std::string getName();
	std::string getParentName();
	ClassNode* getParentNode();
	bool isA(std::string className);
	std::string commonAncestor(ClassNode* class2);
	MethodNode* getMethod(std::string methodName);
	std::vector<SemErr*> semCheck();
	bool redefinedField(FieldNode* field);
	bool redefinedMethod(MethodNode* method);

protected:
	std::string name;
	std::string parentName;
	ClassNode* parentNode;
	std::vector<FieldNode*> fields;
	std::vector<MethodNode*> methods;
};
#endif

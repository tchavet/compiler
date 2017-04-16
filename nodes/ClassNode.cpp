#include "ClassNode.hpp"

ClassNode::ClassNode(int line, int column, std::string name, std::string parent)
{
	AstNode(line, column);
	this->name = name;
	this->parent = parent;
	fields = std::vector<FieldNode*>();
	methods = std::vector<MethodNode*>();
}

void ClassNode::addField(FieldNode* field)
{
	fields.push_back(field);
}

void ClassNode::addFields(std::vector<FieldNode*>* fields)
{
	this->fields.insert(this->fields.end(), fields->begin(), fields->end());
}

void ClassNode::addMethod(MethodNode* method)
{
	methods.push_back(method);
}

void ClassNode::addMethods(std::vector<MethodNode*>* methods)
{
	this->methods.insert(this->methods.end(), methods->begin(), methods->end());
}

std::string ClassNode::printTree(int tabsNb)
{
	std::string print = "";
	
	print += "Class(" + name + ", " + parent + ",\n"
		+ tabs(tabsNb+1) + printList<FieldNode>(tabsNb+1, fields) + ",\n"
		+ tabs(tabsNb+1) + printList<MethodNode>(tabsNb+1, methods) + "\n"
		+ tabs(tabsNb) + ")";
	return print;
}

#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <string>

class AstNode
{
public:
	AstNode(int line=0, int column=0);
	virtual std::string printTree(int tabsNb=0, bool types=false) {};
	int getLine();
	int getColumn();
	void setParent(AstNode* parent);
	virtual std::string getTypeInScope(std::string id);

protected:
	int line;
	int column;
	AstNode* parent;

	std::string tabs(int tabsNb);

	template <class T>
	std::string printList(int tabsNb,bool types, std::vector<T*> nodes)
	{
		std::string print="[\n";
		for (int i=0; i<nodes.size(); i++)
		{
			print += tabs(tabsNb+1) + nodes[i]->printTree(tabsNb+1,types);
			if (i != nodes.size()-1)
				print += ",";
			print += "\n";
		}
		print += tabs(tabsNb) + "]";
		return print;
	}
};
#endif

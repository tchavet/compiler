#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <string>

class AstNode
{
public:
	AstNode(int line=0, int column=0);
	virtual std::string printTree(int tabsNb=0) {};
	int getLine();
	int getColumn();

protected:
	std::string tabs(int tabsNb);
	int line;
	int column;

	template <class T>
	std::string printList(int tabsNb, std::vector<T*> nodes)
	{
		std::string print="[\n";
		for (int i=0; i<nodes.size(); i++)
		{
			print += tabs(tabsNb+1) + nodes[i]->printTree(tabsNb+1);
			if (i != nodes.size()-1)
				print += ",";
			print += "\n";
		}
		print += tabs(tabsNb) + "]";
		return print;
	}
};
#endif

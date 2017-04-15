#include <vector>
#include "Token.hpp"
#ifndef ASTNODE_H
#define ASTNODE_H
class AstNode
{
public:
	AstNode(Token* tokenVal);
	void addNode(AstNode *child);
	void addNodes(std::vector<AstNode*>* nodes);
	std::vector<AstNode*> getChildren();
	std::string printTree(int tabs=0);

protected:
	Token* token;
	int childrenNb;
	std::vector<AstNode*> children;
};
#endif

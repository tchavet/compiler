#include "AstNode.hpp"

AstNode::AstNode(Token *tokenVal)
{
	token = tokenVal;
	childrenNb = 0;
}

void AstNode::addNode(AstNode *child)
{
	children.push_back(child);
	childrenNb++;
}

void AstNode::addNodes(std::vector<AstNode*> nodes)
{
	nodes.insert(nodes.end(), children.begin(), children.end());
	children = nodes;
}

std::vector<AstNode*> AstNode::getChildren()
{
	return children;
}

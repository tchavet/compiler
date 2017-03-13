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

std::vector<Token*> AstNode::children()
{
	return children;
}

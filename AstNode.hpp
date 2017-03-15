#include <vector>
#include "Token.hpp"

class AstNode
{
public:
	AstNode(Token* tokenVal);
	void addNode(AstNode *child);
	void addNodes(std::vector<AstNode*> nodes);
	std::vector<AstNode*> getChildren();

protected:
	Token* token;
	int childrenNb;
	std::vector<AstNode*> children;
};

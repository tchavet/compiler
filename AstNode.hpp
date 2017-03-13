#include <vector>

class AstNode
{
public:
	AstNode(Token* tokenVal);
	void addNode(AstNode *child);
	std::vector<Token*> children();

protected:
	Token* token;
	int childrenNb;
	std::vector<Token*> children;
};

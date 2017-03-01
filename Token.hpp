#include <string>

class Token
{
public:
	Token(int tokenLine, int tokenCol, std::string tokenType);
	void print();

protected:
	int line, col;
	std::string type;
};

#include <string>

class Token
{
public:
	Token(int tokenLine, int tokenCol, std::string tokenType, std::string tokenValue="");
	void print();

protected:
	int line, col;
	std::string type;
	std::string value;
};

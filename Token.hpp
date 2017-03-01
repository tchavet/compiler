#include <string>

class Token
{
public:
	enum Type {Int, String, Null};
	Token(int tokenLine, int tokenCol, std::string tokenType, std::string tokenValue="");
	Token(int tokenLine, int tokenCol, std::string tokenType, int tokenValue);
	void print();

protected:
	int line, col;
	std::string type;
	std::string value;
	int intValue;
	Type valueType;
};

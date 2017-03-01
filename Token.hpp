#include <string>

class Token
{
public:
	enum Type {Int, Keyword, Type, Object, String, Operator};
	Token(int tokenLine, int tokenCol, std::string tokenType, enum Type tokenValueType, std::string tokenValue="");
	Token(int tokenLine, int tokenCol, std::string tokenType, int tokenValue);
	void print();

protected:
	int line, col;
	std::string type;
	std::string value;
	int intValue;
	enum Type valueType;
};

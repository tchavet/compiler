#include <string>

class Token
{
public:
	enum Type {Int, Class, Type, Field, Method, Object, String};
	Token(int tokenLine, int tokenCol, enum Type valueType, std::string tokenValue="");
	Token(int tokenLine, int tokenCol, int tokenValue);
	void print();

protected:
	int line, col;
	std::string value;
	int intValue;
	enum Type type;
};

#include <string>
#ifndef TOKEN_H
#define TOKEN_H
class Token
{
public:
	enum Type {Class, Type, Field, Method, Object, String, Object_id, Type_id, Formal, Block, If, While, Let, Assign, UnOp, BinOp, Call, New, Arg, String_lit, Int_lit, Bool_lit};
	Token(int tokenLine, int tokenCol, enum Type valueType, std::string tokenValue="");
	Token(int tokenLine, int tokenCol, int tokenValue);
	void print();
	int getIntValue();
	std::string getStringValue();
	enum Type getType();

protected:
	int line, col;
	std::string value;
	int intValue;
	enum Type type;
};
#endif

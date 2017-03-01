#include "Token.hpp"
#include <iostream>

Token::Token(int tokenLine, int tokenCol, std::string tokenType, enum Type tokenValueType, std::string tokenValue)
{
	line = tokenLine;
	col = tokenCol;
	type = tokenType;
	value = tokenValue;
	valueType = tokenValueType;
}

Token::Token(int tokenLine, int tokenCol, std::string tokenType, int tokenValue)
{
	line = tokenLine;
	col = tokenCol;
	type = tokenType;
	intValue = tokenValue;
	valueType = Int;
}

void Token::print()
{
	std::cout<<line<<","<<col<<","<<type;
	if(valueType == Int)
		std::cout<<","<<intValue;
	else if(valueType == Type || valueType == Object || valueType == String)
		std::cout<<","<<value;
	std::cout<<std::endl;
}

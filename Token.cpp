#include "Token.hpp"
#include <iostream>

Token::Token(int tokenLine, int tokenCol, std::string tokenType, std::string tokenValue)
{
	line = tokenLine;
	col = tokenCol;
	type = tokenType;
	value = tokenValue;
	if(tokenValue == "")
		valueType = Null;
	else
		valueType = String;
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
	else if(valueType == String)
		std::cout<<","<<value;
	std::cout<<std::endl;
}

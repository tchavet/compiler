#include "Token.hpp"
#include <iostream>

Token::Token(int tokenLine, int tokenCol, enum Type tokenType, std::string tokenValue)
{
	line = tokenLine;
	col = tokenCol;
	value = tokenValue;
	type = tokenType;
}

Token::Token(int tokenLine, int tokenCol, int tokenValue)
{
	line = tokenLine;
	col = tokenCol;
	intValue = tokenValue;
	type = Int;
}

void Token::print()
{
/*	std::cout<<line<<","<<col<<","<<type;
	if(type == Int)
		std::cout<<","<<intValue;
	else if(type == Type || type == Object || type == String)
		std::cout<<","<<value;
	std::cout<<std::endl;*/
}

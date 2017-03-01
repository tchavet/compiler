#include "Token.hpp"
#include <iostream>

Token::Token(int tokenLine, int tokenCol, std::string tokenType, std::string tokenValue)
{
	line = tokenLine;
	col = tokenCol;
	type = tokenType;
	value = tokenValue;
}

Token::Token(int tokenLine, int tokenCol, std::string tokenType)
{
	Token(tokenLine, tokenCol, tokenType, "");
}

void Token::print()
{
	std::cout<<line<<","<<col<<","<<type;
	if(value != "")
		std::cout<<","<<value;
	std::cout<<std::endl;
}

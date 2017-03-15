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
	type = Int_lit;
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

int Token::getIntValue()
{
	return intValue;
}

std::string Token::getStringValue()
{
	return value;
}

enum Token::Type Token::getType()
{
	return type;
}


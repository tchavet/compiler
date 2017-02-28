#include "Token.h"
#include <iostream>

Token::Token(int tokenLine, int tokenCol, std::string tokenType)
{
	line = tokenLine;
	col = tokenCol;
	type = tokenType;
}

void Token::print()
{
	std::cout<<line<<","<<col<<","<<type<<std::endl;
}

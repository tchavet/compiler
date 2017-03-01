EOL			(\r|\n|\r\n)
whitespace 	[ \t]+
DIGIT		[0-9]
HEXDIGIT	[0-9]|[a-f]|[A-F]
BINDIGIT	[0-1]

	#include "Token.hpp"
    #include <string>
	#include <list>
	#include <iostream>
	#define YY_DECL extern "C" int yylex() // Use flex with c++
	using namespace std;
	int line = 1, col = 1;
	extern list<Token> tokens;
	void error(int line, int col);

%%

{EOL} 					{line++; col = 1;}
[+-]?{DIGIT}+			{tokens.push_back(Token(line,col,std::string("integer-literal"),yytext)); col += yyleng;}
[+-]?0x{HEXDIGIT}+		{tokens.push_back(Token(line,col,std::string("integer-literal"),yytext)); col += yyleng;}
[+-]?0b{BINDIGIT}+		{tokens.push_back(Token(line,col,std::string("integer-literal"),yytext)); col += yyleng;}
[+-]?0[xb][a-zA-Z0-9]+	{error(line,col); col+= yyleng;}
{whitespace}			{col += yyleng;}
.						{error(line,col); col++;}

%%


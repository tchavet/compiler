EOL		(\r|\n|\r\n)
whitespace 	[ \t]+
DIGIT	[0-9]

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

//MAXIM


//THIB


//MAXIME 


//TRUC

{EOL} 				{line++; col = 1;}
[+-]?{DIGIT}+		{tokens.push_back(Token(line,col,std::string("integer-literal"))); col += yyleng;}
{whitespace}		{col += yyleng;}
.					{error(line,col); col++;}

%%


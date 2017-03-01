EOL		(\r|\n|\r\n)
whitespace 	[ \t]+
DIGIT	[0-9]

	#include "Token.hpp"
    #include <string>
	#include <vector>
	#include <iostream>
	#define YY_DECL extern "C" int yylex() // Use flex with c++
	using namespace std;
	int line = 1, col = 1;
	vector<Token> tokens;

%%
{EOL} 				{line++; col = 1;}
[+-]?{DIGIT}+		{tokens.push_back(Token(line,col,std::string("integer-literal"))); col += yyleng;}
{whitespace}		{col += yyleng;}
.					{cout << "error" << endl; col++;}

%%


%x NEST_COM LINE_COM

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

	int commentDepth = 0;
%%

"//"				{BEGIN(LINE_COM);}
<LINE_COM>{EOL}		{++line; BEGIN(INITIAL);}
<LINE_COM>.			{}


"(*"				{col+= yyleng; BEGIN(NEST_COM);}
<NEST_COM>"(*"		{col+= yyleng; ++commentDepth;}
<NEST_COM>"*)"		{col+= yyleng; 
					 if (commentDepth) --commentDepth; 
				   	 else BEGIN(INITIAL);}
<NEST_COM>{EOL}		{++line; col = 1;}
<NEST_COM><<EOF>>	{error(line, col);}
<NEST_COM>.      	{col+= yyleng;}


{EOL} 					{line++; col = 1;}

[+-]?{DIGIT}+			{tokens.push_back(Token(line,col,"integer-literal",yytext)); col += yyleng;}
[+-]?0x{HEXDIGIT}+		{tokens.push_back(Token(line,col,"integer-literal",yytext)); col += yyleng;}
[+-]?0b{BINDIGIT}+		{tokens.push_back(Token(line,col,"integer-literal",yytext)); col += yyleng;}
[+-]?0[xb][a-zA-Z0-9]+	{error(line,col); col+= yyleng;}

and 	{tokens.push_back(Token(line,col,"and")); col += yyleng;}
bool 	{tokens.push_back(Token(line,col,"bool")); col += yyleng;}
class 	{tokens.push_back(Token(line,col,"class")); col += yyleng;}
do 		{tokens.push_back(Token(line,col,"do")); col += yyleng;}
else 	{tokens.push_back(Token(line,col,"else")); col += yyleng;}
extends {tokens.push_back(Token(line,col,"extends")); col += yyleng;}
false 	{tokens.push_back(Token(line,col,"false")); col += yyleng;}
if  	{tokens.push_back(Token(line,col,"if")); col += yyleng;}
in  	{tokens.push_back(Token(line,col,"in")); col += yyleng;}
int32 	{tokens.push_back(Token(line,col,"int32")); col += yyleng;}
isnull 	{tokens.push_back(Token(line,col,"isnull")); col += yyleng;}
let 	{tokens.push_back(Token(line,col,"let")); col += yyleng;}
new 	{tokens.push_back(Token(line,col,"new")); col += yyleng;}
not 	{tokens.push_back(Token(line,col,"not")); col += yyleng;}
string 	{tokens.push_back(Token(line,col,"string")); col += yyleng;}
then 	{tokens.push_back(Token(line,col,"then")); col += yyleng;}
true 	{tokens.push_back(Token(line,col,"true")); col += yyleng;}
unit 	{tokens.push_back(Token(line,col,"unit")); col += yyleng;}
while 	{tokens.push_back(Token(line,col,"while")); col += yyleng;}

[A-Z][a-zA-Z0-9_]+	{tokens.push_back(Token(line,col,"type-identifier",yytext)); col += yyleng;}
[a-z][a-zA-Z0-9_]+	{tokens.push_back(Token(line,col,"object-identifier",yytext)); col += yyleng;}

{whitespace}			{col += yyleng;}
.						{error(line,col); col++;}

%%


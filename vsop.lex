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
<LINE_COM>{EOL}		{BEGIN(INITIAL);}
<LINE_COM>.			{}


"(*"				{col+= yyleng; BEGIN(NEST_COM);}
<NEST_COM>"(*"		{col+= yyleng; ++commentDepth;}
<NEST_COM>"*)"		{col+= yyleng; 
					 if (commentDepth) --commentDepth; 
				   	 else BEGIN(INITIAL);}
<NEST_COM>{EOL}		{++line; col = 1;}
<NEST_COM>{EOF}		{error(line, col);}
<NEST_COM>.      	{col+= yyleng;}


{EOL} 					{line++; col = 1;}

[+-]?{DIGIT}+			{tokens.push_back(Token(line,col,std::string("integer-literal"),yytext)); col += yyleng;}
[+-]?0x{HEXDIGIT}+		{tokens.push_back(Token(line,col,std::string("integer-literal"),yytext)); col += yyleng;}
[+-]?0b{BINDIGIT}+		{tokens.push_back(Token(line,col,std::string("integer-literal"),yytext)); col += yyleng;}
[+-]?0[xb][a-zA-Z0-9]+	{error(line,col); col+= yyleng;}

and 	{tokens.push_back(Token(line,col,std::string("and"))); col += yyleng;}
bool 	{tokens.push_back(Token(line,col,std::string("bool"))); col += yyleng;}
class 	{tokens.push_back(Token(line,col,std::string("class"))); col += yyleng;}
do 		{tokens.push_back(Token(line,col,std::string("do"))); col += yyleng;}
else 	{tokens.push_back(Token(line,col,std::string("else"))); col += yyleng;}
extends {tokens.push_back(Token(line,col,std::string("extends"))); col += yyleng;}
false 	{tokens.push_back(Token(line,col,std::string("false"))); col += yyleng;}
if  	{tokens.push_back(Token(line,col,std::string("if"))); col += yyleng;}
in  	{tokens.push_back(Token(line,col,std::string("in"))); col += yyleng;}
int32 	{tokens.push_back(Token(line,col,std::string("int32"))); col += yyleng;}
isnull 	{tokens.push_back(Token(line,col,std::string("isnull"))); col += yyleng;}
let 	{tokens.push_back(Token(line,col,std::string("let"))); col += yyleng;}
new 	{tokens.push_back(Token(line,col,std::string("new"))); col += yyleng;}
not 	{tokens.push_back(Token(line,col,std::string("not"))); col += yyleng;}
string 	{tokens.push_back(Token(line,col,std::string("string"))); col += yyleng;}
then 	{tokens.push_back(Token(line,col,std::string("then"))); col += yyleng;}
true 	{tokens.push_back(Token(line,col,std::string("true"))); col += yyleng;}
unit 	{tokens.push_back(Token(line,col,std::string("unit"))); col += yyleng;}
while 	{tokens.push_back(Token(line,col,std::string("while"))); col += yyleng;}

{whitespace}			{col += yyleng;}
.						{error(line,col); col++;}

%%


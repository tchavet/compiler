%x NEST_COM LINE_COM STR_LIT

EOL			(\r|\n|\r\n)
whitespace 	[ \t]+
DIGIT		[0-9]
HEXDIGIT	[0-9]|[a-f]|[A-F]
BINDIGIT	[0-1]
XHH			"\x" + HEXDIGIT + HEXDIGIT
STR_BSNL	"\\" + EOL + (whitespace)*

	#include "Token.hpp"
    #include <string>
	#include <list>
	#include <iostream>
	#include <cstdlib>
	#include <sstream>
	#define YY_DECL extern "C" int yylex() // Use flex with c++
	using namespace std;
	int line = 1, col = 1;
	extern list<Token> tokens;
	void error(int line, int col);
	std::string char2printable(char* x);

	int comment_depth = 0, opening_col, opening_line;
	string str = ""; 
%%

"\""				{opening_col = col; opening_line = line; col += yyleng; BEGIN(STR_LIT);}
<STR_LIT>"\\\""		{col += yyleng; str += "\\\"";}
<STR_LIT>"\""		{col += yyleng; BEGIN(INITIAL);}
<STR_LIT>XHH		{col += yyleng; str +=  yytext;}
<STR_LIT>"\\b"		{col += yyleng; str +=  "\x08";}
<STR_LIT>"\\t"		{col += yyleng; str +=  "\x09";}
<STR_LIT>"\\n"		{col += yyleng; str +=  "\x0a";}
<STR_LIT>"\\r"		{col += yyleng; str +=  "\x0d";}
<STR_LIT>"\\\\"		{col += yyleng; str += "\\\\";}
<STR_LIT>STR_BSNL	{col = yyleng-1; ++line;}
<STR_LIT>"\\"		{error(line, col);}
<STR_LIT><<EOF>>	{error(opening_line, opening_col);}
<STR_LIT>.			{col += yyleng; str +=  char2printable(yytext);}

"//"				{BEGIN(LINE_COM);}
<LINE_COM>{EOL}		{++line; col = 1; BEGIN(INITIAL);}
<LINE_COM>.			{}


"(*"				{opening_col = col; opening_line = line; col+= yyleng; BEGIN(NEST_COM);}
<NEST_COM>"(*"		{col+= yyleng; ++comment_depth;}
<NEST_COM>"*)"		{col+= yyleng; 
					 if (comment_depth) --comment_depth; 
				   	 else BEGIN(INITIAL);}
<NEST_COM>{EOL}		{++line; col = 1;}
<NEST_COM><<EOF>>	{error(opening_line, opening_col);}
<NEST_COM>.      	{col+= yyleng;}


{EOL} 					{line++; col = 1;}

[+-]?{DIGIT}+			{tokens.push_back(Token(line,col,"integer-literal",atoi(yytext))); col += yyleng;}
[+-]?0x{HEXDIGIT}+		{tokens.push_back(Token(line,col,"integer-literal",strtol(yytext, NULL, 16))); col += yyleng;}
[+-]?0b{BINDIGIT}+		{tokens.push_back(Token(line,col,"integer-literal",strtol(yytext+2, NULL, 2))); col += yyleng;}
[+-]?0[xb][a-zA-Z0-9]+	{error(line,col); col+= yyleng;}

and 	{tokens.push_back(Token(line,col,"and",Token::Keyword)); col += yyleng;}
bool 	{tokens.push_back(Token(line,col,"bool",Token::Keyword)); col += yyleng;}
class 	{tokens.push_back(Token(line,col,"class",Token::Keyword)); col += yyleng;}
do 		{tokens.push_back(Token(line,col,"do",Token::Keyword)); col += yyleng;}
else 	{tokens.push_back(Token(line,col,"else",Token::Keyword)); col += yyleng;}
extends {tokens.push_back(Token(line,col,"extends",Token::Keyword)); col += yyleng;}
false 	{tokens.push_back(Token(line,col,"false",Token::Keyword)); col += yyleng;}
if  	{tokens.push_back(Token(line,col,"if",Token::Keyword)); col += yyleng;}
in  	{tokens.push_back(Token(line,col,"in",Token::Keyword)); col += yyleng;}
int32 	{tokens.push_back(Token(line,col,"int32",Token::Keyword)); col += yyleng;}
isnull 	{tokens.push_back(Token(line,col,"isnull",Token::Keyword)); col += yyleng;}
let 	{tokens.push_back(Token(line,col,"let",Token::Keyword)); col += yyleng;}
new 	{tokens.push_back(Token(line,col,"new",Token::Keyword)); col += yyleng;}
not 	{tokens.push_back(Token(line,col,"not",Token::Keyword)); col += yyleng;}
string 	{tokens.push_back(Token(line,col,"string",Token::Keyword)); col += yyleng;}
then 	{tokens.push_back(Token(line,col,"then",Token::Keyword)); col += yyleng;}
true 	{tokens.push_back(Token(line,col,"true",Token::Keyword)); col += yyleng;}
unit 	{tokens.push_back(Token(line,col,"unit",Token::Keyword)); col += yyleng;}
while 	{tokens.push_back(Token(line,col,"while",Token::Keyword)); col += yyleng;}

[A-Z][a-zA-Z0-9_]*	{tokens.push_back(Token(line,col,"type-identifier",Token::Type,yytext)); col += yyleng;}
[a-z][a-zA-Z0-9_]*	{tokens.push_back(Token(line,col,"object-identifier",Token::Object,yytext)); col += yyleng;}

\{	{tokens.push_back(Token(line,col,"lbrace",Token::Operator)); col += yyleng;}
\}	{tokens.push_back(Token(line,col,"rbrace",Token::Operator)); col += yyleng;}
\(	{tokens.push_back(Token(line,col,"lpar",Token::Operator)); col += yyleng;}
\)	{tokens.push_back(Token(line,col,"rpar",Token::Operator)); col += yyleng;}
:	{tokens.push_back(Token(line,col,"colon",Token::Operator)); col += yyleng;}
;	{tokens.push_back(Token(line,col,"semicolon",Token::Operator)); col += yyleng;}
,	{tokens.push_back(Token(line,col,"comma",Token::Operator)); col += yyleng;}
\+	{tokens.push_back(Token(line,col,"plus",Token::Operator)); col += yyleng;}
\-	{tokens.push_back(Token(line,col,"minus",Token::Operator)); col += yyleng;}
\*	{tokens.push_back(Token(line,col,"times",Token::Operator)); col += yyleng;}
\/	{tokens.push_back(Token(line,col,"div",Token::Operator)); col += yyleng;}
\^	{tokens.push_back(Token(line,col,"pow",Token::Operator)); col += yyleng;}
\.	{tokens.push_back(Token(line,col,"dot",Token::Operator)); col += yyleng;}
=	{tokens.push_back(Token(line,col,"equal",Token::Operator)); col += yyleng;}
\<	{tokens.push_back(Token(line,col,"lower",Token::Operator)); col += yyleng;}
\<=	{tokens.push_back(Token(line,col,"lower-equal",Token::Operator)); col += yyleng;}
\<\-	{tokens.push_back(Token(line,col,"assign",Token::Operator)); col += yyleng;}

{whitespace}			{col += yyleng;}
.						{error(line,col); col++;}

%%
std::string char2printable(char* x){
	char c = x[0];
	stringstream ss;
	if(c < 32){
		ss <<"\\x" << std::hex << (int) c;
		return ss.str();
	}
	else if( c > 126){
		ss << "\\x" << std::hex << (int) c;
        return ss.str();
	}
	ss << c;
	return ss.str();
}

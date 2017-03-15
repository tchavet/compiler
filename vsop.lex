%x NEST_COM LINE_COM STR_LIT

EOL			(\r|\n|\r\n)
whitespace 	[ \t]+
DIGIT		[0-9]
HEXDIGIT	[0-9]|[a-f]|[A-F]
BINDIGIT	[0-1]
XHH			"\\x"{HEXDIGIT}{HEXDIGIT}
STR_BSNL	"\\"{EOL}{whitespace}*

	#include <string>
	#include <list>
	#include <iostream>
	#include <cstdlib>
	#include <sstream>
	#include <utility>
	#include <vector>
	#include "AstNode.hpp"
	#include "vsop.tab.h"

	#define YY_DECL extern "C" int yylex() // Use flex with c++

	#define YY_USER_ACTION update_index();

	using namespace std;

	void lexical_error(int line, int col, string message="");
	
	std::string char2printable(char* x);
	void update_index();
	void print(std::string toPrint);
	
	int comment_depth = 0, opening_col, opening_line;
	string str = ""; 
	list<pair<int,int> > com_open(0);
	extern bool lex;

%%

"\""				{opening_col = yylloc.first_column; opening_line = yylloc.first_line; str = "\""; BEGIN(STR_LIT);}
<STR_LIT>"\\\""		{str += "\\\"";}
<STR_LIT>"\""		{str += "\""; yylval.strval = new string(str); print("string-literal"); BEGIN(INITIAL);return STRING_LIT;}
<STR_LIT>{XHH}		{str +=  yytext;}
<STR_LIT>"\\b"		{str +=  "\\x08";}
<STR_LIT>"\\t"		{str +=  "\\x09";}
<STR_LIT>"\\n"		{str +=  "\\x0a";}
<STR_LIT>"\\r"		{str +=  "\\x0d";}
<STR_LIT>"\\\\"		{str += "\\\\";}
<STR_LIT>{EOL}		{lexical_error(yylloc.first_line,yylloc.first_column,"character '\\n' is illegal in this context."); yylloc.last_line++;yylloc.last_column=1;}
<STR_LIT>{STR_BSNL}	{yylloc.last_line++;}
<STR_LIT>"\\"		{lexical_error(yylloc.first_line, yylloc.last_column);}
<STR_LIT><<EOF>>	{lexical_error(opening_line, opening_col, "string was opened but never closed"); BEGIN(INITIAL);}
<STR_LIT>.			{str +=  char2printable(yytext);}

"//"				{BEGIN(LINE_COM);}
<LINE_COM>{EOL}		{yylloc.last_line++;yylloc.last_column=1; BEGIN(INITIAL);}
<LINE_COM>.			{}


"(*"				{com_open.clear();com_open.push_back(pair<int,int>(yylloc.first_line, yylloc.first_column)); BEGIN(NEST_COM);}
<NEST_COM>"(*"		{++comment_depth; com_open.push_back(pair<int,int>(yylloc.first_line, yylloc.first_column));}
<NEST_COM>"*)"		{com_open.pop_back();
					 if (comment_depth) --comment_depth; 
				   	 else BEGIN(INITIAL);}
<NEST_COM>{EOL}		{yylloc.last_line++;yylloc.last_column=1;}
<NEST_COM><<EOF>>	{lexical_error(com_open.back().first, com_open.back().second, "comments opened but never closed"); BEGIN(INITIAL);}
<NEST_COM>.      	{}


{EOL} 				{yylloc.last_line++;yylloc.last_column=1;}

{DIGIT}+			{yylval.intval = atoi(yytext); print("integer-literal");return INT_LIT;}
0x{HEXDIGIT}+		{yylval.intval = strtol(yytext, NULL, 16); print("integer-literal");return INT_LIT;}
0b{BINDIGIT}+		{yylval.intval = strtol(yytext+2, NULL, 2); print("integer-litteral");return INT_LIT;}
0[xb][a-zA-Z0-9]*	{lexical_error(yylloc.first_line, yylloc.first_column, yytext + string(" is not a valid integer literal."));}

and 	{print("and");return AND;}
bool 	{print("bool");return BOOL;}
class 	{print("class");return CLASS;}
do 		{print("do");return DO;}
else 	{print("else");return ELSE;}
extends {print("extends");return EXTENDS;}
false 	{print("false");return FALSE;}
if  	{print("if");return IF;}
in  	{print("in");return IN;}
int32 	{print("int32");return INT32;}
isnull 	{print("isnull");return ISNULL;}
let 	{print("let");return LET;}
new 	{print("new");return NEW;}
not 	{print("not");return NOT;}
string 	{print("string");return STRING;}
then 	{print("then");return THEN;}
true 	{print("true");return TRUE;}
unit 	{print("unit");return UNIT;}
while 	{print("while");return WHILE;}

[A-Z][a-zA-Z0-9_]*	{yylval.strval = new string(yytext);print("type-identifier"); return TYPE_ID;}
[a-z][a-zA-Z0-9_]*	{yylval.strval = new string(yytext); print("object-identifier");return OBJECT_ID;}

\{	{print("lbrace");return LBRACE;}
\}	{print("rbrace");return RBRACE;}
\(	{print("lpar");return LPAR;}
\)	{print("rpar");return RPAR;}
:	{print("colon");return COLON;}
;	{print("semicolon");return SEMICOLON;}
,	{print("comma");return COMMA;}
\+	{print("plus");return PLUS;}
\-	{print("minus");return MINUS;}
\*	{print("times");return TIMES;}
\/	{print("div");return DIV;}
\^	{print("pow");return POW;}
\.	{print("dot");return DOT;}
=	{print("equal");return EQUAL;}
\<	{print("lower");return LOWER;}
\<=	{print("lower-equal");return LOWER_EQ;}
\<\-	{print("assign");return ASSIGN;}

{whitespace}			{}
.						{lexical_error(yylloc.first_line, yylloc.first_column,"illegal character: "+string(yytext));}

%%
std::string char2printable(char* x){
	char c = x[0];
	stringstream ss;
	if(c == 0)
	{
		lexical_error(yylloc.first_line, yylloc.first_column,"character '\\000' is illegal in this context.");
		return "";
	}
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
void update_index(){
	yylloc.first_line = yylloc.last_line;
	yylloc.last_line = yylloc.last_line;
	yylloc.first_column = yylloc.last_column;
	yylloc.last_column = yylloc.last_column + yyleng;
	
}
void print(std::string toPrint)
{
	if(lex)
		std::cout << yylloc.first_line<< "," << yylloc.first_column << ","<< toPrint << std::endl;
}


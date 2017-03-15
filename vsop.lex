%x NEST_COM LINE_COM STR_LIT

EOL			(\r|\n|\r\n)
whitespace 	[ \t]+
DIGIT		[0-9]
HEXDIGIT	[0-9]|[a-f]|[A-F]
BINDIGIT	[0-1]
XHH			"\\x"{HEXDIGIT}{HEXDIGIT}
STR_BSNL	"\\"{EOL}{whitespace}*

	#include "vsop.tab.h"
    #include <string>
	#include <list>
	#include <iostream>
	#include <cstdlib>
	#include <sstream>
	#include <utility>
	#define YY_DECL extern "C" int yylex() // Use flex with c++

	#define YY_USER_ACTION update_index();

	using namespace std;

	void lexical_error(int line, int col, string message="");
	
	std::string char2printable(char* x);

	int comment_depth = 0, opening_col, opening_line;
	string str = ""; 
	list<pair<int,int> > com_open(0);
%%

"\""				{opening_col = yylloc.first_column; opening_line = yylloc.first_line; str = "\""; BEGIN(STR_LIT);}
<STR_LIT>"\\\""		{str += "\\\"";}
<STR_LIT>"\""		{str += "\""; yylval.strval = str; return STRING_LIT; BEGIN(INITIAL);}
<STR_LIT>{XHH}		{str +=  yytext;}
<STR_LIT>"\\b"		{str +=  "\\x08";}
<STR_LIT>"\\t"		{str +=  "\\x09";}
<STR_LIT>"\\n"		{str +=  "\\x0a";}
<STR_LIT>"\\r"		{str +=  "\\x0d";}
<STR_LIT>"\\\\"		{str += "\\\\";}
<STR_LIT>{EOL}		{lexical_error(yylloc.first_line,yyloc.first_column,"character '\\n' is illegal in this context."); yylloc.last_line++;}
<STR_LIT>{STR_BSNL}	{yyloc.last_line++;}
<STR_LIT>"\\"		{lexical_error(yylloc.first_line, yylloc.last_column);}
<STR_LIT><<EOF>>	{lexical_error(opening_line, opening_col, "string was opened but never closed"); BEGIN(INITIAL);}
<STR_LIT>.			{str +=  char2printable(yytext);}

"//"				{BEGIN(LINE_COM);}
<LINE_COM>{EOL}		{yylloc.last_line++; BEGIN(INITIAL);}
<LINE_COM>.			{}


"(*"				{com_open.clear();com_open.push_back(pair<int,int>(yylloc.first_line, yylloc.first_column)); BEGIN(NEST_COM);}
<NEST_COM>"(*"		{++comment_depth; com_open.push_back(pair<int,int>(yylloc.first_line, yylloc.first_column));}
<NEST_COM>"*)"		{com_open.pop_back();
					 if (comment_depth) --comment_depth; 
				   	 else BEGIN(INITIAL);}
<NEST_COM>{EOL}		{yylloc.last_line++;}
<NEST_COM><<EOF>>	{lexical_error(com_open.back().first, com_open.back().second, "comments opened but never closed"); BEGIN(INITIAL);}
<NEST_COM>.      	{}


{EOL} 				{}

{DIGIT}+			{yylval.intval = atoi(yytext); return INT_LIT;}
0x{HEXDIGIT}+		{yylval.intval = strtol(yytext, NULL, 16); return INT_LIT;}
0b{BINDIGIT}+		{yylval.intval = strtol(yytext+2, NULL, 2); return INT_LIT;}
0[xb][a-zA-Z0-9]*	{lexical_error(yylloc.first_line, yylloc.fist_column, yytext + string(" is not a valid integer literal."));}

and 	{return AND;}
bool 	{return BOOL;}
class 	{return CLASS;}
do 		{return DO;}
else 	{return ELSE;}
extends {return EXTENDS;}
false 	{return FALSE;}
if  	{return IF;}
in  	{return IN;}
int32 	{return INT32;}
isnull 	{return ISNULL;}
let 	{return LET;}
new 	{return NEW;}
not 	{return NOT;}
string 	{return STRING;}
then 	{return THEN;}
true 	{return TRUE;}
unit 	{return UNIT;}
while 	{return WHILE;}

[A-Z][a-zA-Z0-9_]*	{yylval.strval = string(yytext); return TYPE_ID;}
[a-z][a-zA-Z0-9_]*	{yylval.strval = string(yytext); return OBJ_ID;}

\{	{return LBRACE;}
\}	{return RBRACE;}
\(	{return LPAR;}
\)	{return RPAR;}
:	{return COLON;}
;	{return SEMICOLON;}
,	{return COMMA;}
\+	{return PLUS;}
\-	{return MINUS;}
\*	{return TIMES;}
\/	{return DIV;}
\^	{return POW;}
\.	{return DOT;}
=	{return EQUAL;}
\<	{return LOWER;}
\<=	{return LOWER_EQ;}
\<\-	{return ASSIGN;}

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
	yylloc.first_line = yylineno;
	yylloc.last_line = yylineno;
	yylloc.first_column = yylloc.last_column;
	yylloc.last_column = yyloc.last_column + yyleng;
	
}

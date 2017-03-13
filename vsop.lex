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
	#define YY_DECL extern "C" int yylex() // Use flex with c++

	using namespace std;

	void error(int line, int col, string message="");
	std::string char2printable(char* x);

	int comment_depth = 0, opening_col, opening_line;
	string str = ""; 
	list<pair<int,int> > com_open(0);
%%

"\""				{opening_col = col; opening_line = line; col += yyleng; str = "\""; BEGIN(STR_LIT);}
<STR_LIT>"\\\""		{col += yyleng; str += "\\\"";}
<STR_LIT>"\""		{col += yyleng; str += "\""; yylval.strval = str; return STRING_LIT; BEGIN(INITIAL);}
<STR_LIT>{XHH}		{col += yyleng; str +=  yytext;}
<STR_LIT>"\\b"		{col += yyleng; str +=  "\\x08";}
<STR_LIT>"\\t"		{col += yyleng; str +=  "\\x09";}
<STR_LIT>"\\n"		{col += yyleng; str +=  "\\x0a";}
<STR_LIT>"\\r"		{col += yyleng; str +=  "\\x0d";}
<STR_LIT>"\\\\"		{col += yyleng; str += "\\\\";}
<STR_LIT>{EOL}		{error(line,col,"character '\\n' is illegal in this context.");col = 1; line++;}
<STR_LIT>{STR_BSNL}	{col = yyleng-1; ++line;}
<STR_LIT>"\\"		{error(line, col);}
<STR_LIT><<EOF>>	{error(opening_line, opening_col, "string was opened but never closed"); BEGIN(INITIAL);}
<STR_LIT>.			{str +=  char2printable(yytext);col += yyleng;}

"//"				{BEGIN(LINE_COM);}
<LINE_COM>{EOL}		{++line; col = 1; BEGIN(INITIAL);}
<LINE_COM>.			{}


"(*"				{com_open.clear();com_open.push_back(pair<int,int>(line,col)); BEGIN(NEST_COM);col+= yyleng;}
<NEST_COM>"(*"		{++comment_depth; com_open.push_back(pair<int,int>(line,col));col+= yyleng;}
<NEST_COM>"*)"		{com_open.pop_back();col+= yyleng;
					 if (comment_depth) --comment_depth; 
				   	 else BEGIN(INITIAL);}
<NEST_COM>{EOL}		{++line; col = 1;}
<NEST_COM><<EOF>>	{error(com_open.back().first, com_open.back().second, "comments opened but never closed"); BEGIN(INITIAL);}
<NEST_COM>.      	{col+= yyleng;}


{EOL} 				{}

{DIGIT}+			{yylval.intval = atoi(yytext); return INT_LIT;}
0x{HEXDIGIT}+		{yylval.intval = strtol(yytext, NULL, 16); return INT_LIT;}
0b{BINDIGIT}+		{yylval.intval = strtol(yytext+2, NULL, 2); return INT_LIT;}
0[xb][a-zA-Z0-9]*	{error(line,col,yytext + string(" is not a valid integer literal."));}

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
.						{error(line,col,"illegal character: "+string(yytext)); col++;}

%%
std::string char2printable(char* x){
	char c = x[0];
	stringstream ss;
	if(c == 0)
	{
		error(line,col,"character '\\000' is illegal in this context.");
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

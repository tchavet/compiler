%x SC_COMMENT

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

	int comment_nesting = 0;
%%

"(*"             { BEGIN(SC_COMMENT);}
<SC_COMMENT>"(*"           { ++comment_nesting; }
<SC_COMMENT>"*)"        { if (comment_nesting) --comment_nesting; 
						  else BEGIN(INITIAL); }
<SC_COMMENT>.           {}


{EOL} 				{line++; col = 1;}
[+-]?{DIGIT}+		{tokens.push_back(Token(line,col,std::string("integer-literal"))); col += yyleng;}
{whitespace}		{col += yyleng;}

%%


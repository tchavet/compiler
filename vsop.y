%{
#include <iostream>
#include <string>
#include "AstNode.hpp"
#include "vsop.tab.h"

extern "C" int yylex();

extern AstNode *root;

struct YYLTYPE;
static void yyerror(const char*);
void syntax_error(int line, int col, std::string message="");

%}


%union
{
	int intval;
	std::string* strval;
	AstNode* node;
	std::vector<AstNode*>* nodeList;
}

%token <intval> INT_LIT
%token AND
%token BOOL
%token CLASS
%token DO
%token ELSE
%token EXTENDS
%token FALSE
%token IF
%token IN
%token INT32
%token ISNULL
%token LET
%token NEW
%token NOT
%token STRING
%token THEN
%token TRUE
%token UNIT
%token WHILE
%token <strval> TYPE_ID
%token <strval> OBJECT_ID
%token <strval> STRING_LIT
%token LBRACE
%token RBRACE
%token LPAR
%token RPAR
%token COLON
%token SEMICOLON
%token COMMA
%token PLUS
%token MINUS
%token TIMES
%token DIV
%token POW
%token DOT
%token EQUAL
%token LOWER
%token LOWER_EQ
%token ASSIGN

/*Precedence options*/
%right ASSIGN
%left AND
%right NOT
%nonassoc LOWER LOWER_EQ EQUAL 
%left PLUS MINUS
%left TIMES DIV
%right ISNULL 
%right POW
%left DOT

%type <node> class field method type formal block expr args arg literal boolean_literal formals formalopt argopt
%type <nodeList> classopt field_method class_body expropt

%start program

%%

program:
	   class classopt
	   {
		 root->addNode($1);
		 root->addNodes($2);
	   }

classopt:
		classopt class
		{
			$1->push_back($2);
			$$ = $1;
		}
|		%empty
		{
			std::vector<AstNode*> *classes = new std::vector<AstNode*>();
			$$ = classes;
		}

class:
	 CLASS TYPE_ID class_body
	 {
		Token *token = new Token(@1.first_line, @1.first_column, Token::Class);
		AstNode *node = new AstNode(token);
		token = new Token(@2.first_line, @2.first_column, Token::Type, *$2);
		node->addNode(new AstNode(token)); // name
		token = new Token(-1,-1,Token::Type,"Object");
		node->addNode(new AstNode(token)); // parent
		node->addNode((*$3)[0]); // fields
		node->addNode((*$3)[1]); // methods
		$$ = node;
	 }
|	 CLASS TYPE_ID EXTENDS TYPE_ID class_body
	 {
		Token *token = new Token(@1.first_line, @1.first_column, Token::Class);
		AstNode *node = new AstNode(token);
		token = new Token(@2.first_line, @2.first_column, Token::Type, *$2);
		node->addNode(new AstNode(token)); // name
		token = new Token(@4.first_line, @4.first_column, Token::Type, *$4);
		node->addNode(new AstNode(token)); // parent
		node->addNode((*$5)[0]); // fields
		node->addNode((*$5)[1]); // methods
		$$ = node;
	 }

class_body:
		  LBRACE field_method RBRACE
		  {
			$$ = $2;
		  }

field_method:
			%empty
			{
			  std::vector<AstNode*> *fields_methods = new std::vector<AstNode*>(2);
			  (*fields_methods)[0] = new AstNode(NULL);
			  (*fields_methods)[1] = new AstNode(NULL);
			  $$ = fields_methods;
			}
|			field_method field
			{
			  (*$1)[0]->addNode($2);
			  $$ = $1;
			}
|			field_method method
			{
			  (*$1)[1]->addNode($2);
			  $$ = $1;
			}

field:
	OBJECT_ID COLON type SEMICOLON
	{
		Token *token = new Token(@1.first_line, @1.first_column, Token::Field);
		AstNode *node = new AstNode(token);
		token = new Token(@1.first_line, @1.first_column, Token::Object_id, *$1);
		node->addNode(new AstNode(token));
		node->addNode($3);
		$$ = node;
	}

|	OBJECT_ID COLON type ASSIGN expr SEMICOLON
	{
		Token *token = new Token(@1.first_line, @1.first_column, Token::Field);
		AstNode *node = new AstNode(token);
		token = new Token(@1.first_line, @1.first_column, Token::Object_id, *$1);
		node->addNode(new AstNode(token));
		node->addNode($3);
		node->addNode($5);
		$$ = node;
	}

method:
	 OBJECT_ID LPAR formals RPAR COLON type block
	 {
		Token *token = new Token(@1.first_line, @1.first_column, Token::Method);
		AstNode *node = new AstNode(token);
		token = new Token(@1.first_line, @1.first_column, Token::Object_id, *$1);
		node->addNode(new AstNode(token));
		node->addNode($3);
		node->addNode($6);
		node->addNode($7);
		$$ = node;
	 }

type:
	TYPE_ID
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, Token::Type_id, *$1));
	}
|	INT32
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, Token::Type_id, "int32"));
	}
|	BOOL
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, Token::Type_id, "bool"));
	}
|	STRING
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, Token::Type_id, "string"));
	}
|	UNIT
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, Token::Type_id, "unit"));
	}


formals:
	formalopt formal
	{
		$1->addNode($2);
		$$ = $1;
	}
|	%empty
	{
		$$ = new AstNode(NULL);
	}

formalopt:
	formalopt formal COMMA
	{
		$1->addNode($2);
		$$ = $1;
	}
|	%empty
	{
		$$ = new AstNode(NULL);
	}

formal:
	OBJECT_ID COLON type
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::Formal));
		node->addNode(new AstNode(new Token(@1.first_line, @1.first_column, Token::Object_id, *$1)));
		node->addNode($3);
		$$ = node;
	}

block:
	LBRACE expropt expr RBRACE
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::Block));
		node->addNodes($2);
		node->addNode($3);
		$$ = node;
	}

expropt:
	expropt expr SEMICOLON
	{
		$1->push_back($2);
		$$ = $1;
	}
|	%empty
	{
		$$ = new std::vector<AstNode*>();
	}

expr:
	IF expr THEN expr
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::If));
		node->addNode($2);
		node->addNode($4);
		$$ = node;
	}
|	IF expr THEN expr ELSE expr
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::If));
		node->addNode($2);
		node->addNode($4);
		node->addNode($6);
		$$ = node;
	}
|	WHILE expr DO expr
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::While));
		node->addNode($2);
		node->addNode($4);
		$$ = node;
	}
|	LET OBJECT_ID COLON type IN expr
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::Let));
		node->addNode(new AstNode(new Token(@2.first_line, @2.first_column, Token::Object_id, *$2)));
		node->addNode($4);
		node->addNode($6);
		$$ = node;
	}
|	LET OBJECT_ID COLON type ASSIGN expr IN expr
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::Let));
		node->addNode(new AstNode(new Token(@2.first_line, @2.first_column, Token::Object_id, *$2)));
		node->addNode($4);
		node->addNode($6);
		node->addNode($8);
		$$ = node;
	}
|	OBJECT_ID ASSIGN expr
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::Assign));
		node->addNode(new AstNode(new Token(@1.first_line, @1.first_column, Token::Object_id, *$1)));
		node->addNode($3);
		$$ = node;
	}
|	NOT expr
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::UnOp, "not"));
		node->addNode($2);
		$$ = node;
	}
|	MINUS expr
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::UnOp, "-"));
		node->addNode($2);
		$$ = node;
	}
|	ISNULL expr
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::UnOp, "isnull"));
		node->addNode($2);
		$$ = node;
	}
|	expr AND expr 
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "and"));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|	expr EQUAL expr 
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "="));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|	expr LOWER expr
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "<"));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|   expr LOWER_EQ expr 
    {
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "<="));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|	expr PLUS expr
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "+"));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|	expr MINUS expr
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "-"));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|	expr TIMES expr
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "*"));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|	expr DIV expr
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "/"));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|	expr POW expr
	{
		AstNode* node = new AstNode(new Token(@2.first_line, @2.first_column, Token::BinOp, "^"));
		node->addNode($1);
		node->addNode($3);
		$$ = node;
	}
|	OBJECT_ID LPAR args RPAR
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::Call));
		node->addNode(new AstNode(new Token(@1.first_line, @1.first_column, Token::Object_id, "self")));
		node->addNode(new AstNode(new Token(@1.first_line, @1.first_column, Token::Object_id, *$1)));
		node->addNode($3);
		$$ = node;
	}
|	expr DOT OBJECT_ID LPAR args RPAR
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::Call));
		node->addNode($1);
		node->addNode(new AstNode(new Token(@3.first_line, @3.first_column, Token::Object_id, *$3)));
		node->addNode($5);
		$$ = node;
	}
|	NEW TYPE_ID
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::New));
		node->addNode(new AstNode(new Token(@2.first_line, @2.first_column, Token::Type_id, *$2)));
		$$ = node;
	}
|	OBJECT_ID
	{
		AstNode* node = new AstNode(new Token(@1.first_line, @1.first_column, Token::Object_id, *$1));
		$$ = node;
	}
|	literal
	{
		$$ = $1;
	}
|	LPAR expr RPAR
	{
		$$ = $2;
	}
|	block
	{
		$$ = $1;
	}

args:
	argopt arg
	{
		$1->addNode($2);
		$$ = $1;
	}
| %empty
	{
		$$ = new AstNode(NULL);
	}

argopt:
	  argopt arg COMMA
	  {
		$1->addNode($2);
		$$ = $1;
	  }
| %empty
	{
		$$ = new AstNode(NULL);
	}
arg:
   expr
	{
		$$ = $1;
	}

literal:
	STRING_LIT
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, Token::String_lit, *$1));
	}
|	INT_LIT
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, $1));
	}
|	boolean_literal
	{
		$$ = $1;
	}

boolean_literal:
	FALSE
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, Token::Bool_lit, "false"));
	}
|	TRUE
	{
		$$ = new AstNode(new Token(@1.first_line, @1.first_column, Token::Bool_lit, "true"));
	}
%%

static void yyerror(const char *s)
{
	syntax_error(yylloc.first_line, yylloc.first_column);
}

